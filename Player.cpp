#include "Player.h"

//Initializer Functions
void Player::initVariables()
{
}

void Player::initComponents()
{
	
}

//Constructors / Destructors
Player::Player(float x, float y, sf::Texture& texture_sheet)
{
	this->initVariables();

	this->setPosition(x, y);
	
	this->createMovementComponent(200.f, 40.f, 30.f);
	this->createAnimationComponent(texture_sheet);

	this->animationComponent->addAnimation("PLAYER_SHEET", 100.f, 0, 0, 7, 1, 55, 55);
}

Player::~Player()
{
}
