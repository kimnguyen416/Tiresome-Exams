#pragma once

#include "gameState.h"

//Game over is another game state when the player loses all of their health/lives.
class GameOver : public BaseState {
public:
	void Initialize(sf::RenderWindow *window);
	void Update(sf::RenderWindow *window);
	void Render(sf::RenderWindow *window);
	void Destroy(sf::RenderWindow *window);
private:
	sf::Font *font; 
	sf::Text *gameOverText;
	bool enterKey;
};