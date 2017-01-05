## Assignment Asteroids using OpenGl and C++
#### author Stephen Collins
#### student number 20061696

##### Refactoring

* Collision detection
	Point circle collision detection, collision between the ship with asteroids and powerups and between bullets and asteroids.
	Powerups are consumed when the ship collides with them, asteroids are destroyed when their health is depleted by either collision with bullets or the ship and 		break into smaller asteroids(random amounts within a range).
* Bonus and power ups
	*Teleporting capability, Implemented but needs improvement, pressing tab teleports the ship.
	*Limited bullets, Player has an ammo count that depletes when fired and increase when collecting a powerup. The ammo count is 		 represented by a yellow bar close to the ship.
	*Ship invincible for a period of time, there is a short grace period where the ship cannot be damaged by an asteroid after 	            collision with one.
	*Ship health or/and extra lives, the ship has health and lives, the health is represented by a health bar close to the ship. The 	  ships health depletes by ten when collision occurs with an asteroid, the ship has 3 lives and will loose one when health drops to  	      zero.
	*There are 3 different powerups, ammo, health and extra life. Colliding with them has the expected affect.
3. Game flow
	Large asteroids are spawned randomly, the larger asteroids break into smaller when destroyed. The powerups spawn randomly and 		provide the player with additional health, lives and ammo.
