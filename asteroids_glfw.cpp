// ===============================================
// @file   GameObjects.h
// @author scollins
// @practical asteroids
// @brief  File to handle rendering of all game objects
// ===============================================
#include "asteroids.h"
float timer = 0;
void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1,0,0);
    
	ship.render();
	drawHealthBar();
	drawAmmoCount();
	//for (int k = 0; k < ship.numLives; k++) drawLives(k/2); // not properly implememnted
	for (int k=0; k<bullets.size(); ++k) bullets[k].render();
	for (int k=0; k<asteroids.size(); ++k) asteroids[k].render();
	for (int k=0; k<powerups.size(); ++k) powerups[k].render();
	
	glfwSwapBuffers(); 

	// update frame timer
	if (timer <= 0) timer -=dt;
	previousTime = currentTime;
	currentTime = glfwGetTime();
	dt = clamp(currentTime - previousTime, 0.01, 0.1);
}

void Ship::render() {

    glColor3f(0,0,1);
	glPushMatrix();
	glTranslatef(position.x, position.y, 0);
	glScalef(0.004, 0.003, 1);
	glRotatef(angle, 0,0,1);
	//glColor3ub(0,200, 0);	
	glBegin(GL_QUADS);		
		glVertex3f(12, 0, 0);
		glVertex3f(-5, 5, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(12, 0, 0);
		glVertex3f(-5, -5, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 0);
    glEnd();	
	glPopMatrix();
}

void PowerUp::render() {
	if (type == AMMO) glColor3f(1,1,0);
	else if (type == HEALTH) glColor3f(0,1,0);
	else if (type == EXTRA_LIFE) glColor3f(0,1,1);
	//else if (type == SHIELD) glColor3f(1,0,1);
	glPushMatrix();
	glTranslatef(position.x, position.y, 0);
	glScalef(0.04, 0.04, 1);
	drawCircles((position.x, position.y));
	glPopMatrix();
}

void drawHealthBar() {
	glColor3f(0,1,0);
	glPushMatrix();
	glTranslatef(ship.position.x, ship.position.y, 0);
	glScalef(0.15, 0.08, 1);

	glBegin(GL_QUADS);		
		glVertex3f(0, 0, 0);
		glVertex3f(ship.health/100.0f, 0, 0);
		glVertex3f(ship.health/100.0f, .1, 0);
		glVertex3f(0, .1, 0);
    glEnd();	
	glPopMatrix();
}

void drawLives(float distance) {
	glColor3f(1,0,0);
	glPushMatrix();
	glTranslatef(ship.position.x, ship.position.y + 0.008f, 0);
	glScalef(0.02, 0.3, 1);
		glBegin(GL_QUADS);		
			glVertex3f(0, 0, 0);
			glVertex3f(1, 0, 0);
			glVertex3f(1, 0.1, 0);
			glVertex3f(0, 0.1, 0);
    	glEnd();	
	glPopMatrix();
}

void drawAmmoCount() {
	glColor3f(1,1,0);
	glPushMatrix();
	glTranslatef(ship.position.x, ship.position.y - 0.008f, 0);
	glScalef(0.15, 0.08, 1);

	glBegin(GL_QUADS);		
		glVertex3f(0, 0, 0);
		glVertex3f(ship.ammoAmount/500.0f, 0, 0);
		glVertex3f(ship.ammoAmount/500.0f, .1, 0);
		glVertex3f(0, .1, 0);
    glEnd();	
	glPopMatrix();
}

void drawCircles(Vector2f vecPosition) {
	const int segments = 100;
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);    
		for (int k = 0; k <= segments; ++k) glVertex3f(cos(2*PI/segments*k), sin(2*PI/segments*k), 0);
	glEnd();
	glPopMatrix();
}

void Bullet::render() {

    glColor3f(1,0,0);
	glPushMatrix();
	glTranslatef(position.x, position.y, 0);
	glRotatef(angle, 0,0,1);
	glScalef(0.04, 0.04, 1);
	//printf("angle = %f\n", angle);

	glBegin(GL_QUADS);		
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(1, .1, 0);
		glVertex3f(0, .1, 0);
    glEnd();	
	glPopMatrix();
}

void Asteroid::render () {
    glColor3f(1,1,1);
	glPushMatrix();
	glTranslatef(position.x, position.y, 0);
	glScalef(ASTEROID_SCALE*size, ASTEROID_SCALE*size, 1);
	glRotatef(angle, 0,0,1);
	glBegin(GL_TRIANGLE_FAN);	
		for (int k=0; k<ASTEROID_GRANULARITY; ++k) {
			glVertex3f(vertices[k].x,vertices[k].y, 0);
		}
    glEnd();

	//drawCircle(Vector2f::ZERO, size);

	glPopMatrix();	
}

void getInput() {
	if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED)) {
		gameState = GAME_QUIT;
		return;
	}

    if (glfwGetKey('R')) {
		ship.reset();
    }
	
    ship.acceleration = Vector2f::ZERO;
	if (glfwGetKey(GLFW_KEY_UP)) {
		float rad = degToRad(ship.angle);
		ship.acceleration = SHIP_MAX_ACCELERATION * Vector2f(cos(rad)/2,sin(rad)/2);
    } else if (glfwGetKey(GLFW_KEY_DOWN)) {
		ship.velocity *= 0.5f;
	}
	
	// bug here, cannot press left, up and space together or left turning won't work
    if (glfwGetKey(GLFW_KEY_LEFT)) {
		ship.angularVelocity = SHIP_MAX_ANGULAR_VELOCITY;
    } else if (glfwGetKey(GLFW_KEY_RIGHT)) {
		ship.angularVelocity = -SHIP_MAX_ANGULAR_VELOCITY;
    } else {
		ship.angularVelocity = 0;
	}
 	
	if (glfwGetKey(GLFW_KEY_TAB)) {
		timer = 3.0f;
		ship.position.x+= 0.05;
		ship.position.y+= 0.05;
	}

	ship.fire = glfwGetKey(GLFW_KEY_SPACE);
}

