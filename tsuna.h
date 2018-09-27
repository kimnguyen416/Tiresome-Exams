#pragma once

#include "baseNPC.h"

//The Tsuna NPC of Town of Valencia
class TsunaNPC : public BaseNPC {
public:
	TsunaNPC(Dialogue *dialogue, Map *map, std::string text, float x, float y) : BaseNPC(dialogue, "tsuna.png", map, text, x, y) {

	}
};