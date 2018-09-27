#pragma once

#include "baseNPC.h"

//The Basil NPC in the Town of Valencia. 
class BasilNPC : public BaseNPC {
public:
	BasilNPC(Dialogue *dialogue, Map *map, std::string text, float x, float y) : BaseNPC(dialogue, "basil.png", map, text, x, y) {

	}
};