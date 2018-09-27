#pragma once

#include <SFML/Audio.hpp>

#include "item.h"

sf::SoundBuffer *soundBuffers = NULL;
sf::Sound *sounds = NULL;

//This is the MP Potion class. It's an item that will recover the players mana. 
class MPPotion : public Item {
public:
	MPPotion(Map *map, float x, float y) : Item("potion_2_4.png", map, x, y) {
		this->groupID = 7;

		if (soundBuffers == NULL) {
			soundBuffers = new sf::SoundBuffer();
			soundBuffers->loadFromFile("Sounds/potiondrink.ogg");
		}
		if (sounds == NULL && soundBuffers != NULL) {
			sounds = new sf::Sound(*soundBuffers);
		}
	}

	//Once the potion is used, it is destroyed and the sound plays
	void Destroy() {
		sounds->play();
		Entity::Destroy();
	}
private:

};