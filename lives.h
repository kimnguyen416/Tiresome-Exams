#pragma once

#include <SFML/Graphics.hpp>

//Extends to the text class of SFML since it needs to be written. How many lives that either the enemy or player has.
class Lives : public sf::Text {
public:
	Lives(sf::Font &font, unsigned int size); //default constructor
	inline void SetValue(int value) { //inline allows you to define the function in the header 
		this->value = value; 
	}
	void RemoveLife();
	void Update();
	int GetLife();
private:
	int value;
};