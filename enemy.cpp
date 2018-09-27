#include "enemy.h"
#include "mainGame.h" //enemies will be in the mainGame


Enemy::Enemy(Map *map, std::string gfx, float x, float y, sf::IntRect rect) {
	this->position = sf::Vector2f(x, y);
	this->Load(gfx, rect); //makes sure tht the entity has their frame size set correctly(which is what the rect is) 
	this->setPosition(x, y); //set position of the enemy based on x & y
	this->map = map; //sets the map properly
	this->groupID = 5; //groupID of enemies will be 5(CHANGE THIS LATER!!)
	this->health = 1; //give the enemy 1 HP initially
	this->velocity = sf::Vector2f(0, 0); //enemy has a speed
}

//function made to move the Enemy's position sprite up and down
void Enemy::offset(float x, float y) { //got rid of the move to stop making the enemies glitch whenever the player jumps
	this->position.x += x;
	this->position.y += y;
}


bool Enemy::Update(sf::RenderWindow *window) {
	//keeps track of the scrolling and the enemy's position at the same time
	this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);

	//checks for if the enemy's health is == 0, then they should be destroyed
	if (this->health <= 0) {
		this->Destroy(); 
	}
	
	this->offset(this->velocity.x, this->velocity.y);
	return true;
}

//Check for collisions when enemy is collided with another entity
void Enemy::Collision(Entity *entity) {
	//need to get groupID of the player's sword bullet. Add later and it needs to be tested for its range
	//collisions based on groupID
	switch (entity->GroupID()) {
	case 4: //player's offensive bullet(NOT THE INVISIBLE INTERACTION BULLET which has a groupID of 2), just the normal player attack
		entity->Destroy();
		this->health -= 1; //reduce enemy's health by 1 every time it is hit by the player's bullet
		break;
	case 8: //this is the player's special attack move. so it should do more damage.
		entity->Destroy();
		this->health -= 3; //does 3 damage if the enemy gets hit by it. 
		break;
	}
}