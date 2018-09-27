#include "mainGame.h" //player needs to be loaded into the mainGame
#include "player.h"
#include "bullet.h"
#include "playerBullet.h"
#include "playerAOEBullet.h"
#include "winScreen.h"
#include "gameOver.h"
#include <iostream>


Player::Player(sf::RenderWindow *window, EntityManager *entityManager, Map *map, float x, float y) {
	this->Load("animations/player/byakuranspritesheet.png", sf::IntRect(0, 0, 80, 120)); 
	this->setPosition(x, y); //set position of the player based on x & y
	this->map = map; //sets the map properly
	this->speed = 1.0f; //ref to the speed of the player
	this->groupID = 1; //groupID of the player is 1
	this->jump = 0; //not jumping initially
	this->attack = 0; 
	this->sprint = 0; //not sprinting initially
	this->entityManager = entityManager;
	Entity::scroll = sf::Vector2f(0, 0);//resets the scrolling every time
	this->fps = 16 * 7; //slows down the animations 
	this->yOffset = 2; //this adds perfect collision to the ground blocks

	//Set these based on the save system file 
	this->health = saveSystem.health; //health is set based on the savey system file 
	this->mana = saveSystem.mana;
	this->stamina = saveSystem.stamina; 

	//Jump sound effect
	this->jumpBuffer = new sf::SoundBuffer();
	this->jumpBuffer->loadFromFile("Sounds/jump.ogg"); //.ogg is waaay smaller than .wav, so it will load faster 
	this->jumpSound = new sf::Sound(*this->jumpBuffer);
	//Shoot sound effect
	this->shootBuffer = new sf::SoundBuffer();
	this->shootBuffer->loadFromFile("Sounds/shoot.ogg"); //.ogg is waaay smaller than .wav, so it will load faster 
	this->shootSound = new sf::Sound(*this->shootBuffer);
	//Special sound effect
	this->specialMoveBuffer = new sf::SoundBuffer();
	this->specialMoveBuffer->loadFromFile("Sounds/specialskill.ogg"); //.ogg is waaay smaller than .wav, so it will load faster 
	this->specialMoveSound = new sf::Sound(*this->specialMoveBuffer);
	//Guard sound effect
	this->guardBuffer = new sf::SoundBuffer();
	this->guardBuffer->loadFromFile("Sounds/guardspell.ogg"); //.ogg is waaay smaller than .wav, so it will load faster 
	this->guardSound = new sf::Sound(*this->guardBuffer);


	this->Flip(saveSystem.flip);
	if (saveSystem.flip) { //if the last save said you flipped the player's position, then it flips next time it loads in
		this->direction = 180; 
	}

	//***SCROLLING(while loop in order for it to update the scroll constantly)
	//(RIGHT)the window screen should scroll the map along whenever the player moves forwards(to the right) (also makes sure the screen stops scrolling after we reached the end of the map)
	while (this->getPosition().x + this->getGlobalBounds().width + 512 > window->getSize().x && Entity::scroll.x < this->map->getGlobalBounds().width - window->getSize().x) {
		this->map->move(-abs(this->speed), 0);
		this->move(-abs(this->speed), 0); //prevents the player from moving too much forwards
		Entity::scroll.x += abs(this->speed); //increase the scrolling since the player is scrolling at this point
	}
	//(LEFT)the window screen should scroll the map along whenever the player moves backwards(to the left) (also makes sure the screen stops scrolling after we reached the end of the map)
	while (this->getPosition().x - 512 < 0 && Entity::scroll.x > 0) {
		this->map->move(abs(this->speed), 0);
		this->move(abs(this->speed), 0); //prevents the player from moving too much forwards
		Entity::scroll.x -= abs(this->speed); //increase the scrolling since the player is scrolling at this point
	}
	//(UP)the window screen should scroll the map along whenever the player moves up (also makes sure the screen stops scrolling after we reached the end of the map)
	while (this->getPosition().y + this->getGlobalBounds().height + 256 > window->getSize().y && Entity::scroll.y < this->map->getGlobalBounds().height - window->getSize().y) {
		this->map->move(0, -abs(this->speed));
		this->move(0, -abs(this->speed)); //prevents the player from moving too much forwards
		Entity::scroll.y += abs(this->speed); //increase the scrolling since the player is scrolling at this point
	}
	//(DOWN)the window screen should scroll the map along whenever the player moves down(also makes sure the screen stops scrolling after we reached the end of the map)
	while (this->getPosition().y - 256 < 0 && Entity::scroll.y > 0) {
		this->map->move(0, abs(this->speed));
		this->move(0, abs(this->speed)); //prevents the player from moving too much forwards
		Entity::scroll.y -= abs(this->speed); //increase the scrolling since the player is scrolling at this point
	}
}
	//The Swim and stuff helps clean up the code of the player Update. 
//A function just to deal with water collision. Since the player can swim.
void Player::Swim(sf::RenderWindow *window) {
	this->jump = 0; //when the player hits the water, their jump should reset. 

	//takes in player movement(player can only move left and right since it's a platformer game) (You can do this with the A and D, A going to the left and D going to the right)
	this->velocity.x = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) * this->speed;
	//takes in player movement(player can only move up & down while underwater) (You can do this with the W and S, W to go up and S to go down while in water
	this->velocity.y = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) * this->speed;

	//checks on all sides for collision with a block/tile on the map 
	//the tile collisions will be diff now. If specifically using tile 69,70,etc, then the player should go past this tile but collide with it.
	if (this->map->CheckCollision(this, LEFT).group == GROUND) { //the tiles that are a part of the GROUND tile group
		this->move(-abs(this->speed), 0);
	}
	if (this->map->CheckCollision(this, RIGHT).group == GROUND) {
		this->move(abs(this->speed), 0);
	}
	if (this->map->CheckCollision(this, UP).group == GROUND) {
		this->move(0, abs(this->speed));
	}
	if (this->map->CheckCollision(this, DOWN).group == GROUND) { //this also allows the player to go through the green block(2nd block) from the bottom and land on top. it's a special block
		this->move(0, -abs(this->speed));
	}
}

void Player::Ground(sf::RenderWindow *window) { //this is what happens whent he player is on the regular ground tiles throughout the levels. 
	//Stamina Logic Below and the character animations for it(just an overall timer for all of the animations)
	sf::Time time = staminaTimer.getElapsedTime();
												
	//takes in player movement(player can only move left and right since it's a platformer game) (You can do this with the A and D, A going to the left and D going to the right)
	this->velocity.x = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) * this->speed;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) 
		&& sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { //just fixes the cheap method of flipping since you cannot hold the left and right key at the same time,so the player will just stay still in their default pose
		this->SetFrame(0); //otherwise, just give him the standing frame. 
	}
	//Animates the walking of the player
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		this->fps = 16 * 12; //slows down the animations 
		this->Animate(1, 6); //since the animation for walking starts at 1, and to the right, there are 5 more frames so 6 in total
	}
	else if(this->attack == 0){ //if not attacking
		if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) || !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))) { //not walking or attacking
			this->SetFrame(0); //otherwise, just give him the standing frame. 
		}
						   
		//this->Animate(6, 10); //this is the running animation
		//this->Animate(10, 18);//this is the normal attack animation
		//this->Animate(18, 26); //this is the special attack animation
		//this->Animate(26, 32); //this is the jump animation 
		//this->SetFrame(33); //guard animation 
		//this->SetFrame(34); //hurt animation 
	}

	//Sprint animations
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		&& sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) { //you're spriting and using the A or D key to move back and forth. 
		this->fps = 16 * 5;
		this->Animate(7, 10);
	}
	//Jump animation
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		this->fps = 16 * 9; 
		this->SetFrame(27);
		this->Animate(27, 32); //this is the jump animation 
	}
	//Guard animation
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)) {
		this->SetFrame(33); //guard animation 
	}

	//(Gravity)keeps track at every frame when the player falls back down after jumping
	if (this->velocity.y < 3.0f) {
		this->velocity.y += 0.1f; //this slows down the velocity.y each time to give a smooth transition of jumping upwards
	}

	//checks on all sides for collision with a block/tile on the map 
	//the tile collisions will be diff now. If specifically using tile 69,70,etc, then the player should go past this tile but collide with it.
	if (this->map->CheckCollision(this, LEFT).group == GROUND) { //tiles that are a part of the GROUND tile group 
		this->move(-abs(this->speed), 0);
	}
	if (this->map->CheckCollision(this, RIGHT).group == GROUND) {
		this->move(abs(this->speed), 0);
	}
	if (this->map->CheckCollision(this, UP).group == GROUND) {
		this->move(0, abs(this->speed));
		this->velocity.y = 0.0f; //if the player hits the top of this block, they should just fall back down(sense of gravity)
	}
	if (this->map->CheckCollision(this, DOWN).group == GROUND) { //this also allows the player to go through the green block(2nd block) from the bottom and land on top. it's a special block
		this->velocity.y = 0.0f; //if the player hits the bottom of this block, they should just fall back down(sense of gravity)
		this->jump = 0; //resets the jump back to 0 for the following if statements to work below
	}


	//the space bar key will be the player's jump, also if the player is not alrdy in the air then they can jump freely
	//gives variable jumps based on the int variable jump(by holding the space bar key to have variable jumps)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && this->jump == 0) {
		this->velocity.y = -3.0f;
		this->jump = 1;
		this->jumpSound->play();
	}
	else  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && this->jump == 1 && this->velocity.y > -5.0f) {
		this->velocity.y -= 0.25f; //adds a bit more jump to the initial jump
	}
	else if (this->velocity.y <= -5.0f) { //prevents infinite jumps
		this->jump = 2;
	}


	
	//Decrement the stamina when the player is holding down the sprint button and still has stamina left
	if (time.asSeconds() >= 1.0f && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) && saveSystem.GetStamina() != 0) {
		this->stamina -= 1;
		saveSystem.DecrementStamina(1);
		staminaTimer.restart(); //restart the timer 
	}
	
	//Regenerate the stamina back again if the current stamina is 0 and can keep recovering if it is less than the max stamina which is 10 as long as they are no longer holding the sprint key
	if ((!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) && saveSystem.GetStamina() < 10 ) ) {
		if (time.asSeconds() >= 1.0f) { //every second
			this->stamina += 1;
			saveSystem.IncrementStamina(1);
			staminaTimer.restart();
		}
	}
	
	
	//Make sure it's going back to walking speed if stamina runs out. One where you release the shift key and the stamina is 0. 
	if (saveSystem.GetStamina() == 0  && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) && this->speed > 2.0f) {
		this->speed -= 0.02f; //go back to normal speed
		this->sprint = 0; //so we can do the sprint again once they stop pressing the shift key
	} //the other being where the player keeps trying to shift but it shouldn't work since stamina is 0. Reset speed back to normal walking speed. 
	else if (saveSystem.GetStamina() == 0 && this->speed > 2.0f) {
		this->speed -= 0.02f; //go back to normal speed
		this->sprint = 0; //so we can do the sprint again once they stop pressing the shift key
	}

	//As long as we have stamina, then they can continue sprinting
	if (saveSystem.GetStamina() != 0) {
		//player can sprint if they hold the LShift 
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) && this->sprint == 0 && this->speed < 4.0f) {
			this->speed += 0.01f; //makes the player gradually move faster
		}
		//limits how fast they can run
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) && this->sprint == 0 && this->speed >= 4.0f) {
			this->sprint = 1;
		}
		//ensures that once we stop the player slows back down again. 
		else if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) && this->speed > 2.0f) { //if you're not pressing LShift
			this->speed -= 0.02f; //go back to normal speed
			this->sprint = 0; //so we can do the sprint again once they stop pressing the shift key
		}
	}
}

bool Player::Update(sf::RenderWindow *window) {
	if (saveSystem.GetHP() == 0) { //If the player reaches 0 health at any point, you restart the entire game
		saveSystem.Reset();
	}
	
	//if the player enters the water tiles
	if (this->map->CheckCollision(this, NONE).group == WATER) { //none since the player can go inside of the water
		this->Swim(window); //when the player collides with these specific 
	}
	else{ //this deals with the player's normal controls. Like walking and attacking, etc
		this->Ground(window);
	}

	
	//If the player is on the portal to the next area.
		//**NOTE: I think I need to get the name of the next area and then somehow get access to the map's sound system to change the background pic and sound. 
	if (this->map->CheckCollision(this, NONE).group == LEAVE) { //none since the player needs to be on it to interact w/ it
		if (this->enterKey) { //and they happen to press the enterkey while they are on it
			saveSystem.currentAreaEntry = this->map->CheckCollision(this, NONE).nextAreaEntry;
			saveSystem.currentMap = this->map->CheckCollision(this, NONE).nextArea;
			saveSystem.flip = this->flippedH; 
			saveSystem.AddAreaToFinished();
			saveSystem.Save(); // saves when the player goes to the nextArea. 
			coreState.SetState(new MainGame());
			return false;
		}
	}

	//When the player reaches the last examination door, they win the game and it should go to the win screen 
	if (this->map->CheckCollision(this, NONE).group == WIN) { 
		if (this->enterKey) { //and they happen to press the enterkey while they are on it
			saveSystem.Reset(); //resets the game to the beginning so the save file restarts
			saveSystem.Save(); // saves when the player goes to the nextArea. 
			coreState.SetState(new WinScreen());
			return false;
		}
	}
	

	//Dealing with the lava tiles and what happens to the player if they land on it 
	if (this->map->CheckCollision(this, DOWN).group == LAVA) { //if the player lands on top of the lava
		this->SetFrame(34); //hurt animation 
		this->SetFrame();
		if (this->jump == 0) { //if the player is jumping in the air, then we cannot knock them back 
			this->damageKnockBack = (this->flippedH) ? 3 : -3; //this will determine how much be bounce backwards on the x axis
		}
		this->velocity.x = this->damageKnockBack; //make these 0 to make the knockback more stable rather than being knocked back by a crazy amount, so knock the player back on the x axis a bit more
		this->velocity.y = -3;
		this->jump = 1; //allows us to jump out of it once we get hit
		saveSystem.DecrementHP(1);
		this->health -= 1; //get rid of the player's health 
	}


	//Dealing with the Trap tiles and what happens to the player if they land on it 
	if (this->map->CheckCollision(this, DOWN).group == TRAP) { //if the player lands on top of the trap
		this->SetFrame(34); //hurt animation 
		this->SetFrame();
		if (this->jump == 0) { //if the player is jumping in the air, then we cannot knock them back 
			this->damageKnockBack = (this->flippedH) ? 3 : -3; //this will determine how much be bounce backwards on the x axis
		}
		this->velocity.x = this->damageKnockBack; //make these 0 to make the knockback more stable rather than being knocked back by a crazy amount, so knock the player back on the x axis a bit more
		this->velocity.y = -3;
		this->jump = 1; //allows us to jump out of it once we get hit
		saveSystem.DecrementHP(1);
		this->health -= 1; //get rid of the player's health 
	}
	else if (this->map->CheckCollision(this, LEFT).group == TRAP || this->map->CheckCollision(this, RIGHT).group == TRAP) {
		std::cout << "Im hurting?" << std::endl;
		
	}
	else if (this->map->CheckCollision(this, UP).group == TRAP) { //If we hit a spike upwards, our height should go back down to 0
		if (jump == 0) {
			this->damageKnockBack = (this->flippedH) ? 3 : -3;
		}
		this->velocity.x = 0; 
		this->velocity.y = this->damageKnockBack; 
		this->jump = 0; //cannot jump again. 
		saveSystem.DecrementHP(1);
		this->health -= 1; //get rid of the player's health 
	}


	//All stuff below deal with knockback mechanic on the player
	this->velocity.x += this->damageKnockBack; //constantly adjusts the damageKnockBack based on the player's current position and movement
	
	//This if statement makes sure that the x to make sure he doesnt slide too much once he falls down from the knockback
	if (abs(this->velocity.x) < 1 &&  //pretty much gives a consistent amount of knockback in the x axis 
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))) {
		damageKnockBack = 0;
	}
	//damageKnockBack in the x axis so it causes a bit of a slide to the opposing side depending on
		//if the player gets hit on the left or the right
	if (this->damageKnockBack > 0) { 
		this->damageKnockBack -= 0.05f; //lower or increase these amounts to get more or less knockback on the x axis
	}
	else if (this->damageKnockBack < 0) {
		this->damageKnockBack += 0.05f;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		this->Flip(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)); //cheap way of making the player flip left and right depending on where they walk
	}

	//determines where the direction of the interaction invisible bullet/offensive bullet will go
		//can shoot in only 2 directions, left or right
	if (this->velocity.x > 0) { //x going to the right
		this->direction = 0.0f; //if facing the right shoot to the right
	}
	else if (this->velocity.x < 0) { //x going to the left
		this->direction = 180.0f; //if facing the left then shoot to the left 
	}
	
	//***SCROLLING
	//(RIGHT)the window screen should scroll the map along whenever the player moves forwards(to the right) (also makes sure the screen stops scrolling after we reached the end of the map)
	if (this->getPosition().x + this->getGlobalBounds().width + 512 > window->getSize().x && Entity::scroll.x < this->map->getGlobalBounds().width - window->getSize().x) {
		this->map->move(-abs(this->speed), 0);
		this->move(-abs(this->speed), 0); //prevents the player from moving too much forwards
		Entity::scroll.x += abs(this->speed); //increase the scrolling since the player is scrolling at this point
	}
	//(LEFT)the window screen should scroll the map along whenever the player moves backwards(to the left) (also makes sure the screen stops scrolling after we reached the end of the map)
	if (this->getPosition().x - 512 < 0 && Entity::scroll.x > 0) {
		this->map->move(abs(this->speed), 0);
		this->move(abs(this->speed), 0); //prevents the player from moving too much forwards
		Entity::scroll.x -= abs(this->speed); //increase the scrolling since the player is scrolling at this point
	}
	//(UP)the window screen should scroll the map along whenever the player moves up (also makes sure the screen stops scrolling after we reached the end of the map)
	if (this->getPosition().y + this->getGlobalBounds().height + 256 > window->getSize().y && Entity::scroll.y < this->map->getGlobalBounds().height - window->getSize().y) {
		this->map->move(0, -abs(this->speed + this->velocity.y));
		this->move(0, -abs(this->speed + this->velocity.y)); //prevents the player from moving too much forwards
		Entity::scroll.y += abs(this->speed + this->velocity.y); //increase the scrolling since the player is scrolling at this point
	}
	//(DOWN)the window screen should scroll the map along whenever the player moves down(also makes sure the screen stops scrolling after we reached the end of the map)
	if (this->getPosition().y - 256 < 0 && Entity::scroll.y > 0) {
		this->map->move(0, abs(this->speed));
		this->move(0, abs(this->speed)); //prevents the player from moving too much forwards
		Entity::scroll.y -= abs(this->speed); //increase the scrolling since the player is scrolling at this point
	}


	//If the player falls off the map, then they should die and the game over game state should take over. 
	if (this->getPosition().y + this->getGlobalBounds().height >= window->getSize().y) {
		coreState.SetState(new GameOver());
		return false;
	}


	/*
	//checks if the player is trying to go up to the nextArea to the left
	if (this->getPosition().x <= 0) {
		saveSystem.currentMap = map1->leftArea; //set the nextArea to the leftArea
		
		//set the player's new position now that we are in a diff area
		saveSystem.x = window->getSize().x - this->getGlobalBounds().width - 32;
		saveSystem.y = this->getPosition().y; //positions player to the right side of nextArea
		
		coreState.SetState(new main_game()); //update the state to go to the next area
		return false;
	}
	//if the player goes down then they should go to the right
	if (this->getPosition().x + this->getGlobalBounds().width >= window->getSize().x) {
		saveSystem.currentMap = map1->rightArea; //set the nextArea to the rightArea
		
		//set the player's new position now that we are in a diff area
		saveSystem.x = 32;
		saveSystem.y = this->getPosition().y; //positions player to the left side of nextArea
		
		coreState.SetState(new main_game()); //update the state to go to the next area
		return false;
	}
	*/

	//if the enterKey is pressed, that means the player is trying to interact w/ an NPC
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return) && !this->enterKey) {
		this->entityManager->Add("bullet", new Bullet(this->map, this->getPosition().x + Entity::scroll.x, this->getPosition().y + this->getGlobalBounds().height / 4 + Entity::scroll.y, direction, 64)); //shoot out an invisible bullet(range of 32) to interact
	}
	//if the S key is pressed, that means the player is shooting their offensive bullets which is pretty much the range of their normal attacks.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I) && !this->iKey) {
		this->entityManager->Add("pBullet", new PlayerBullet(this->map, this->getPosition().x + Entity::scroll.x, this->getPosition().y + this->getGlobalBounds().height / 4 + Entity::scroll.y, direction, 300)); //shoot out offensive bullets w/ range depending on the sword tip. The addition to the y axis was to make the bullet go lower
		//this->Animate(10, 18);//this is the normal attack animation
		this->SetFrame(13);
		this->SetFrame(); //automatically resets to the default pose once the attack is done 
		this->shootSound->play();
	}


	//If the O Key is pressed, it is an AOE move, so I think I need to make at least 2 player bullet that shoots in 2 directions. I should make a duplicate playerBullet class so that it is transparent and not visible.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O) && !this->oKey && saveSystem.GetMP() != 0) { //MP must not be 0 in order to use the special skill
		//The bullet gets shot in both directions as a range. 
		this->entityManager->Add("rPBullet", new PlayerAOEBullet(this->map, this->getPosition().x + Entity::scroll.x, this->getPosition().y + this->getGlobalBounds().height / 4 + Entity::scroll.y, direction, 100));
		this->entityManager->Add("lPBullet", new PlayerAOEBullet(this->map, this->getPosition().x + Entity::scroll.x, this->getPosition().y + this->getGlobalBounds().height / 4 + Entity::scroll.y, direction + 180.0f, 100)); //added the direction by 180 degrees to go the opposite direction
		this->SetFrame(26);
		this->SetFrame();
		saveSystem.DecrementMana(1);
		this->mana -= 1;
		this->specialMoveSound->play();
	}

	//If the player presses U, then they are guarding and it should send a bullet out that knocks the enemy backwards. CURRENTLY NOT DONE, JUST SOUND EFFECTS & ANIMATIONS
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U) && !this->uKey) {
		this->guardSound->play();
	}

	this->spaceKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space); //keeps track of when the player presses the spaceBar for jumping
	this->iKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I); //keeps track of when the player presses the I key for normal attacks
	this->oKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O); //keeps track of when the player presses the O key for a special attack that uses mana
	this->uKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U); //keeps track of the player pressing the U key to guard
	this->enterKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return); //keeps track of when the player presses the enterKey for interactions

	Entity::Update(window); //be sure to constantly update the entity's movement(in this case the player's movement)
	return true;
}

//Tracks collisions that the player does with other entities
void Player::Collision(Entity *entity) {
	switch (entity->GroupID()) {
	case 5: //if player collides with an enemy 
		this->SetFrame(34); //hurt animation 
		this->SetFrame();
		//This is the exact same effect when the player touches lava, you get a knockback when colliding with the enemy
		this->damageKnockBack = (this->flippedH) ? 3 : -3; //this will bounces away from the enemy on the x axis
		this->velocity.x = 0; //make these 0 to make the knockback more stable rather than being knocked back by a crazy amount
		this->velocity.y = -3.0f; //makes us bounce us upwards
		this->jump = 0; //allows us to jump out of it once we get hit
		saveSystem.DecrementHP(1);
		this->health -= 1; //take away the health 

		entity->velocity.x *= -1; //entity walks the other way once collision occurs
		entity->flippedH = !entity->flippedH; //enemy gets flipped to go to the other direction 
		break;
	case 6: //when the player collides with the HP potion 
		entity->Destroy();
		saveSystem.IncrementHP(1);
		this->health += 1; //recovers the players health by 1
		break;
	case 7: //whent he player collides with the MP potion
		entity->Destroy();
		saveSystem.IncrementMana(1);
		this->mana += 1; 
		break;
	}
}

Player::~Player() {
	delete this->jumpBuffer;
	delete this->jumpSound;
	delete this->shootBuffer;
	delete this->shootSound;
	delete this->guardBuffer;
	delete this->guardSound;
	delete this->specialMoveBuffer;
	delete this->specialMoveSound;
}