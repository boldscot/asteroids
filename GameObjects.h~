// ===============================================
// @file   GameObjects.h
// @author scollins
// @practical asteroids
// @brief  Header file for game objects
// ===============================================

#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include "../../common/core.h"
#include "../../common/Pool.h"

const float ASTEROID_SCALE = 0.1f;
const int ASTEROID_GRANULARITY = 20;
const float ASTEROID_MAX_RADIUS = 1.2f;
const float ASTEROID_MIN_RADIUS = 0.8f;
const float ASTEROID_ANGLE_VARABILITY = 0.5f;

// ======================================
// BaseGameObject
// ======================================

class BaseGameObject {
public:
	enum State {ASLEEP, ALIVE, DEAD};
	State state;
		
	float damping;
	Vector2f position;
	Vector2f velocity;
	Vector2f acceleration;
	
	float angle;
	float angularVelocity;
	float angularAcceleration;
	
	virtual void render() = 0;
	virtual void update(float dt);
	virtual void reset();
};

// ======================================
// Ship
// ======================================

class Ship: public BaseGameObject {
	float timeToFire;
public:
	bool fire;
	int ammoAmount;
	int health;
	int numLives;
	float collisionDelay;

	void render();
	void update(float dt);
	void reset();
};

// ======================================
// Bullet
// ======================================

class Bullet: public BaseGameObject {
	float timeToDie;
public:
	void render();
	void reset();
	void update(float dt);
};

typedef Pool<Bullet> BulletPool;

// ======================================
// Asteroid
// ======================================

class Asteroid: public BaseGameObject {
public:
	enum Type {LARGE, MEDIUM, SMALL, TINY};
	Type type;
	  
	float size;
	int health;
	Vector2f vertices[ASTEROID_GRANULARITY];

	void render();
	void reset();
	void update(float dt);
};

typedef Pool<Asteroid> AsteroidPool;

// ======================================
// Powerups
// ======================================
class PowerUp: public BaseGameObject {
public:
	enum Type {AMMO, HEALTH, EXTRA_LIFE, SHIELD};
	Type type;
	
	float size;
	
	void render();
	void reset();
	void update(float dt);
};

typedef Pool<PowerUp> PowerUpPool;

#endif
