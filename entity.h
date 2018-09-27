#pragma once

#include <SFML/Graphics.hpp> 
#include <string>

//Serves as a template to make other child entities out of this class
//Needs to be public so that all of it's methods can be used to other classes and files
class Entity : public sf::Sprite { //sprite since the entities are essentially sprites & to get access to sprite manipulation functions
public:
	sf::Vector2f velocity; //speed of the entities 
	//static = meaning it never changes
	static sf::Vector2f scroll; //keep track of the limits of the scrolling of the screen
	int fps; //needed to slow down the frame rate of the entity animation 
	int xOffset, yOffset; 
	bool flippedH, flippedV; //determines if the player goes left and right and what direction they need to face

	//***When an entity is created it will have the following
	Entity(); //default constructor

	//loads in the image file that we desire
	void Load(std::string filename, const sf::IntRect &area = sf::IntRect());

	//Helps add in different frames of animations on an entity
	void SetFrameSize(sf::IntRect size);
	
	//moves the object based on the velocity give to it. We want to be able to get access to the window size and make more entities w/ the manager
	virtual bool Update(sf::RenderWindow *window);

	//Renders the animations every frame
	virtual bool Render(sf::RenderWindow *window);

	//checks for collision between entities/objects
	bool CheckCollision(Entity *entity);

	//We need a specialized collision function that can be changed(which is why it is virtual) to detect diff types of collisions
		//in this case diff bullets affect diff entities
	virtual void Collision(Entity *entity);

	inline void SetFrame() { //sets the frames of animations of the entities
		//Animate here, keeps track of the current frames of the entity
		int frameX = this->currentFrame * this->frameSize.width;
		int frameY = 0;
		while (frameX >= this->texture->getSize().x) {
			frameX -= this->texture->getSize().x;
			frameY += this->frameSize.height;
		}
		this->frameSize.left = frameX;
		this->frameSize.top = frameY;

		int left = (this->flippedH) ? (this->frameSize.left + this->xOffset) + this->frameSize.width : (this->frameSize.left + this->xOffset);
		int top = (this->flippedV) ? (this->frameSize.top + this->yOffset) + this->frameSize.height : (this->frameSize.top + this->yOffset);
		int width = (this->flippedH) ? -this->frameSize.width : this->frameSize.width;
		int height = (this->flippedV) ? -this->frameSize.height : this->frameSize.height;
		this->setTextureRect(sf::IntRect(left, top, width, height));
		this->timer.restart();
	}

	inline void Animate(int start, int end){ //actually animate those frames
		this->startFrame = start;
		this->endFrame = end; 
	}

	inline void SetFrame(int frame) {
		this->startFrame = this->endFrame = this->currentFrame = frame; 
	}

	inline void Flip(bool horizontal = false, bool vertical = false) { //makes the entity flip depending on where the entity is walking
		this->flippedH = horizontal;
		this->flippedV = vertical;
	}

	//gets the groupID
	int GroupID();

	//checks if the entity is active or not
	int Active();

	//allows you to destroy an entity
	virtual void Destroy();

	//Ensures that the entity is properly deleted when it is destroyed
	~Entity();
protected: //variables that cannot be accessed outside of this class unless other classes inherit it
	int active; 
	int groupID; //gives every entity a category. Helps use worry abt each entities names less since we now have a groupID
	int currentFrame; 
private:
	int startFrame, endFrame; //start and end frame of the animation
	sf::Clock timer; //the timer will help us track things 
	sf::IntRect frameSize; //ref to the frame size of the entity
	sf::Texture *texture; //reference to the texture, actul graphics of the entity 
};