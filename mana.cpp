#include "mana.h"

//default constructor
Mana::Mana(sf::Font &font, unsigned int size) : sf::Text("Mana : 3", font, size) {
	this->value = 3; //set initial lives to be 3
}

//To Remove Mana
void Mana::RemoveMana() {
	this->value -= 1;
}

//Get the current amount of mana
int Mana::GetMana() {
	return this->value;
}

//Updates the drawing of the lives on the screen
void Mana::Update() {
	this->setString("Mana: " + std::to_string(this->value));
}