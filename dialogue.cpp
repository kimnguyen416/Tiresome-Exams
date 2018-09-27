#include "dialogue.h"

//Dialogue default constructor
Dialogue::Dialogue(sf::Font &font, unsigned int size, sf::RenderWindow *window) : sf::Text("", font, size) {
	//Properly sets the texture of the speech 
	this->texture = new sf::Texture();
	this->texture->loadFromFile("Graphics/sprites/other/dialogueboxes.png"); //grab the image of the dialogue box
	this->setTexture(*this->texture);

	//Positioning of both the text and imagery
	sf::Sprite::setPosition(0, window->getSize().y - sf::Text::getGlobalBounds().height);
	sf::Text::setPosition(5.0f, window->getSize().y - sf::Text::getGlobalBounds().height);
	this->window = window; 
	this->talking = false; //set it to initially false since we're talking to no1 right now
}

//Sets the text of the dialogue as well as scale the dialogue box based on the dialogue itself
void Dialogue::SetText(std::string text) {
	this->talking = true; //when you set the text of the speech bubble, then this indicates that the player is talking to someone
	this->setString(text); //sets the string

	//scale the dialogue box based on speech 
	float scaleWidth = sf::Text::getGlobalBounds().width / sf::Sprite::getTextureRect().width + 0.2f;
	float scaleHeight = sf::Text::getGlobalBounds().height / sf::Sprite::getTextureRect().height + 0.2f;
	
	sf::Sprite::setScale(scaleWidth, scaleHeight);
	
	sf::Sprite::setPosition(this->window->getSize().x / 2 - 45 - sf::Text::getGlobalBounds().width / 2, this->window->getSize().y - sf::Text::getGlobalBounds().height - 64);
	sf::Text::setPosition(this->window->getSize().x / 2 - sf::Text::getGlobalBounds().width / 2 + 5.0f - 20, this->window->getSize().y - sf::Text::getGlobalBounds().height - 64);
}

//Draws out the dialogue box
void Dialogue::Render() {
	//If the NPC has something to say then render, otherwise do not render these images & text
	if (this->talking) {
		this->window->draw((sf::Sprite)*this); //draws out the texture of the dialogue box
		this->window->draw((sf::Text)*this); //draws out the text inside of the box
	}
}


Dialogue::~Dialogue() {
	delete this->texture; 
}


