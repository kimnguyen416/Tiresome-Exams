#pragma once

#include <iostream>
#include <fstream>
#include <string>
#define SAVE_FILE "saveFile.sav"

//Class to save the position of the player and on which map design that they are currently on if the player enters the game the next time
	//It will output al of this data onto a text file so it remembers this info
class SaveSystem {
public:
	SaveSystem() { //it will first look for a file that exists of the save system and read in that data
		std::ifstream input(SAVE_FILE); //reads in this save file with that specific name
		std::string temp;

		if (input) { //if this file exists and we can read from it
			input >> x >> y >> souls >> flip >> health >> mana >> stamina >> currentAreaEntry >> currentMap; //reads the player's current position and what map they are in(in that exact order)
			
			visitedAreas.clear(); 
			while (input >> temp) {
				visitedAreas.push_back(temp);
			}
		}

		if (currentMap == "") { //currentMap is initially NULL, so when that happens set it to the townofvalencia.map
			this->Reset(); 
			this->Save(); //be sure to save this initial information
		}

		input.close(); //close the file 
	}

	void Reset() { //pretty much resets the save file back to the start of the game again once the player beats it. 
		currentMap = "townofvalencia.json";
		//in addition to the currentMap being NULL initially, the same goes for the player's x and y position so set a default position for now
		x = 0; //which is at the middle of the townofvalencia.map
		y = 1330;
		souls = 0; //by default
		health = 8; 
		mana = 5;
		stamina = 10; 
		currentAreaEntry = ""; //make it empty at first 
		flip = false;
	}

	void Save() { //creates an output of all of that information
		std::ofstream output(SAVE_FILE); //makes an updated version of the save file 
		output << x << " " << y << " " << souls << " " << flip << " " << health << " " << mana << " " << stamina << " " << currentAreaEntry << " " << currentMap; //the " " is to separate each of the parts that is being saved so it can be read more easily
		for (auto iterator = visitedAreas.begin(); iterator != visitedAreas.end(); iterator++) {
			output << " " << *iterator; 
		}
		output.close(); //then close that file
	}

	//Variables
	float x, y; //position of the player
		//Souls
	inline void IncrementSouls(int amount) { //the reason he did the if statement is to prevent you from getting the same points again, but a better solution is to keep track of the amount of enemies in the area and to prevent them from showing up but that's a lot of work considerign I have to do animations and other things
		this->souls += 1; //in the vid he had the same if statement as AddAreaToFinished, but I want the souls to increment every time you kill an enemy and to save it all the time, not when you finished the area. 
	}
	inline int GetSouls() {
		return this->souls;
	}
		//HP
	inline void IncrementHP(int amount) {
		this->health += 1; 
	}
	inline void DecrementHP(int amount) {
		this->health -= 1; 
	}
	inline int GetHP() {
		return this->health;
	}
		//MP
	inline void IncrementMana(int amount) {
		this->mana += 1;
	}
	inline void DecrementMana(int amount) {
		this->mana -= 1;
	}
	inline int GetMP() {
		return this->mana;
	}
		//Stamina
	inline void IncrementStamina(int amount) {
		this->stamina += 1;
	}
	inline void DecrementStamina(int amount) {
		this->stamina -= 1;
	}
	inline int GetStamina() {
		return this->stamina;
	}

	inline void AddAreaToFinished(){
		if (std::find(visitedAreas.begin(), visitedAreas.end(), this->currentMap) == visitedAreas.end()) {
			visitedAreas.push_back(this->currentMap);
		}
	}

	std::string currentAreaEntry; //keeps track of which level entry the player is in. 
	std::string currentMap; //on which map is the player in
	bool flip; //to remember what direction the player was facing. 
	int health; //to remember how much health the player has 
	int mana; //to remember how much mana the player has 
	int stamina; //to remember how much stamina the player has 
private:
	int souls;  //save the amoount of souls you have and it's essentially your score on the exam 
	std::vector<std::string> visitedAreas; 
};