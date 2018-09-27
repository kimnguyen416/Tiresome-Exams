#pragma once

#include <SFML/Audio.hpp>

#include "item.h"

sf::SoundBuffer *soundBuffer = NULL;
sf::Sound *sound = NULL;

//This is the HP Potion class. It's an item that will recover the players health. 
class HPPotion : public Item{
public: 
	HPPotion(Map *map, float x, float y) : Item("potion_2_3.png", map, x, y){
		this->groupID = 6; 

		if (soundBuffer == NULL) {
			soundBuffer = new sf::SoundBuffer();
			soundBuffer->loadFromFile("Sounds/potiondrink.ogg"); 
		}
		if (sound == NULL && soundBuffer != NULL) {
			sound = new sf::Sound(*soundBuffer);
		}
	}

	//Once the potion is used, it is destroyed and the sound plays
	void Destroy(){
		sound->play(); 
		Entity::Destroy();
	}
private:
	
};