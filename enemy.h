#pragma once

#include "entity.h"
#include "map.h" //to include the enemies in the map levels
#include "saveSystem.h"

extern SaveSystem saveSystem;

//Base enemy class
class Enemy : public Entity { //make it public so that it can freely reference the entity parent class
public:
	Enemy(Map *map, std::string gfx, float x, float y, sf::IntRect rect = sf::IntRect()); //the intrect is to actually give the enemy's a specific frame size(intrect) when they are animated
	void offset(float x, float y);
	virtual bool Update(sf::RenderWindow *window);
	void Collision(Entity *entity);
protected:
	Map *map; //to know where the enemies are within the map
	int health; //enemy has health
	sf::Vector2f position; //position of the enemy 
};