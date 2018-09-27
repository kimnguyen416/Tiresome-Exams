#include "bullet.h"

//Bullet default constructor
Bullet::Bullet(Map *map, float x, float y, float direction, float distance) { 
	this->active = 1; //make the ship be active
	this->Load("other/fireball.png");
	this->setColor(sf::Color::Transparent);
	 //the bullet here will act as a way for the player to interact with other entities.
	//so it will be an invisible bullet with a fixed range
	this->velocity.x = cos(direction / 180.0f * 3.14f) * 3; //the multiplication part is to convert from radians to degrees
	this->velocity.y = sin(direction / 180.0f * 3.14f) * 3;

	position = sf::Vector2f(x + this->getGlobalBounds().width, y + this->getGlobalBounds().height);

	this->groupID = 2; //the bullet's groupID is set to 2

	//position on where the bullet is exiting the player's body
	this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y); //we do the - to ensure that where we put it it will be in the center of the window screen
	this->setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2); //this make sures it rotates on the center axis
	this->setScale(0.5f, 0.5f);
	this->distance = distance;
	this->map = map;
}


bool Bullet::Update(sf::RenderWindow *window) {
	this->position += this->velocity;
	this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);
	//this->rotate(this->velocity.x); //rotates the bullet
	//if the bullet goes offscreen, it should be destrooyed 
	if (this->getPosition().y <= 0 || this->getPosition().y + this->getGlobalBounds().height >= window->getSize().y) {
		this->Destroy();
	}

	//Calculates distance of the bullet
	this->distance -= sqrt(this->velocity.x * this->velocity.x + this->velocity.y * this->velocity.y); //calculates the distance on every frame(distance formula) WOW MATH IS ACTUALLY IMPORTANT :OOOO
	if (this->distance <= 0) { //if the distance is this close
		this->Destroy(); //destroy the bullet
	}

	//If the bullet collides with the wall blocks, it should get destroyed instead of going offscreen
	if (this->map->CheckCollision(this, NONE).group != NO_GROUP && this->map->CheckCollision(this, NONE).group != WATER && this->map->CheckCollision(this, NONE).group != LEAVE) {
		this->Destroy();
	}

	//Entity::Update(window); //also calls the entity update to update the velocity of the entity as well
	return true;
}

//What happens when another entity collides with the bullet depending on the groupID
void Bullet::Collision(Entity *entity) {
	
}