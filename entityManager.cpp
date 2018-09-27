#include "entityManager.h"

//default constructor
EntityManager::EntityManager() {
	
}

//Adding a new entity within the manager
void EntityManager::Add(std::string name, Entity *entity) {
	//Looks for the name of the entity first
	std::unordered_map<std::string, Entity*>::const_iterator found = this->entities.find(name);
	//while you have not found the name yet and it is not the end of the list. This ensures that we have a unique name even if there is the same name within the list
	while (found != this->entities.end()) {
		//so we found the name
		name += "0"; //so we add on a 0 to indicate that we found the name
		found = this->entities.find(name); //make found = to the name found within the list
	}
	//once we reach the end of the list. once it has a unique name it does the following. 
	this->entities.insert(std::make_pair(name, entity)); //we give this specific entity a name and it's entity pair

}

//Get the name of the entity
Entity * EntityManager::Get(std::string name) {
	//look for the entity that we are searching for name
	std::unordered_map<std::string, Entity*>::const_iterator found = this->entities.find(name);
	if (found == this->entities.end()) { //if we reached the end of the list and have still not found the entity
		return NULL; 
	}
	else { //otherwise return the entity since it was found
		return found->second;
	}
}



bool EntityManager::Update(sf::RenderWindow *window) {
	std::vector<std::string> toRemove; //contains a list of entities that we want to remove by name

	//go through the entities 
	for (auto &iterator: this->entities) {
		if (iterator.second->GroupID() > 2 || iterator.second->GroupID() == 1) { //GROUPIDS MIGHT BE CHANGED!!!!(I dont remember why it's 1 or > 2 for the groupID)
			for (auto &iterator2 : this->entities) {
				//this if statement checks if the entities are diff through their names first
				if (iterator.first != iterator2.first) {
					//Need to check if both of the entities are both active as well
					if (iterator.first != iterator2.first) { //check by string names
						//checks for collisions
						if (iterator.second->CheckCollision(iterator2.second)) {
							//Collision between entities
							iterator.second->Collision(iterator2.second);
						}
					}
				}
			}
		}


		//switch case for looking at the active state of the entity(so the case if an entity's active is 0 or 1)
		switch (iterator.second->Active()) {
		case 0: //active state is 0
			//add this entity to the remove vector by it's string name
			toRemove.push_back(iterator.first); //first parameter being the entity's name
			break;
		default: //any other active state will be on this default case and will just update like normal	 
			//second = second argument which is in this case the pointer of entity* and update it
			if (!iterator.second->Update(window)) {
				return false; 
			}
			break;
		}
	}

	//for loop to actually get rid of the entities that are in the toRemove vector permanently
	for (auto &iterator : toRemove) {
		//look for the entity that we are searching for name
		std::unordered_map<std::string, Entity*>::const_iterator found = this->entities.find(iterator);
		if (found != this->entities.end()) { //if we have not reached the end of the llist
			delete found->second; //delete each one of the entries within the lsit
			this->entities.erase(iterator);
		}
	}
	toRemove.clear(); //then clear the vector to empty out the vector list for now

	return true; 
}


void EntityManager::Render(sf::RenderWindow *window, bool animate) {
	for (auto &iterator : this->entities) {
		if (animate) {
			iterator.second->Render(window); //renders the entities animations
		}
		
		//just to draw out the entity within the entities list. 
		window->draw(*iterator.second);
	}
}



EntityManager::~EntityManager() {
	//properly delete each entity
	for (auto &iterator : this->entities) {
		delete iterator.second; 
	}

	this->entities.clear(); //clear out the entities list
}