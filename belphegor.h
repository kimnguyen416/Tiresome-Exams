#pragma once

#include "baseNPC.h"

//The Belphegor NPC of Town of Valencia
class BelphegorNPC : public BaseNPC {
public:
	BelphegorNPC(Dialogue *dialogue, Map *map, std::string text, float x, float y) : BaseNPC(dialogue, "belphegor.png", map, text, x, y) {

	}
};