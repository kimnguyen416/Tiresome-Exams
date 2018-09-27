#pragma once

#include <SFML/Graphics.hpp>

//Extends to the text class of SFML since it needs to be written. How many lives that either the enemy or player has.
class Stamina : public sf::Text {
public:
	Stamina(sf::Font &font, unsigned int size); //default constructor
	inline void SetValue(int value) { //inline allows you to define the function in the header 
		this->value = value;
	}
	void RemoveStamina();
	void Update();
	int GetStamina();
	int StaminaRegen(); //boost stamina up by 1 every second or so. 
private:
	int value;
};