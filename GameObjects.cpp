// ===============================================
// @file   GameObjects.cpp
// @author scollins
// @practical asteroids
// @brief  File for game objects
// ===============================================

#include "asteroids.h"
#include "GameObjects.h"

// ======================================
// BaseGameObject
// ======================================

void BaseGameObject::update(float dt) {
	// linear movement
	velocity += dt*acceleration;
	position += dt*velocity;
	velocity *= pow(damping, dt);
	
	//anglular movement
	angle += dt*angularVelocity;
}

void BaseGameObject::reset() {
	state = BaseGameObject::ALIVE;
	velocity = Vector2f::ZERO;
	position = Vector2f::ZERO;
	acceleration = Vector2f::ZERO;
	angle = 0;
	angularVelocity = 0;
	damping = 1;
}

// ======================================
// Ship
// ======================================

void Ship::reset() {
	BaseGameObject::reset();
	
	position = Vector2f(ASPECT/2,0.5);
	damping = 0.8;
	
	timeToFire = 0;
	ammoAmount = 500;
	health = 100;
	collisionDelay = SHIP_TIME_COLLIDE;
	if (numLives == 0) numLives = 3; 
}

void Ship::update(float dt) {
	// TODO - update position (using base class)
	BaseGameObject::update(dt);

	// TODO - process ship control events (update velocity)
	velocity = velocity * SHIP_MAX_SPEED;
	angularVelocity = angularVelocity * SHIP_MAX_ANGULAR_VELOCITY;
	
	// TODO - process fire events
	if (timeToFire > 0) timeToFire -=dt;
	if (collisionDelay > 0) collisionDelay -=dt;

	if (fire && timeToFire <= 0 && ammoAmount > 0) {
		Bullet & bullet = bullets.allocate();
		bullet.reset();
		bullet.position = position;
		bullet.angle = angle;		

		// get the bullet traveling at the correct angle
		float rad = degToRad(angle);
		bullet.velocity.x = cos(rad) /2;
		bullet.velocity.y = sin(rad) /2;

		timeToFire = SHIP_FIRE_DELAY;
		ammoAmount--;
	}
	
	if (health <= 0 ) {
		numLives--;
		reset();
	}
}

// ======================================
// Bullet
// ======================================

void Bullet::reset() {
	// TODO - reset start (using base class),  and reset time left to die.
	BaseGameObject::reset();
	timeToDie = BULLET_DIE_DELAY;	
}

void Bullet::update(float dt) {
	// TODO - update position (using base class), update and check time left to die.
	BaseGameObject::update(dt);
	if (timeToDie > 0 ) timeToDie -= dt;
}


// ======================================
// Asteroid
// ======================================

void Asteroid::reset() { 
	BaseGameObject::reset();
	
	// give the asteroids a size and health
	if (type == TINY) {
		health = 1;
		size = 0.2f;
	} else if (type == SMALL) {
		health = 2;
		size = 0.4f;
	} else if (type == MEDIUM) {
		health = 3;
		size = 0.75f;
	} else if (type == LARGE) {
		health = 4;
		size = 1.0f;
	}
  
	// TODO - create random asteroid as outlined in class 
	float vertexAngle = 360 / ASTEROID_GRANULARITY;
	for(int k = 0; k < ASTEROID_GRANULARITY; ++k){ 
		float randomPoint = randomFloat(ASTEROID_MIN_RADIUS,ASTEROID_MAX_RADIUS); 
		vertexAngle += ((360/ASTEROID_GRANULARITY) + randomFloat(-1,1) * ASTEROID_ANGLE_VARABILITY); 
		vertices[k].x = randomPoint * cos(vertexAngle); 
		vertices[k].y = randomPoint * sin(vertexAngle); 
	}

	// TODO - random rotation angle 
	angularVelocity = randomFloat(-100,100); 
	angle = randomFloat(1,360); 
	velocity = Vector2f(cos(degToRad(angle))/2 ,sin(degToRad(angle))/2).normal() * randomFloat(ASTEROID_MIN_SPEED,ASTEROID_MAX_SPEED);
}


void Asteroid::update(float dt) {
	BaseGameObject::update(dt);
}

// ======================================
// PowerUp
// ======================================

void PowerUp::reset() {
	BaseGameObject::reset();
	
	// TODO - random rotation angle 
	angularVelocity = randomFloat(-100,100); 
	angle = randomFloat(1,360); 
	velocity = Vector2f(cos(degToRad(angle))/2 ,sin(degToRad(angle))/2).normal() * randomFloat(ASTEROID_MIN_SPEED,ASTEROID_MAX_SPEED);
}

void PowerUp::update(float dt) {
	BaseGameObject::update(dt);
}
