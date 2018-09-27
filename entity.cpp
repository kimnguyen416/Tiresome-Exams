#include "entity.h"

sf::Vector2f Entity::scroll(0, 0);

//When an entity is created it will have the following
Entity::Entity() { //default constructor
	this->texture = new sf::Texture(); //makes sure that whenever an entity is created, we make sure that its texture is set up
	this->active = 1; //initially, the entity will be set to active. 
	this->groupID = 0; //set all entities to the groupID by default as 0 for now
	this->startFrame = 0;
	this->endFrame = 0; 
	this->currentFrame = 0; 
	this->fps = 16; 
	this->xOffset = 0; 
	this->yOffset = 0;
	this->Flip(false, false); //initially set the entities flipping to false 
}

//loads in the image file that we desire
void Entity::Load(std::string filename, const sf::IntRect &area) { //the 2nd parameter is needed to handle animating the entities
	this->texture->loadFromFile("Graphics/sprites/" + filename); //this is the specific directory that the sprites are in
	this->setTexture(*this->texture); //properly set the texture so that it can be used. 
	this->frameSize = area;
	this->timer.restart(); //timer restarts whenever the sprite is loaded (so every animation causes a timer restart?)
	
	if (this->frameSize.width <= 0 || this->frameSize.height <= 0) {
		this->frameSize = sf::IntRect(0, 0, this->getGlobalBounds().width, this->getGlobalBounds().height);
	}
	else { //keeps track of flipping the entity
		int left = (this->flippedH) ? (this->frameSize.left + this->xOffset) + this->frameSize.width : (this->frameSize.left + this->xOffset);
		int top = (this->flippedV) ? (this->frameSize.top + this->yOffset) + this->frameSize.height : (this->frameSize.top + this->yOffset);
		int width = (this->flippedH) ?  -this->frameSize.width : this->frameSize.width;
		int height = (this->flippedV) ? -this->frameSize.height : this->frameSize.height;
		this->setTextureRect(sf::IntRect(left, top, width, height));
	}
	this->startFrame = 0;
	this->endFrame = 0;
	this->currentFrame = 0; 
}

//Just sets the current frame size of animation of the entity(like how big each frame should be)
void Entity::SetFrameSize(sf::IntRect size) {
	this->frameSize = size; 
}


//Checks for collision between entities/objects
bool Entity::CheckCollision(Entity *entity) {
	return this->getGlobalBounds().intersects(entity->getGlobalBounds());
}

//Gets the groupID
int Entity::GroupID() {
	return this->groupID;
}


//Checks if the entity is active or not
int Entity::Active() {
	return this->active;
}

//Allows you to destroy an entity
void Entity::Destroy() {
	this->active = 0; //we do this by setting active to 0
}


//Moves the object based on the velocity give to it. We want to be able to get access to the window size and make more entities w/ the manager
bool Entity::Update(sf::RenderWindow *window) { //virtual allows you to override a function
	this->move(this->velocity);
	return true;
}

//Draws out every frame of animation
bool Entity::Render(sf::RenderWindow *window) {
	sf::Time elapsed = this->timer.getElapsedTime();
	if (elapsed.asMilliseconds() >= this->fps) {
		this->currentFrame += 1;
		if (this->currentFrame >= this->endFrame) {
			this->currentFrame = this->startFrame; //loops back to the first frame?
		}
		this->SetFrame();
		this->timer.restart();
	}
	return true; 
}

//We need a specialized collision function that can be changed(which is why it is virtual) to detect diff types of collisions
//In this case diff bullets affect diff entities
void Entity::Collision(Entity *entity) {
}

//Ensures that the entity is properly deleted when it is destroyed
Entity::~Entity() {
	delete this->texture; //will go through and delete our textures
}