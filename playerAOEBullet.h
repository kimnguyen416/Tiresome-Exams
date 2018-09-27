#pragma once

#include "bullet.h"

//This is another bullet of the player except it is 
class PlayerAOEBullet : public Bullet {
public:
	PlayerAOEBullet(Map *map, float x, float y, float direction, float distance) : Bullet(map, x, y, direction, distance) {
		this->groupID = 8; //groupID of 8 so that when the enemy gets hit by this special move then it does more damage
		this->setColor(sf::Color::Transparent); 
		this->setScale(1.0f, 1.0f); //make the bullets bigger 
	}
};