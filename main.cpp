#include <Windows.h>
#include "gameState.h"
#include "mainMenu.h"

//Global variables
GameState coreState; 
bool quitGame = false; //initially set to false since the player hasn't quit the game yet

//Pretty much loads up the window screen of the game and manages all of the events that occur within the game
int main(){
	sf::RenderWindow window(sf::VideoMode(1280, 768), "Tiresome Exams"); //sets the window size and name of the window

	coreState.SetWindow(&window); //set the current state's window
	coreState.SetState(new MainMenu()); //set the state to the Main Menu at the start when the game application is open

	//run the program as long as the window is open
	while (window.isOpen()) {
		//check all of the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event)) {
			//"close requested" event: we close the window
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::Black); //makes the window screen black
		
		coreState.Update(); //constantly update the game state
		coreState.Render(); //make sure to render the game as well to show the graphics

		window.display(); 

		//check if player wants to quit the game
		if (quitGame) {
			window.close();
		}
		
		Sleep(1.0f);  //makes the game run a bit slower
	}

	return 0;
}