#include "mainGame.h"
#include "mainMenu.h" //to go back to the mainMenu once the player pauses and exits
#include "player.h" //related to the player
#include "gameOver.h" //once the player loses all of their health/lives, it should go to the game over screen
#include "winScreen.h" //once the player completes the game, they should have a win screen
#include "baseNPC.h" //have NPCs in the main game state
#include "enemy.h" //have enemies in the main game state


//Global Variables
bool gameOver = false; //initially set it to false
SaveSystem saveSystem; //will update the player's position and current location


//Creates the objects for the MainGame
void MainGame::Initialize(sf::RenderWindow *window) {
	//Font
	this->font = new sf::Font();
	this->font->loadFromFile("Graphics/fonts/EuropeanTypewriter.ttf");
	
	//HP/Lives
	this->hpBar = new sf::RectangleShape();
	this->hpBar->setFillColor(sf::Color::Red);
	this->hpBar->setSize(sf::Vector2f((float)saveSystem.GetHP() * 20.0f, 30.0f));
	this->hpBar->setPosition(0, 20);
	//Initiate the life here but don't draw it out
	this->lives = new Lives(*font, 64U);
	this->lives->setFillColor(sf::Color::Red);
	
	//Mana
	this->mpBar = new sf::RectangleShape();
	this->mpBar->setFillColor(sf::Color::Blue);
	this->mpBar->setSize(sf::Vector2f((float)saveSystem.GetMP() * 20.0f, 30.0f));
	this->mpBar->setPosition(0, 52);
	this->manas = new Mana(*font, 64U);
	this->manas->setFillColor(sf::Color::Blue);

	//Stamina
	this->staminaBar = new sf::RectangleShape();
	this->staminaBar->setFillColor(sf::Color::Yellow);
	this->staminaBar->setSize(sf::Vector2f((float)saveSystem.GetStamina() * 20.0f, 30.0f));
	this->staminaBar->setPosition(0, 85);
	this->staminas = new Stamina(*font, 64U);
	this->staminas->setFillColor(sf::Color::Yellow);

	//Dialogue
	this->dialogue = new Dialogue(*font, 32U, window);

	//SOULS
	this->souls = new Souls(*font, 55U);
	this->souls->setPosition(window->getSize().x - this->souls->getGlobalBounds().width - 40, 0);  //make the Souls text to be to the far right side
	this->souls->setFillColor(sf::Color::Cyan);
																							  //Pause Text
	this->pausedText = new sf::Text("         Paused\nPress Escape to Quit", *font, 64U);
	this->pausedText->setOrigin(this->pausedText->getGlobalBounds().width / 2, this->pausedText->getGlobalBounds().height / 2);
	this->pausedText->setPosition(window->getSize().x / 2, window->getSize().y / 2);
	this->pausedText->setFillColor(sf::Color::Black);

	//Initialize the pause variables
	this->paused = false;
	this->pKey = true; //set to true so that the game is not automatically paused when you 1st enter the game
	this->enterKey = true; //set to true so it doesn't set the dialogue right away. 

	//Set up the entity manager
	manager = new EntityManager();

	//Set up the map level
	map = new Map(manager);
		//Cheap method to change the background image depending on what level the player is in. 
	if (saveSystem.currentMap == "townofvalencia.json") {
		backgroundLocation = "Graphics/backgrounds/JnRLayer01.png";
	}
	else if (saveSystem.currentMap == "tutorial.json") {
		backgroundLocation = "Graphics/backgrounds/JnRLayer01.png"; //doesn't really matter since you shouldn't see anything anyways for background
	}
	else if (saveSystem.currentMap == "forest1.json") {
		backgroundLocation = "Graphics/backgrounds/BG.png";
	}
	else if (saveSystem.currentMap == "forest2.json") {
		backgroundLocation = "Graphics/backgrounds/ice.png";
	}
	else if (saveSystem.currentMap == "desert1.json") {
		backgroundLocation = "Graphics/backgrounds/sunset.png";
	}
	else if (saveSystem.currentMap == "desert2.json") {
		backgroundLocation = "Graphics/backgrounds/sunset.png";
	}
	else if (saveSystem.currentMap == "volcano1.json") {
		backgroundLocation = "Graphics/backgrounds/bg_volcano.png";
	}
	else if (saveSystem.currentMap == "volcano2.json") {
		backgroundLocation = "Graphics/backgrounds/ice.png";
	}
	map->Load(saveSystem.currentMap, backgroundLocation, this->dialogue); //read in the map file


	//Initializes the player to the map
	this->manager->Add("player", new Player(window, manager, map, saveSystem.x, saveSystem.y));

	//Background Music
	this->backgroundMusicBuffer = new sf::SoundBuffer();  //or maybe change the sound here with if statements and the save system.currentMap == "townofvalencia.json"
	if (saveSystem.currentMap == "townofvalencia.json") {
		this->backgroundMusicBuffer->loadFromFile("Sounds/townofvalencia.ogg");
	}
	else if (saveSystem.currentMap == "tutorial.json") {
		this->backgroundMusicBuffer->loadFromFile("Sounds/temple.ogg");
	}
	else if (saveSystem.currentMap == "forest1.json") {
		this->backgroundMusicBuffer->loadFromFile("Sounds/forestsong.ogg");
	}
	else if (saveSystem.currentMap == "forest2.json") {
		this->backgroundMusicBuffer->loadFromFile("Sounds/megabossloop.ogg"); 
	}
	else if (saveSystem.currentMap == "desert1.json") {
		this->backgroundMusicBuffer->loadFromFile("Sounds/desertloop.ogg");
	}
	else if (saveSystem.currentMap == "desert2.json") {
		this->backgroundMusicBuffer->loadFromFile("Sounds/myenemyloop.ogg");
	}
	else if (saveSystem.currentMap == "volcano1.json") {
		this->backgroundMusicBuffer->loadFromFile("Sounds/eerie.ogg"); 
	}
	else if (saveSystem.currentMap == "volcano2.json") {
		this->backgroundMusicBuffer->loadFromFile("Sounds/wrathbattleloop.ogg"); 
	}

	this->backgroundMusic = new sf::Sound(*this->backgroundMusicBuffer);
	this->backgroundMusic->setLoop(true); //to make the song loop
	this->backgroundMusic->play(); //to play the song
}

//Constantly updates what is going on in the Main Game
void MainGame::Update(sf::RenderWindow *window) {
	//update the hpBar, manaBar, & staminaBar
	this->hpBar->setSize(sf::Vector2f((float)saveSystem.GetHP() * 20.0f, 30.0f)); //put the HP bar here to constantly update the size of the health bar if the player takes dmg and such
	this->mpBar->setSize(sf::Vector2f((float)saveSystem.GetMP() * 20.0f, 30.0f));
	this->staminaBar->setSize(sf::Vector2f((float)saveSystem.GetStamina() * 20.0f, 30.0f));

	//Properly set the values of souls, lives, manas, and staminas
	this->lives->SetValue(((Player*)this->manager->Get("player"))->health);//set the life of the player
	this->manas->SetValue(((Player*)this->manager->Get("player"))->mana);
	this->staminas->SetValue(((Player*)this->manager->Get("player"))->stamina);
	this->souls->SetValue(saveSystem.GetSouls()); //just constantly get the latest scores based on the save system 

	//While the game is paused
	if (this->paused) {
		//If the player presses enter again, the screen would unpause.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && !this->pKey) {
			this->paused = false;
		}
		//if the player presses the escape key during any time in the Main game while paused, go back to MainMenu
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			//saves the player's positioning ++ the game's scroll mechanic
			saveSystem.x = this->manager->Get("player")->getPosition().x + Entity::scroll.x;
			saveSystem.y = this->manager->Get("player")->getPosition().y + Entity::scroll.y;
			saveSystem.flip = this->manager->Get("player")->flippedH;
			saveSystem.Save(); //when the player leaves the game, the system should save what's needed

			coreState.SetState(new MainMenu()); //set the state to MainMenu
			return;
		}
	}
	else if (this->dialogue->talking) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return) && !this->enterKey) {
			this->dialogue->talking = false;
		}
	}
	//if the game is not paused
	else {
		if (!this->manager->Update(window)) { //update EntityManager if game is not paused
			return;
		}

		//Update the Souls when game is unpaused
		this->souls->Update();
		//Make sure that the lives are properly updated when the game is running. 
		this->lives->Update();

		//But if the player presses P during any time of gameplay, the game is paused
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && !this->pKey) {
			this->paused = true;
		}
	}

	//check if there is a gameOver(if the player gets hit by the enemy or if the player's life is 0)
	if (gameOver || this->lives->GetLife() <= 0) {
		gameOver = false; 
		//Game Over, make this the new game state and switch to that window screen
		coreState.SetState(new GameOver());
		return;
	}


	//Checks if the player presses the enterKey to try and interact with someone
	this->enterKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return);
	//Put the keypress down below to constantly keep track if the player presses the P key to pause
	this->pKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
	
	if (!this->paused) { //if not paused, then update the maps. Do not update map if the game is paused. 
		this->map->Update(window);
	}
}

//Actually renders/draws these objects onto the window screen
void MainGame::Render(sf::RenderWindow *window) {
	map->Render(window);
	this->manager->Render(window, !this->paused); //animate the animations only when the game is unpaused. 
	window->draw(*this->souls);
	window->draw(*this->hpBar);
	window->draw(*this->mpBar);
	window->draw(*this->staminaBar);
	//window->draw(*this->lives); //dont draw out the text but draw out the hpBar
	this->dialogue->Render();
	
	//If the game is paused, draw the pausedText
	if (this->paused) {
		window->draw(*this->pausedText);
	}
}

//Where we delete our MainGame objects
void MainGame::Destroy(sf::RenderWindow *window) {
	delete this->souls;
	delete this->lives;
	delete this->hpBar;
	delete this->manas;
	delete this->mpBar;
	delete this->staminas;
	delete this->staminaBar;
	delete this->font;
	delete this->pausedText;
	delete this->dialogue;
	delete this->manager; 
	delete this->map;

	this->backgroundMusic->stop();
	delete this->backgroundMusic;
	delete this->backgroundMusicBuffer;
}

