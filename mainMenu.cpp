#include <iostream> //C++ library that allows us to print and take in input(cin/cout)
#include "mainMenu.h"
#include "mainGame.h" //to switch to this state if the player presses play
#include "map.h" //to load in the map level once the main game gets loaded in


//Initializes the objects and textures needed for the MainMenu
void MainMenu::Initialize(sf::RenderWindow *window) {
	//By default, make selected on play first
	this->selected = 0; //0 being play

	//BACKGROUND IMAGE
	this->backgroundTexture = new sf::Texture();
	this->backgroundTexture->loadFromFile("Graphics/backgrounds/titlebackground.jpg");
	this->background = new sf::Sprite();
	this->background->setTexture(*this->backgroundTexture);
	this->background->setScale(sf::Vector2f(3.2f, 3.2f)); //makes the image larger by scaling it

	//FONT
	this->font = new sf::Font(); //creates the font
	this->font->loadFromFile("Graphics/fonts/PandoraFont.otf"); //finds the location of the font

	//TITLE
	this->title = new sf::Text("Tiresome Exams", *this->font, 200U); //parameters include what the text says, the font it uses, and its size
	this->title->setOrigin(this->title->getGlobalBounds().width / 2, this->title->getGlobalBounds().height / 2); //sets location of where this text will be on the MainMenu screen, corners.
	this->title->setPosition(window->getSize().x / 2, window->getSize().y / 8); //x and y axis of the title text position
	this->title->setFillColor(sf::Color::Blue); //set the title color to be Blue

	//PLAY GAME
	this->play = new sf::Text("Play Game", *this->font, 120U);
	this->play->setOrigin(this->play->getGlobalBounds().width / 2, this->play->getGlobalBounds().height / 2);
	this->play->setPosition(window->getSize().x / 2, window->getSize().y / 2);

	//QUIT
	this->quit = new sf::Text("Quit", *this->font, 120U);
	this->quit->setOrigin(this->quit->getGlobalBounds().width / 2, this->quit->getGlobalBounds().height / 2);
	this->quit->setPosition(window->getSize().x / 2, window->getSize().y / 2 + this->play->getGlobalBounds().height + 40);

	//Background Music
	this->backgroundMusicBuffer = new sf::SoundBuffer();
	this->backgroundMusicBuffer->loadFromFile("Sounds/titlescreen.ogg"); //.ogg is waaay smaller than .wav, so it will load faster 
	this->backgroundMusic = new sf::Sound(*this->backgroundMusicBuffer);
	this->backgroundMusic->setLoop(true); //to make the song loop
	this->backgroundMusic->play(); //to play the song
}

//Constantly updates what is going on in the Main Menu
void MainMenu::Update(sf::RenderWindow *window) {
	//if the up key is being pressed now and it wasn't being pressed before, do something
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !this->upKey) {
		this->selected -= 1; 
	}
	//if the down key is being pressed now and it wasn't being pressed before, do something
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !this->downKey) {
		this->selected += 1;
	}

	
	//if the player keeps pressing upwards, that means that they are still on the play button
		//This essentially allows you to loop from top to bottom
	if (this->selected > 1) {
		this->selected = 0; //set to 0 so you go to play
	}
	//if the player keeps pressing downwards, that means they are still on the quit button
	if (this->selected < 0) {
		this->selected = 1; //set to 1 so you go to quit
	}

	//Check if the player chooses the selected option
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return)) {
		switch (this->selected) {
		case 0: //picks play
			coreState.SetState(new MainGame()); //set the state to MainGame(so enter the game once play is pressed)
			break;
		case 1: //picks quit
			quitGame = true; //quit out of the game
			break;
		}
	}

	//to figure out what the player's input is currently 
	this->upKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
	this->downKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
}

//Actually renders/draws these objects onto the window screen
void MainMenu::Render(sf::RenderWindow *window) {
	//Default colors of the text if not selected
	this->play->setFillColor(sf::Color::White); //setColor is deprecated, it is now setFillColor
	this->quit->setFillColor(sf::Color::White);
	//changes the play/quit text color if player is hovering over either one of them
	switch (this->selected) {
	case 0: //select play
		this->play->setFillColor(sf::Color::Yellow);
		break;
	case 1: //select quit
		this->quit->setFillColor(sf::Color::Yellow);
		break;
	}

	//draws out the text objects onto the MainMenu window
	window->draw(*this->background);
	window->draw(*this->title);
	window->draw(*this->play);
	window->draw(*this->quit);
}

//where we delete our main_menu objects
void MainMenu::Destroy(sf::RenderWindow *window) {
	//makes sure that the objects below gets deleted 
	delete this->font;
	delete this->title; 
	delete this->play;
	delete this->quit; 
	delete this->background;
	delete this->backgroundTexture;

	
	this->backgroundMusic->stop();
	delete this->backgroundMusic;
	delete this->backgroundMusicBuffer;
}


