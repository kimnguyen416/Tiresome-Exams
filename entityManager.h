#pragma once

#include <unordered_map> //basically a hash table/map
#include <vector> //vector is basically just an array list
#include "entity.h"


class EntityManager {
public:
	EntityManager(); //default constructor

	void Add(std::string name, Entity *entity); //function to add a new entity 
	bool Update(sf::RenderWindow *window); //to update all of the entities. 
	void Render(sf::RenderWindow *window, bool animate); //in order to be able to render things. Extra parameter animate so that we can stop the entities from animating when the game is paused. 
	Entity *Get(std::string name); //to get entities names
	~EntityManager(); //to clean up and destroy the entities when they are no longer used
private:
	//unordered_map is to allow us to keep track of all of the entities and assign them names
	std::unordered_map<std::string, Entity*> entities; //list of entities within the EntityManager
};