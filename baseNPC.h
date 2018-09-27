#pragma once

#include "entity.h"
#include "map.h"
#include "dialogue.h"


//Base class of the basic features an NPC should have
class BaseNPC : public Entity { //make it public so that it can freely reference the entity parent class
public:
	BaseNPC(Dialogue *dialogue, std::string gfx, Map * map, std::string text, float x, float y);
	void offset(float x, float y);
	bool Update(sf::RenderWindow *window);
	void Collision(Entity *entity);
protected:
	std::string text; //text specifically for this NPC //I MIGHT NEED A VECTOR INSTEAD FOR MULTIPLE TEXT DIALOGUES & SOME BOOLS TO KNOW WHEN IT ENDS OR USE THE VECTOR FUNCTIONS TO KNOW WHEN IT ENDS
	Map * map; //to know where the NPCs are on each map
private:
	sf::Vector2f position; //positioning of the npcs
	Dialogue *dialogue; //the NPCs will be holding a specific dialogue conversation
};