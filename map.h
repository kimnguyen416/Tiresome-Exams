#pragma once

#include <SFML/Graphics.hpp> 
#include <string>

#include "dialogue.h"
#include "entityManager.h"
#include "saveSystem.h"

//Checks for collisions at all angles when an entity hits the tile on the map design
typedef enum {
	NONE = -1,
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	TOP_LEFT = 4,
	TOP_RIGHT = 5,
	BOTTOM_LEFT = 6,
	BOTTOM_RIGHT = 7
}Direction;

//So you would go on tiled and give a property to the tile to the following # below to associate w/ whether it's water, ground, etc. 
typedef enum { //easier way to determine which groups are the ground tiles and etc
	NO_GROUP = 0,
	GROUND = 1,
	WATER = 2,
	LAVA = 3,
	LEAVE = 4, //property of a tile that serves as an entry point to the next area 
	WIN = 5,
	SAND = 6,
	TRAP = 7 //should work similar to the LAVA if the player lands on it. 
}TileGroup; //determines which group the tile is in. 

typedef struct {
	int id; //to get all of the ids of the tiles on Tiled
	TileGroup group; //group up the set of tiles that should be in the same group 
	bool wavy;
	std::string nextArea, nextAreaEntry; //to figure out what area that the player goes to next 
}TileProperties; //needed to determine if a specific tile needs to have a wavy animation

extern SaveSystem saveSystem; //needed to start saving the currentMap and such 

//Class for the game level maps 
class Map  { 
public:
	Map(EntityManager *entityManager);

	//Loads in the map design based on reading in the json file
	void Load(std::string filename, std::string backgroundFile, Dialogue *dialogue);

	//Moves the object based on the velocity give to it. We want to be able to get access to the window size and make more entities w/ the manager
	virtual void Update(sf::RenderWindow *window);

	//Checks for collision between entities/objects
	TileProperties CheckCollision(Entity *entity, Direction direction);

	void Render(sf::RenderWindow *window); //draw out the map

	void move(float x, float y);
	void move(sf::Vector2f amount);

	sf::FloatRect getGlobalBounds();

	//Ensures that the entity is properly deleted when it is destroyed
	~Map();
protected: //variables that cannot be accessed outside of this class unless other classes inherit it
	std::string tileSet; //the name of the png file with all of our block images
	 //the .map file also includes the measurements below, we need to record them when reading in that file
		//exact ordering of what is being read from the json file
	int width = 0;
	int height = 0;
	int tileWidth = 0;
	int tileHeight = 0;
	std::unordered_map<std::string, int*> data;  //takes in all of the tiled block locations
private:
	//these variables lets us know when the tilesets start within the json file. 
		//This also helps with figuring out how to animate the water tiles by flipping them constantly
	int groundTilesStart;
	int npcTilesStart;
	int mobTilesStart;
	int itemTileStart;

	sf::Texture *backgroundTexture; //texture of the background image
	std::unordered_map<int, TileProperties> tileProperties; //holds the tiles that will have the wavy tile property to them so that they can be animated wavy
	std::unordered_map<int, sf::Image*> tiles; //holds all of the tiles to be rendered into the game, w/ the pair int and image(int for which tile it is and image for the tile's image)
	std::unordered_map<int, sf::Image*> flippedTiles; //holds tiles that flip for an animated effect
	EntityManager *entityManager; //need a ref of all of the entities/NPCs on the map so they can be placed properly
	std::unordered_map<std::string, sf::Texture*> tileTextures; //reference to the texture. Actual graphics.
	std::unordered_map<std::string, sf::Sprite*> mapTiles; //holds the sprite of the tiles
	sf::Sprite *background; //sprite of the background scrolling image

	sf::Clock timer; //to make the flippedTiles animations go slower
};