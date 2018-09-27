#pragma once

#include "entity.h"
#include "map.h"

extern SaveSystem saveSystem; //update the player's position 

//PLAYER CONTROL IDEA: A and D to move left and right. W and S to move up and down when in the water,
//Space bar to jump, left shit to sprint, I for regular attacks, O for special attack, and P to pause the game
//enterKey is for interaction, such as objects or initiating dialogue, If we add a guard move then prob hold U or something. 

class Player : public Entity { //make it public so that it can freely reference the entity parent class
public:
	//Player stats
	int health; 
	int mana;
	int stamina;

	Player(sf::RenderWindow *window, EntityManager *entityManager, Map *map, float x, float y); //default constructor, x and y to figure out where the player's starting position will be on each map/level
	void Swim(sf::RenderWindow *window); //helps clean up the player's update method more. 
	void Ground(sf::RenderWindow *window); //helps clean up the player's update method more. 
	bool Update(sf::RenderWindow *window);
	void Collision(Entity *entity);
	~Player(); 
private:
	Map *map; //player needs a ref to the map level to collide with the tiles properly & to see where they are on the map
	float speed;
	EntityManager *entityManager; 
	bool enterKey, spaceKey, iKey, oKey, uKey; //to tell when the player wants to interact w/ something or jump or I for normal attacks(which are essentially the offensive bullet), oKey is for the special move, U for guarding
	int jump, sprint, attack; //(allows us to have varying lengths in our jump and run) so you can build up your jump and sprint
	float direction; //direction that the invisible interaction bullet will go at. Needed to talk to other people. 
	float damageKnockBack; //helps us create the knockback mechanic whenever the player takes damage. 

	//Sound Effects for the Player
	sf::SoundBuffer *jumpBuffer;
	sf::Sound *jumpSound;
	sf::SoundBuffer *shootBuffer;
	sf::Sound *shootSound;
	sf::SoundBuffer *specialMoveBuffer;
	sf::Sound *specialMoveSound;
	sf::SoundBuffer *guardBuffer;
	sf::Sound *guardSound; 

	//A timer to delay some things
	sf::Clock staminaTimer; 
};