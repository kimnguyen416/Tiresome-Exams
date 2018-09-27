#pragma once

#include <SFML/Graphics.hpp>

//Class that is in charge of the conversation/dialogue between entities
//Dialogue inherits from both text and sprite(to get the text of the dialogue as well as the imagery to cover that text)
class Dialogue : public sf::Text, public sf::Sprite { 
public:
	Dialogue(sf::Font &font, unsigned int size, sf::RenderWindow *window); //default constructor
	bool talking; //checks when the player decides to speak with an NPC
	void SetText(std::string text); //to set the text to a specific dialogue text
	void Render(); //to draw out the dialogue box
	~Dialogue(); //to delete and free some memory after the application is done
private:
	sf::Texture *texture; //need the dialogue box
	sf::RenderWindow *window;
};