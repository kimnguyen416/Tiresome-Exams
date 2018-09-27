#include "winScreen.h"
#include "mainMenu.h" //goes back to the mainMenu from the winScreen

void WinScreen::Initialize(sf::RenderWindow *window) {
	//Just all of the stuff needed to get the win screen text in a proper format
	enterKey = false;
	this->font = new sf::Font();
	this->font->loadFromFile("Graphics/fonts/PandoraFont.otf");

	this->winText = new sf::Text("     You Win!", *this->font, 100U);
	this->winText->setFillColor(sf::Color::Green);
	this->winText->setOrigin(sf::Vector2f(this->winText->getGlobalBounds().width / 2, this->winText->getGlobalBounds().height / 2));
	this->winText->setPosition(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
}

void WinScreen::Update(sf::RenderWindow *window) {
	//if the player presses enter after entering the win screen, this should return the player back to the main menu
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return) && enterKey) {
		coreState.SetState(new MainMenu());
	}

	enterKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return);
}

//Draw the win text to the win screen window
void WinScreen::Render(sf::RenderWindow *window) {
	window->draw(*this->winText);
}

//Destroy the text afterwards
void WinScreen::Destroy(sf::RenderWindow *window) {
	delete this->font;
	delete this->winText;
}

