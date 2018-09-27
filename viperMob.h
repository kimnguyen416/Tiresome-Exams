#pragma once

#include "enemy.h"

//Class of the Mob Bianchi but he's an enemy within the Volcano area. 
class ViperMob : public Enemy {
public:
	//".png" is referenced to the tsuna's spritesheet that I re-edited so that it would work. 
	ViperMob(Map *map, float x, float y) : Enemy(map, "viperspritesheet.png", x, y, sf::IntRect(0, 0, 60, 70)) { 
		this->health = 5; 
		this->velocity = sf::Vector2f(-0.9f, 0);
		this->Animate(0, 7); //running animation
		//this->Animate(7, 11); //dead animation
		//this->Animate(11, 15); //atk animation
		this->yOffset = -1; //determines the y offset of the viper's positioning. 
		this->fps = 16 * 9; //slows down the animations 
	}

	//Constantly updates the enemy's movements and behavior
	bool Update(sf::RenderWindow *window) {
		//keeps track of the scrolling and the enemy's position at the same time
		this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);

		if (this->health > 0) {
			//Checks the collisions of the ground tiles to determine the direction of the enemy
			int tileL = this->map->CheckCollision(this, Direction::RIGHT).group;
			int tileR = this->map->CheckCollision(this, Direction::LEFT).group;
			int tileBL = this->map->CheckCollision(this, Direction::BOTTOM_RIGHT).group;
			int tileBR = this->map->CheckCollision(this, Direction::BOTTOM_LEFT).group;

			//checks when the enemy walks to the left on the x axis
			if ((tileL == GROUND || tileBL != GROUND || this->position.x < 0) && this->velocity.x < 0) {//before it reaches the water tile on the town of valencia then it goes back and forth. Can reapply this knowledge in the forest area. 
				this->velocity.x *= -1;
			}
			//checks for when the enemy walks to the right on the x axis
			if ((tileR == GROUND || tileBR != GROUND || this->position.x + this->getGlobalBounds().width > this->map->getGlobalBounds().width) && this->velocity.x > 0) {//before it reaches the water tile on the town of valencia then it goes back and forth. Can reapply this knowledge in the forest area. 
				this->velocity.x *= -1;
			}

			this->Flip(this->velocity.x < 0); //this will flip the wolf enemy (in the vid he did < but I need > to make him flip in the correct orientation)
			this->offset(this->velocity.x, this->velocity.y); //moves the wolf back and forth when they still have health
		}
		//checks for if the enemy's health is == 0, then they should be destroyed
		else if (this->health <= 0 && this->currentFrame < 7) { //If I wanted a death anim, these are the things I have to do: this->health <= 0 && this->currentFrame < 15
			//If I want a death anim for the enemy 
			this->SetFrame(7);
			this->yOffset = -20;
			this->Animate(7, 11); //dead animation
			this->fps = 16 * 15; 
		}
		//If I wanted a death anim then I have to do this below too
		if (this->currentFrame >= 10) {
			this->Destroy();
			saveSystem.IncrementSouls(1); //you get one soul from killing this enemy 
		}
		

		return true;
	}
};