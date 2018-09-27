#pragma once

#include "entity.h"
#include "map.h"
#include "dialogue.h"

//Base class of the picking up items feature 
class Item : public Entity { //make it public so that it can freely reference the entity parent class
public:
	Item(std::string gfx, Map * map, float x, float y);
	void offset(float x, float y);
	bool Update(sf::RenderWindow *window);
protected:
	Map * map; //to know where the items are on each map
private:
	sf::Vector2f position; //positioning of the items
};