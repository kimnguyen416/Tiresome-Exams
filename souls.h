#pragma once

#include <SFML/Graphics.hpp>

//Extends to the text class of SFML. Souls is similar to scores except in this game it will be used as currency.
class Souls : public sf::Text {
public:
	Souls(sf::Font &font, unsigned int size); //default constructor
	inline void SetValue(int value) { //inline allows you to define the function in the header 
		this->value = value;
	}
	void IncrementSouls();
	void Update();
private:
	int value;
};