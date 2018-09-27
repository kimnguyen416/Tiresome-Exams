#pragma once

#include "entity.h"
#include "map.h"

//Base class for projectiles(bullets). Can be used as a way for the player to interact with objects/NPCs or as an attack range
class Bullet : public Entity {
public:
	Bullet(Map *map, float x, float y, float direction, float distance);
	bool Update(sf::RenderWindow *window);
	void Collision(Entity *entity);
protected:
	sf::Vector2f position; 
private:
	float distance; //gives the bullet a fixed range
	Map *map;
};