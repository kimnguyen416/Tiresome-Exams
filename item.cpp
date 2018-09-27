#include "item.h"
#include "mainGame.h"


Item::Item(std::string gfx, Map *map, float x, float y) {
	this->position = sf::Vector2f(x, y);
	this->Load(gfx); //loads the NPCs based on the item tileset
	this->setPosition(x, y); //set position of the item based on x & y
	this->map = map; //sets the map properly
	this->groupID = 6; //all items have a groupId of 6
}

//function made to move the Item's position sprite up and down
void Item::offset(float x, float y) {
	this->move(x, y);
	this->position.x += x;
	this->position.y += y;
}

bool Item::Update(sf::RenderWindow *window) {
	//keeps track of the scrolling and the Item's position at the same time
	this->setPosition(this->position.x - Entity::scroll.x, this->position.y - Entity::scroll.y);
	Entity::Update(window); //update the entity's movement
	return true;
}

