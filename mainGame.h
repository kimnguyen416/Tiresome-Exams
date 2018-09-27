#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "gameState.h" //access to all of the game states. 
#include "souls.h" //to print the souls out onto the screen
#include "lives.h" //to show the lives the player has on the screen
#include "mana.h" //to show how much mana the player currently has
#include "stamina.h" //to show how much staminat he player currently has
#include "entityManager.h"
#include "map.h" //to load in the map based on which level the player is in
#include "dialogue.h" //to include dialogue within the game for specific NPCs
#include "saveSystem.h" //to constantly save things related to the player

//Global variable 
extern bool gameOver; 
extern SaveSystem saveSystem; //keeps track of where the player is and oon which map design they are currently in

//Main Game is one of the game states
class MainGame : public BaseState { //public part makes sure that this class has access to all of the public stuff of BaseState
public:
	//since we are inheriting from BaseState, we have access to the public functions & variables from BaseState
	void Initialize(sf::RenderWindow *window); 	//Creates a game state
	void Update(sf::RenderWindow *window); 	//Updates the game state
	void Render(sf::RenderWindow *window); 	//Renders the game state
	void Destroy(sf::RenderWindow *window);	//Destroys the game state
private:
	//Variables related to pausing the game
	sf::Font *font;
	sf::Text *pausedText;
	bool paused, pKey; //press p to pause the game

	Souls *souls; //the amount of souls the player has. 
	Lives *lives; //the amount of lives the player have
	Mana *manas; //the amount of mana the player has
	Stamina *staminas; //the amount of stamina the player has
		//Player stats UI
	sf::RectangleShape *hpBar; 
	sf::RectangleShape *mpBar;
	sf::RectangleShape *staminaBar; 

	//Dialogue
	Dialogue *dialogue; //ref to dialogue boxes and text within the game
	bool enterKey;

	EntityManager *manager; //ref to the EntityManager object

	Map *map; //get ref to the map

	//Background Music
	sf::SoundBuffer *backgroundMusicBuffer;
	sf::Sound *backgroundMusic;

	std::string backgroundLocation; //needed to change the background depending on what level the player is in
};