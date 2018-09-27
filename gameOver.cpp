#include "gameOver.h"
#include "mainMenu.h"//to go back to the mainMenu from the game over screen

void GameOver::Initialize(sf::RenderWindow *window) {
	//Just all of the stuff needed to get the game over text in a proper format
	enterKey = false;
	this->font = new sf::Font();
	this->font->loadFromFile("Graphics/fonts/PandoraFont.otf");

	this->gameOverText = new sf::Text("  Game Over", *this->font, 100U);
	this->gameOverText->setFillColor(sf::Color::Red);
	this->gameOverText->setOrigin(sf::Vector2f(this->gameOverText->getGlobalBounds().width / 2, this->gameOverText->getGlobalBounds().height / 2));
	this->gameOverText->setPosition(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
}

void GameOver::Update(sf::RenderWindow *window) {
	//If the player presses enter after entering the game over screen, this should return the player back to the main menu
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return) && enterKey) {
		coreState.SetState(new MainMenu());
	}

	enterKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return);
}

//Draw the game over text to the game over window
void GameOver::Render(sf::RenderWindow *window) {
	window->draw(*this->gameOverText);
}

//Destroy the game over text
void GameOver::Destroy(sf::RenderWindow *window) {
	delete this->gameOverText;
	delete this->font;
}

