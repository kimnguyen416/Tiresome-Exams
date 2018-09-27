#pragma once

#include "baseNPC.h"

//The Hughes NPC of Town of Valencia
class HughesNPC : public BaseNPC {
public:
	HughesNPC(Dialogue *dialogue, Map *map, std::string text, float x, float y) : BaseNPC(dialogue, "hughes.png", map, text, x, y) {

	}
};