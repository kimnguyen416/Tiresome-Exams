#pragma once

#include "bullet.h"

//This class is a specialized bullet for the player to shoot an invisible bullet as long as their sword animation
	//in order to attack their enemies with it.
class PlayerBullet : public Bullet {
public:
	PlayerBullet(Map *map, float x, float y, float direction, float distance) : Bullet(map, x, y, direction, distance){
		this->Load("other/playerbulletspritesheet.png", sf::IntRect(0, 0, 80, 50)); //You can animate the bullet so that's what we're going to do!
		
		//This makes sure the image flips depending on the player's direction
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			this->Flip(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A));
		}
		
		this->Animate(0, 6);
		this->fps = 16 * 6; //slows down the animations 
		this->groupID = 4; //groupID of the player's offensive bullets will be 4 
		this->setColor(sf::Color::White); //change the color of the player's bullet
		this->setScale(1.0f, 1.0f); //make the bullets bigger 

	}
};