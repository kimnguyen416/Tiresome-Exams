#pragma once

#include "enemy.h"

//Class of Tsuna's angry form but he's an enemy within the Forest area. 
class TsunaAngry : public Enemy {
public:
	//".png" is referenced to the tsuna's spritesheet that I re-edited so that it would work. 
	TsunaAngry(Map *map, float x, float y) : Enemy(map, "angrytsunaspritesheet.png", x, y, sf::IntRect(0, 0, 60, 75)) { 
		this->velocity = sf::Vector2f(-1.4f, 0);
		this->health = 3; 
		//this->Animate(15, 17); //death animation
		//this->Animate(0, 8); //attack animation
		this->SetFrame(8);
		this->Animate(8, 15); //running animation
		this->yOffset = -2; //determines the y offset of tsuna's positioning. 
		this->fps = 16 * 4; //slows down the animations 
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
		else if (this->health <= 0 && this->currentFrame < 15) { //If I wanted a death anim, these are the things I have to do: this->health <= 0 && this->currentFrame < 15
			//If I want a death anim for the enemy 
			this->SetFrame(15);
			this->Animate(15, 17);
			this->fps = 16 * 12; 
		}
		//If I wanted a death anim then I have to do this below too
		if (this->currentFrame >= 16) {
			this->Destroy();
			saveSystem.IncrementSouls(1); //you get one soul from killing this enemy 
		}
		

		return true;
	}
};