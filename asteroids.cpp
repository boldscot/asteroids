// ===============================================
// @file   asteroids.cpp
// @author scollins
// @practical asteroids
// @brief  Main file for PSP+GLFW asteroids game
// ===============================================

#include "asteroids.h"

Ship ship;
BulletPool bullets(1000);
AsteroidPool asteroids(20);
PowerUpPool powerups(5);

int main() {

	bool done = false;
	
	initGraphics("Asteroids");
	
	while (!done) {

		switch (gameState) {

			case(GAME_INTRO): {
				changeGameState(GAME_START);
				break;
			}

			case(GAME_START): {
				changeGameState(LEVEL_START);
				break;
			}

			case(LEVEL_START): {
				ship.reset();
				ship.position = Vector2f(ASPECT/2, 0.5f);
				ship.velocity = Vector2f::ZERO;
				Asteroid & asteroid = asteroids.allocate();
				asteroid.type = Asteroid::LARGE;
				asteroid.reset();
				asteroid.position = Vector2f(randomFloat(0.01, ASPECT), randomFloat(0.01f, 1.0f));

				PowerUp & powerUp = powerups.allocate();
				powerUp.type = PowerUp::AMMO;
				powerUp.reset();
				powerUp.position = Vector2f(ASPECT/3, 1.0/2);
				changeGameState(LEVEL_PLAY);
				break;
			}

			case(LEVEL_PLAY): {
				update();
				render();
				getInput();
				break;
			}

			case(LEVEL_OVER): {
				changeGameState(GAME_OVER);
				break;
			}

			case(GAME_OVER): {
				changeGameState(GAME_QUIT);
				break;
			}

			case(GAME_QUIT): {
				done = true;
				break;
			}

			default: {
				printf ("Game state %d is not valid.\n", gameState);				
			}
		}
	}
	
	deinitGraphics();
}

bool wrapGameObject(BaseGameObject & o) {
	Vector2f currentPos = o.position;  // Store position so we can return true or false if position need updating 
	
	// TODO - check if a game object is outside screen area and wrap it if needed.
	if (currentPos.x > ASPECT) o.position.x = 0.01;
	else if (currentPos.x < 0) o.position.x = ASPECT - 0.01;
	else if (currentPos.y < 0) o.position.y = 0.99;
	else if (currentPos.y > 1) o.position.y = 0.01;

	// TODO - return true if wraping was performed.
	return ( (currentPos.x < 0) || (currentPos.x > ASPECT) || (currentPos.y < 0) || (currentPos.y > 1) );
}

void update() {

	// TODO - update ship
	ship.update(dt);
	
	// TODO - update bullets
	for (int b = 0; b < bullets.size(); ++b) {
		bullets[b].update(dt);
		if (wrapGameObject(bullets[b])) bullets.free(b);
	}

	// TODO - check collisions bullets-asteroids
	for (int a = 0; a < asteroids.size(); a++) {
		for (int b = 0; b < bullets.size(); b++) {				
			if (checkBulletAsteroidCollision(bullets[b], asteroids[a])) {
				bullets.free(b);
				if (asteroids[a].health > 0) asteroids[a].health-=1; 
			}
		}

		if (asteroids[a].health <= 0) {
			spawnAsteroids(asteroids[a].type, asteroids[a].position);
			asteroids.free(a);
		}
	}

	// TODO - check collisions ship-asteroids
	if (ship.collisionDelay <= 0 ) {
		for (int a = 0; a< asteroids.size(); ++a) {
			if (isPointInAsteroid(ship.position + Vector2f(12*0.004, 0), asteroids[a])
				|| isPointInAsteroid(ship.position + Vector2f(-5*0.004, 5*0.003), asteroids[a])
				|| isPointInAsteroid(ship.position + Vector2f(-5*0.004, -5*0.003), asteroids[a]) ) {
			
				ship.collisionDelay = SHIP_TIME_COLLIDE;
				ship.health -=10;
				asteroids[a].health -= 1;
				
				std::cout << ship.health << std::endl;

				if (asteroids[a].health <= 0) {
					spawnAsteroids(asteroids[a].type, asteroids[a].position);
					asteroids.free(a);
				}
			}
		}
	}
	// TODO - check collisions ship-powerups
	for (int p = 0; p< powerups.size(); ++p) {
		if ( isPointInPowerUp(ship.position + Vector2f(12*0.004, 0), powerups[p]) 
			|| isPointInPowerUp(ship.position + Vector2f(-5*0.004, 5*0.003), powerups[p])  
			|| isPointInPowerUp(ship.position + Vector2f(-5*0.004, -5*0.003), powerups[p]) ) {
				
				if (powerups[p].type == PowerUp::AMMO) ship.ammoAmount = 500;
				else if (powerups[p].type == PowerUp::HEALTH) ship.health = 100;
				else if (powerups[p].type == PowerUp::EXTRA_LIFE && ship.numLives < 3) ship.numLives++;
				powerups.free(p);
		}
	}
	
	// TODO - update asteroids
	for (int a = 0; a< asteroids.size(); ++a) {
		asteroids[a].update(dt);
		wrapGameObject(asteroids[a]);
	}

	// spawn large asteroids randomly
	float rng = randomFloat(0, 10000);
	if (rng < 1 && asteroids.size() < (asteroids.capacity()) ) {
		Asteroid & asteroid = asteroids.allocate();
		asteroid.type = Asteroid::LARGE;
		asteroid.reset();
		asteroid.position = Vector2f(randomFloat(0.01, ASPECT), randomFloat(0.01f, 1.0f));
	}

	// TODO - update powerups
	for (int p = 0; p< powerups.size(); ++p) {
		powerups[p].update(dt);
		wrapGameObject(powerups[p]);
	}

	spawnPowerUps();
	wrapGameObject(ship);
}

bool isPointInPowerUp(Vector2f position, PowerUp & powerup) {
	return sqr(position.x - powerup.position.x) + sqr(position.y -  powerup.position.y) < sqr(0.04);
}

bool isPointInAsteroid(Vector2f position, Asteroid & asteroid) {
	return sqr(position.x - asteroid.position.x) + sqr(position.y - asteroid.position.y) < sqr(asteroid.size * ASTEROID_SCALE);
}

bool checkBulletAsteroidCollision(Bullet & bullet, Asteroid & asteroid) {
	// TODO - check bullet-asteroid collision
	return sqr(bullet.position.x - asteroid.position.x) + sqr(bullet.position.y - asteroid.position.y) < sqr(asteroid.size * ASTEROID_SCALE);
}

// function for spawning asteroids after one has been destroyed
void spawnAsteroids(Asteroid::Type type, Vector2f position) {
	int numOfAsteroids = 0;
	Asteroid::Type typeToSpawn;
	
	// choose number of asteroids and type
	if (type==Asteroid::LARGE) {
		numOfAsteroids = int(randomFloat(2, 4));
		typeToSpawn = Asteroid::MEDIUM;
	} else if (type==Asteroid::MEDIUM) {
		numOfAsteroids = int(randomFloat(2, 5));
		typeToSpawn = Asteroid::SMALL;
	} else if (type==Asteroid::SMALL) {
		numOfAsteroids = int(randomFloat(3, 7));
		typeToSpawn = Asteroid::TINY;
	}

	for (int counter = 0; counter < numOfAsteroids; ++counter) {
		if ((asteroids.size() + numOfAsteroids) < asteroids.capacity()) {
			Asteroid & asteroid = asteroids.allocate();
			asteroid.type = typeToSpawn;
			asteroid.reset();
			asteroid.position = position;
		}
	}
}

void spawnPowerUps() {
	// spawn large asteroids randomly
	float rng = randomFloat(0, 10000);
	if (rng < 1 && powerups.size() < (powerups.capacity()) ) {
		PowerUp & powerup = powerups.allocate();
		powerup.type = PowerUp::Type(int(randomFloat(0, 3)));
		powerup.reset();
		powerup.position = Vector2f(randomFloat(0.01f, ASPECT), randomFloat(0.01f, 1.0f));
	}
}
