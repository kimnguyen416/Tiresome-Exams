#pragma once

#include "baseNPC.h"

//The Bianchi NPC in the Town of Valencia. 
class BianchiNPC : public BaseNPC {
public:
	BianchiNPC(Dialogue *dialogue, Map *map, std::string text, float x, float y) : BaseNPC(dialogue, "bianchi.png" , map, text, x, y) {

	}
};