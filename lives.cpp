#include "lives.h"

//default constructor
Lives::Lives(sf::Font &font, unsigned int size) : sf::Text("Lives : 3", font, size) {
	this->value = 3; //set initial lives to be 3
}

//To Remove a life
void Lives::RemoveLife() {
	this->value -= 1;
}

//Get the current amount of life
int Lives::GetLife() {
	return this->value;
}

//Updates the drawing of the lives on the screen
void Lives::Update() {
	this->setString("Lives: " + std::to_string(this->value));
}