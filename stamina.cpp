#include "stamina.h"

//default constructor
Stamina::Stamina(sf::Font &font, unsigned int size) : sf::Text("Stamina : 3", font, size) {
	this->value = 3; //set initial lives to be 3
}

//To Remove Stamina
void Stamina::RemoveStamina() {
	this->value -= 1;
}

//Get the current amount of stamina
int Stamina::GetStamina() {
	return this->value;
}

//Recovers stamina over time by 1
int Stamina::StaminaRegen() { 
	return this->value += 1; 
}

//Updates the drawing of the lives on the screen
void Stamina::Update() {
	this->setString("Stamina: " + std::to_string(this->value));
}