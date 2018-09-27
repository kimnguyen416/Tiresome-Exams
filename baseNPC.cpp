#include "baseNPC.h"
#include "mainGame.h"
#include <iostream>


BaseNPC::BaseNPC(Dialogue *dialogue, std::string gfx, Map *map, std::string text, float x, float y) {
	this->position = sf::Vector2f(x, y);
	this->Load(gfx); //loads the NPCs based on the NPC tileset
	this->setPosition(x, y); //set position of the NPC based on x & y
	this->map = map; //sets the map properly
	this->groupID = 3; //all NPCs have a groupId of 3
	this->dialogue = dialogue;
	this->text = text;
}

//function made to move the NPC's position sprite up and down
void BaseNPC::offset(float x, float y) {
	this->move(x, y);
	this->position.x += x;
	this->position.y += y;
}

bool BaseNPC::Update(sf::RenderWindow *window) {
	//keeps track of the scrolling and the NPC's position at the same time
	this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);
	Entity::Update(window); //be sure to constantly update the entity's movement(in this case the player's movement)1
	return true;
}

//Check for collisions with the player's invisible bullet(which allows the player to interact w/ the NPC)
void BaseNPC::Collision(Entity *entity) {
	//Collisions based on groupID
	switch (entity->GroupID()) { //if the NPC gets hit by the interaction bullet from the player
	case 2:
		dialogue->SetText(this->text);
		entity->Destroy();
		break;
	}
}