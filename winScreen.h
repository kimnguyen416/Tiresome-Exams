#pragma once

#include "gameState.h"

//Win screen is a new game state if the player beats the game
class WinScreen : public BaseState {
public:
	void Initialize(sf::RenderWindow *window);
	void Update(sf::RenderWindow *window);
	void Render(sf::RenderWindow *window);
	void Destroy(sf::RenderWindow *window);
private:
	sf::Font *font; 
	sf::Text *winText;
	bool enterKey;
};