#pragma once

#include <SFML/Audio.hpp>

#include "gameState.h"

//Main Menu is one of the game states
class MainMenu : public BaseState { //public part makes sure that this class has access to all of the public stuff of the parent class BaseState
public:
	//gets all of the functions from the BaseState and now the game state MainMenu can modify them
	void Initialize(sf::RenderWindow *window); 	//Creates a game state
	void Update(sf::RenderWindow *window); 	//Updates the game state
	void Render(sf::RenderWindow *window); 	//Renders the game state
	void Destroy(sf::RenderWindow *window);	//Destroys the game state
private:
	sf::Font *font; // to the MainMenu font
	//ref to the Texts of the MainMenu
	sf::Text *title;
	sf::Text *play;
	sf::Text *quit;

	//Variables for the background picture
	sf::Texture *backgroundTexture; //texture of the background 
	sf::Sprite *background;

	int selected; //variable needed to figure out whether the player is hovering over the play/quit buttons
	bool upKey, downKey; // sees if the player presses the upKey or the downKey to navigate through the MainMenu

	//Background Music
	sf::SoundBuffer *backgroundMusicBuffer;
	sf::Sound *backgroundMusic;
};