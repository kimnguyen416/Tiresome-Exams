#include "souls.h"

//default constructor
Souls::Souls(sf::Font &font, unsigned int size) : sf::Text("Souls : 0", font, size) {
	this->value = 0; //set initial score to be 0; 
}

//To Increase the Souls by one for now. We have to change the amount later(maybe have multiple functions with varying amounts or changing the value number)
void Souls::IncrementSouls() {
	this->value += 1; 
}

//Update the drawing of the Souls on the screen
void Souls::Update() {
	this->setString("Souls: " + std::to_string(this->value)); 
}