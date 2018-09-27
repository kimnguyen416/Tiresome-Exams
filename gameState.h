#pragma once //pretty much says this header will be created only once in every file it is included in

#include <SFML/Graphics.hpp> //needed to manipulate graphics
#include "saveSystem.h"

extern SaveSystem saveSystem;

//Manages all of the game states within the game. So the game state manager.
class BaseState {
public: //visible to everyone
		//These are all virtual since you want to be able to override all of these functions of the game state since they will be slightly diff in each state. 
	//Creates a game state
	virtual void Initialize(sf::RenderWindow *window) {

	}
	//Updates the game state
	virtual void Update(sf::RenderWindow *window) {

	}
	//Renders the game state
	virtual void Render(sf::RenderWindow *window) {

	}
	//Destroys the objects within the game state along with the game state itself
	virtual void Destroy(sf::RenderWindow *window) {

	}
};

//Game State class
class GameState {
public: //visible to everyone
	GameState() { 	//default constructor
		this->window = window; //since we're not sure what the state is when it is first initialized
	}

	//function to set the window
	void SetWindow(sf::RenderWindow *window) {
		this->window = window;
	}

	//function to set the game state
	void SetState(BaseState *state) { //using the BaseState as a basis, all game states will have the tiny state structure
		if (this->state != NULL) { //if current state isn't NULL(If there is a previous state)
			this->state->Destroy(this->window); //destroy the current state
		}

		saveSystem.Save(); //we want to save whenever we change the state 

		this->state = state; //then reset the current state 
		if (this->state != NULL) { //then initialize the state
			this->state->Initialize(this->window); //makes sure we initialize the window
		}
	}

	//This function will call the state's update method
	void Update() {
		if (this->state != NULL) { //if the state exists(so not NULL)
			this->state->Update(this->window); //call the state's update method
		}
	}

	//This function will draw out all of the objects into a game state
	void Render() {
		if (this->state != NULL) { //if the state exists(so not NULL)
			this->state->Render(this->window); //call the state's update method
		}
	}

	//When the game state is destroyed, we need to make sure that we destroy the state properly
	~GameState() {
		if (this->state != NULL) { //if current state isn't NULL(If there is a previous state)
			this->state->Destroy(this->window); //destroy the current state
		}
	}

private: //variables only visible to this specific class
	sf::RenderWindow *window; //ref to the screen window
	BaseState *state; //ref to the game state
};

//Global variables in relation to the game state
	//The core/main state that is currently being used
extern GameState coreState;
	//keeps track of when the player quits the game
extern bool quitGame;