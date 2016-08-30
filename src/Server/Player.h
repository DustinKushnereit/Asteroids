#pragma once
#include "RakPeerInterface.h"
#include "../Common/Common.h"

class Player
{
public:
	Player();
	Player(RakNet::RakNetGUID myGUID);
	~Player();

	void setPosition(float X, float Y);
	void setPosition(Position newPos);
	void setRotation(float newRotation);
	Position getPosition();
	float getRotation();
	float getX();
	float getY();
	RakNet::RakNetGUID getGUID();
private:
	Position position;
	float rotation;

	RakNet::RakNetGUID guid;
};