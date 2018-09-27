#pragma once

#include "baseNPC.h"

//The Dino NPC in the Town of Valencia. 
class DinoNPC : public BaseNPC {
public:
	DinoNPC(Dialogue *dialogue, Map *map, std::string text, float x, float y) : BaseNPC(dialogue, "dino.png", map, text, x, y) {

	}
};