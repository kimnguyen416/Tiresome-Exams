#include <iostream>
#include <fstream> //to read in files, since we need to read the blocks from the json files to properly render them
#include <sstream>

#include "map.h"
#include "enemy.h"
//ENEMIES
#include "wolf.h"
#include "basilBoss.h"
#include "belphegorBoss.h"
#include "bianchiBoss.h"
#include "dinoBoss.h"
#include "skeleton.h"
#include "snake.h"
#include "tsunaNorm.h"
#include "tsunaAngry.h"
#include "viperMob.h"
//NPCs
#include "basil.h"
#include "bianchi.h"
#include "dino.h"
#include "hughes.h"
#include "tsuna.h"
#include "belphegor.h"
#include "viper.h"
//Items
#include "item.h"
#include "hpPotion.h"
#include "mpPotion.h"

//3rd Party Tool: Rapid JSON, to parse the json file
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;  //just a shortcut to use this name instead of typing everything out 

//when an entity is created it will have the following
Map::Map(EntityManager *entityManager) { //entity base class 
	this->tiles = std::unordered_map<int, sf::Image*>();
	this->entityManager = entityManager;
	this->backgroundTexture = new sf::Texture();
	this->background = new sf::Sprite();
}

//Read in all of the tile blocks in our map design made with Tiled
void Map::Load(std::string filename, std::string backgroundFile, Dialogue *dialogue) {
	this->backgroundTexture->loadFromFile(backgroundFile);//load in the image 
	this->backgroundTexture->setRepeated(true);
	this->background->setTexture(*this->backgroundTexture);
	if (saveSystem.currentMap == "forest1.json") { //scale by this much for the forest scene
		this->background->scale(3.5f, 2.0f);
	}
	else if (saveSystem.currentMap == "forest2.json") {
		this->background->scale(3.5f, 4.0f);
	}
	else if (saveSystem.currentMap == "desert1.json") {
		this->background->scale(5.0f, 2.5f);
	}
	else if (saveSystem.currentMap == "desert2.json") {
		this->background->scale(3.5f, 4.5f);
	}
	else if (saveSystem.currentMap == "volcano1.json") {
		this->background->scale(3.0f, 2.5f);
	}
	else if (saveSystem.currentMap == "volcano2.json") {
		this->background->scale(3.5f, 4.5f);
	}
	else {
		this->background->scale(10.0f, 10.0f); //should just be like this for the first area & tutorial(doesn't matter)
	}

	//Set up to read in the map's json file w/ a 3rd party tool called JSON parser
	std::ifstream mapFile("Graphics/mapjsons/" + filename); //reads in the file
	std::string mapFileData((std::istreambuf_iterator<char>(mapFile)), //iterates through every char in the json file
		std::istreambuf_iterator<char>());
	Document mapFileDoc;
	mapFileDoc.Parse(mapFileData.c_str());

	//Reads in some of the properties of the json file to get height,tileHeight, etc
	this->width = mapFileDoc["width"].GetInt();
	this->height = mapFileDoc["height"].GetInt();
	this->tileWidth = mapFileDoc["tilewidth"].GetInt();
	this->tileHeight = mapFileDoc["tileheight"].GetInt();

	//std::cout << this->tileWidth << ", " << this->tileHeight << std::endl;

	Value &dataArray = mapFileDoc["layers"];
	if (dataArray.IsArray()) {
		for (int i = 0; i < dataArray.Capacity(); i += 1) {
			Value &data = dataArray[i]["data"];

			//reads in the data(so where all of the blocks are placed)
			int *tempData = new int[this->width * this->height];
			for (int y = 0; y < this->height; y += 1) {
				for (int x = 0; x < this->width; x += 1) {
					if (x + y * this->width < data.Capacity()) {
						if (data[x + y * this->width].IsInt()) {
							tempData[x + y * this->width] = data[x + y * this->width].GetInt();
						}
					}
					//std::cout << data[x + y * this->width].GetInt() << std::endl; ////Double checks if it is reading the data portion of the json file correctly
				}
			}
			std::pair<std::string, int*> tempPair(dataArray[i]["name"].GetString(), tempData);
			this->data.insert(tempPair);
		}
	}

	
	int groundTileSet = -1; 
	int npcTileSet = -1;
	int mobsTileSet = -1; 
	int itemTileSet = -1; 

	//get the tiles from our png
	Value &tileList = mapFileDoc["tilesets"]; // gets all of the tilesets within Tiled(since we have NPCs, alltiles, ...)
	//Goes through all of the tilesets available 
	if (tileList.IsArray()) {
		for (int i = 0; i < tileList.Capacity(); i += 1) {
			std::string name = mapFileDoc["tilesets"][i]["name"].GetString(); //determines which tileset we are dealing with(NPC, Ground(all of the tiles), ..)

			if (name == "Ground") { //these are the exact names of the tilesets on Tiled
				groundTileSet = i;
			}
			if (name == "NPC") {
				npcTileSet = i; 
			}
			if (name == "Mobs") {
				mobsTileSet = i; 
			}
			if (name == "Items") {
				itemTileSet = i; 
			}

			if (i == groundTileSet) { //only load in the ground tiles once
				Value &tileData = mapFileDoc["tilesets"][i]["tiles"]; //the tile images we load
				int firstgid = mapFileDoc["tilesets"][i]["firstgid"].GetInt();
				for (Value::ConstMemberIterator iterator = tileData.MemberBegin(); iterator != tileData.MemberEnd(); iterator += 1) {
					//Stuff below is all needed to get the name of the image of the tiles and the image itself and put it all in a hash table for future ref
					sf::Image *temp = new sf::Image();
					temp->loadFromFile("Graphics/sprites/" + std::string(iterator->value["image"].GetString())); //the tiles can be found under the "Graphics/sprites/tiles" directory

					std::string name = iterator->name.GetString();
					std::pair<int, sf::Image*> tempPair(std::stoi(name) + 0 + firstgid, temp);
					this->tiles.insert(tempPair);

					//Prep work needed to get the group of tiles that need the wavy property for some animations to it(default properties set to other tiles if they do not have the properties below)
					TileProperties tempProperty = { std::stoi(name) + 0 + firstgid, NO_GROUP, false, "", "" }; //set the wavy property as false initially and get the id of the tile 
					if (mapFileDoc["tilesets"][i]["tileproperties"].HasMember(name.c_str())) {
						//If statements below is to get the tile properties of each tiles that we added
						if (mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()].HasMember("wavy")) { //if the tile has the property wavy, set it to true
							tempProperty.wavy = true; 
						}

						if (mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()].HasMember("nextArea") &&
							mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()].HasMember("nextAreaEntry")) { //if the tile has the nextArea property & nextAreaEntry property
							tempProperty.nextArea = mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()]["nextArea"].GetString(); //get the nextArea string
							tempProperty.nextAreaEntry = mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()]["nextAreaEntry"].GetString(); //get the nextAreaEntry string
						}

						if (mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()].HasMember("group")) { //looks at the group property of the tile
							std::string stringValue = mapFileDoc["tilesets"][i]["tileproperties"][name.c_str()]["group"].GetString(); //get the group id by string
							int intValue = std::stoi(stringValue); //convert the string to int
							tempProperty.group = (TileGroup)intValue; //set the group id of the tile to the converted intValue of the json file 
						}


					}
					std::pair<int, TileProperties> tempPairProperty(std::stoi(name) + 0 + firstgid, tempProperty);
					this->tileProperties.insert(tempPairProperty);

					//If the tile has the property of wavy
					if (tempProperty.wavy) { //add the water tiles to the flipped tiles hash map
						sf::Image *temp2 = new sf::Image();
						temp2->loadFromFile("Graphics/sprites/" + std::string(iterator->value["image"].GetString()));
						temp2->flipHorizontally();
						std::pair<int, sf::Image*> tempPair2(std::stoi(name) + 0 + firstgid, temp2);
						this->flippedTiles.insert(tempPair2);
					}
				}
			}
		}
	}
	

	//Pretty much gets the start of all of the tile sets and each variable holds a specific set 
	this->groundTilesStart = mapFileDoc["tilesets"][groundTileSet]["firstgid"].GetInt();
	if (npcTileSet > -1) {
		this->npcTilesStart = mapFileDoc["tilesets"][npcTileSet]["firstgid"].GetInt();
	}
	if (mobsTileSet > -1) {
		this->mobTilesStart = mapFileDoc["tilesets"][mobsTileSet]["firstgid"].GetInt();
	}
	if (itemTileSet > -1) {
		this->itemTileStart = mapFileDoc["tilesets"][itemTileSet]["firstgid"].GetInt();
	}


	for (auto iterator = this->data.begin(); iterator != this->data.end(); iterator++) {
		int *layerData = iterator->second;
		sf::Texture *tempTexture = new sf::Texture(); 
		//get the tiles from our png
		tempTexture->create(this->width * this->tileWidth, this->height * this->tileHeight); //changes to this will determine how far the scroll can look down or up, to the right/left
		sf::Sprite *tempSprite = new sf::Sprite();
		tempSprite->setTexture(*tempTexture);

		std::pair<std::string, sf::Texture*> tempPairTexture(iterator->first, tempTexture);
		this->tileTextures.insert(tempPairTexture);
		std::pair<std::string, sf::Sprite*> tempPairSprite(iterator->first, tempSprite);
		this->mapTiles.insert(tempPairSprite);

		//load in all of the tiles, so draws them all out onto the screen
		for (int y = 0; y < this->height; y += 1) {
			for (int x = 0; x < this->width; x += 1) {
				//specifically for the ground tiles
				if (layerData[x + y * this->width] > 0 && layerData[x + y * this->width] < mapFileDoc["tilesets"][npcTileSet]["firstgid"].GetInt()
					&& layerData[x + y * this->width] < mapFileDoc["tilesets"][mobsTileSet]["firstgid"].GetInt()) {
					//just updates all of the tiles
					tempTexture->update(*this->tiles[layerData[x + y * this->width]], x * this->tileWidth, y * this->tileHeight);

					if (saveSystem.currentAreaEntry != "" && this->tileProperties[layerData[x + y * this->width]].nextAreaEntry != "") {
						//I added this but not sure what it completely does just yet. Continue tmrow....
						if (saveSystem.currentAreaEntry == this->tileProperties[layerData[x + y * this->width]].nextAreaEntry) {
							saveSystem.x = x * this->tileWidth;
							saveSystem.y = y * this->tileHeight - this->tileHeight / 2;
						}
					}

				}
				//Loads in all of the NPC tile sets
				else if (layerData[x + y * this->width] >= mapFileDoc["tilesets"][npcTileSet]["firstgid"].GetInt() && layerData[x + y * this->width] < mapFileDoc["tilesets"][mobsTileSet]["firstgid"].GetInt()) {
					Value &tileSet = mapFileDoc["tilesets"][npcTileSet];
					int tile = layerData[x + y * this->width] - tileSet["firstgid"].GetInt();

					BaseNPC *temp = new BaseNPC(dialogue, tileSet["tiles"][std::to_string(tile).c_str()]["image"].GetString(), this,
						tileSet["tileproperties"][std::to_string(tile).c_str()]["text"].GetString(), x * this->tileWidth, y * this->tileHeight);
					this->entityManager->Add("npc", temp);
					temp->offset(-(temp->getGlobalBounds().width - (float)this->tileWidth), -(temp->getGlobalBounds().height - (float)this->tileHeight));
					layerData[x + y * this->width] = 0;
				}
				//loads in the enemy tile sets 
				else if (layerData[x + y * this->width] >= mapFileDoc["tilesets"][mobsTileSet]["firstgid"].GetInt() && layerData[x + y * this->width] < mapFileDoc["tilesets"][itemTileSet]["firstgid"].GetInt()) {
					Value &tileSet = mapFileDoc["tilesets"][mobsTileSet];
					int tile = layerData[x + y * this->width] - tileSet["firstgid"].GetInt();

					Enemy *temp = NULL;
					switch (tile) { //a switch to identify which id the enemy tileset has the following mob/boss to load in
					case 1:
						temp = new TsunaNorm(this, x * this->tileWidth, y * this->tileHeight);
						break;
					case 2:
						temp = new TsunaAngry(this, x * this->tileWidth, y * this->tileHeight);
						break;
					case 3:
						temp = new DinoBoss(this, x * this->tileWidth, y * this->tileHeight);
						break;
					case 4:
						temp = new Snake(this, x * this->tileWidth, y * this->tileHeight);
						break;
					case 5:
						temp = new BasilBoss(this, x * this->tileWidth, y * this->tileHeight);
						break;
					case 6:
						temp = new BianchiBoss(this, x * this->tileWidth, y * this->tileHeight);
						break;
					case 7:
						temp = new Skeleton(this, x * this->tileWidth, y * this->tileHeight);
						break;
					case 8:
						temp = new ViperMob(this, x * this->tileWidth, y * this->tileHeight);
						break;
					case 9:
						temp = new BelphegorBoss(this, x * this->tileWidth, y * this->tileHeight);
						break;
					default:
						temp = new WolfMob(this, x * this->tileWidth, y * this->tileHeight);
						break;
					}
					this->entityManager->Add("mobs", temp);
					temp->offset(-(temp->getGlobalBounds().width - (float)this->tileWidth), -(temp->getGlobalBounds().height - (float)this->tileHeight));
					layerData[x + y * this->width] = 0;
				}
				//Loads in items that can be picked up 
				else if (layerData[x + y * this->width] >= mapFileDoc["tilesets"][itemTileSet]["firstgid"].GetInt()) {
					Value &tileSet = mapFileDoc["tilesets"][itemTileSet];
					int tile = layerData[x + y * this->width] - tileSet["firstgid"].GetInt();

					Item *temp = NULL;
					switch (tile) {
					case 1:
						temp = new MPPotion(this, x * this->tileWidth, y * this->tileHeight);
						break;
					default:
						temp = new HPPotion(this, x * this->tileWidth, y * this->tileHeight);
						break;
					}
					this->entityManager->Add("item", temp);
					temp->offset(-(temp->getGlobalBounds().width - (float)this->tileWidth), -(temp->getGlobalBounds().height - (float)this->tileHeight));
					layerData[x + y * this->width] = 0;
				}
			}
		}

	}

	mapFile.close(); //close the map file after it is done being read
}


sf::FloatRect Map::getGlobalBounds() {
	return this->mapTiles["Ground"]->getGlobalBounds();
}

void Map::move(float x, float y) { //moves the sprite
	for (auto iterator = this->mapTiles.begin(); iterator != mapTiles.end(); iterator++) {
		iterator->second->move(x, y);
	}

	this->background->move(x * 0.5f, y * 0.5f); //background image will only move 75% of the speed of the tiles
}

void Map::move(sf::Vector2f amount) { //this moves the sprite by a certain amount
	this->move(amount.x, amount.y);
}

bool flip = false;

//moves the object based on the velocity give to it. We want to be able to get access to the window size and make more entities w/ the manager
void Map::Update(sf::RenderWindow *window) { //virtual allows you to override a function
		//Note: the reason this is here in the update method as well is because we want to try and animate the water constantly
	 //load in all of the tiles, so draws them all out onto the screen
	sf::Time elapsed = this->timer.getElapsedTime();
	if (elapsed.asMilliseconds() > 16 * 15) { //slows down the water animations 
		
		int *layerData = this->data["Ground"];

		for (int y = 0; y < this->height; y += 1) {
			for (int x = 0; x < this->width; x += 1) {
				int tile = layerData[x + y * this->width];
				if (tile > 0) {
					if (this->tileProperties[tile].wavy) { //if the tile has the wavy property
						if (flip) {
							//just updates all of the tiles
							this->tileTextures["Ground"]->update(*this->flippedTiles[tile], x * this->tileWidth, y * this->tileHeight);
						}
						else {
							//just updates all of the tiles
							this->tileTextures["Ground"]->update(*this->tiles[tile], x * this->tileWidth, y * this->tileHeight);
						}
					}
				}
			}
			if (y < this->height - 1) {
				flip = !flip;
			}
		}
		timer.restart();
	}
}

void Map::Render(sf::RenderWindow *window) {
	window->draw(*this->background); //background should get drawn first so that the tiles will be drawn on top of it
	
	for (auto iterator = this->mapTiles.begin(); iterator != this->mapTiles.end(); iterator++) {
		window->draw(*iterator->second);
	}
}


//checks for collision when bumping into the tiles in all directions
TileProperties Map::CheckCollision(Entity *entity, Direction direction) {
	entity->move(Entity::scroll); //this is going to move the entity by the scroll position

	//switch based on the direction the entity is coming towards the tile
	int x = (int)(entity->getPosition().x + entity->getGlobalBounds().width / 2) / this->tileWidth;
	int y = (int)(entity->getPosition().y + entity->getGlobalBounds().height / 2) / this->tileHeight;
	switch (direction) {
	case LEFT:
		//x and y position
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height / 2) / this->tileHeight;
		break;
	case RIGHT:
		//x and y position
		x = (int)(entity->getPosition().x) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height / 2) / this->tileHeight;
		break;
	case UP:
		//x and y position
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width / 2) / this->tileWidth;
		y = (int)(entity->getPosition().y) / this->tileHeight;
		break;
	case DOWN:
		//x and y position
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width / 2) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height) / this->tileHeight;
		break;
	case TOP_LEFT:
		//x and y position
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width) / this->tileWidth;
		y = (int)(entity->getPosition().y) / this->tileHeight;
		break;
	case TOP_RIGHT:
		//x and y position
		x = (int)(entity->getPosition().x) / this->tileWidth;
		y = (int)(entity->getPosition().y) / this->tileHeight;
		break;
	case BOTTOM_LEFT:
		//x and y position
		x = (int)(entity->getPosition().x + entity->getGlobalBounds().width) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height) / this->tileHeight;
		break;
	case BOTTOM_RIGHT:
		//x and y position
		x = (int)(entity->getPosition().x) / this->tileWidth;
		y = (int)(entity->getPosition().y + entity->getGlobalBounds().height) / this->tileHeight;
		break;
	}
	entity->move(-Entity::scroll); //allow the player to move backwards and scroll backwards
	if (this->data["Ground"][x + y * this->width] == 0) {
		TileProperties temp = { 0, NO_GROUP, false };
		return temp; 
	}
	else {
		return this->tileProperties[this->data["Ground"][x + y * this->width]];
	}
}

//Ensures that the entity is properly deleted when it is destroyed
Map::~Map() {
		//Properly deletes all of the tiles once the game is closed 
	//for loop makes sure to delete all of the references in relation to the tiles 
	for (auto iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		delete iterator->second;
	}
	for (auto iterator = this->flippedTiles.begin(); iterator != this->flippedTiles.end(); iterator++) {
		delete iterator->second;
	}
	for (auto iterator = this->data.begin(); iterator != this->data.end(); iterator++) {
		delete iterator->second;
	}
	for (auto iterator = this->tileTextures.begin(); iterator != this->tileTextures.end(); iterator++) {
		delete iterator->second;
	}
	for (auto iterator = this->mapTiles.begin(); iterator != this->mapTiles.end(); iterator++) {
		delete iterator->second;
	}
	this->data.clear();
	this->tiles.clear();
	this->tileProperties.clear();
	
	delete this->backgroundTexture;
	delete this->background;
}