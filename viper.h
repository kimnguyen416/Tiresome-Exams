#pragma once

#include "baseNPC.h"

//The viper NPC of Town of Valencia
class ViperNPC : public BaseNPC {
public:
	ViperNPC(Dialogue *dialogue, Map *map, std::string text, float x, float y) : BaseNPC(dialogue, "viper.png", map, text, x, y) {

	}
};