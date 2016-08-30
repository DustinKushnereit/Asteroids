#include "Player.h"

Player::Player()
{
	guid = RakNet::UNASSIGNED_RAKNET_GUID;
}

Player::Player(RakNet::RakNetGUID myGUID)
{
	guid = myGUID;
}

Player::~Player()
{

}

void Player::setPosition(float X, float Y)
{
	position.x = X;
	position.y = Y;
}

void Player::setPosition(Position newPos)
{
	position = newPos;
}

void Player::setRotation(float newRotation)
{
	rotation = newRotation;
}

Position Player::getPosition()
{
	return position;
}

float Player::getRotation()
{
	return rotation;
}

float Player::getX()
{
	return position.x;
}

float Player::getY()
{
	return position.y;
}

RakNet::RakNetGUID Player::getGUID()
{
	return guid;
}