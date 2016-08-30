#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "ball.h"
#include "gameObject.h"

class Block : public GameObject
{
public:
	Block(float screenWidth, float screenHeight);
	~Block(){};

	virtual void Init();
	virtual void Draw(sf::RenderWindow *window);
	virtual void CheckCollisions(Ball* ball);

	void setFillColor(sf::Color newColor);
	void setOutlineColor(sf::Color newColor);
	void setPosition(int tmpX, int tmpY) { mBlock.setPosition((float)tmpX, (float)tmpY); };
	void setAliveStatus(bool aliveStatus) { mIsAlive = aliveStatus; };

private:
	float blockWidth;
	float blockHeight;

	sf::SoundBuffer blockBuffer;
	sf::Sound blockSound;
	sf::Vector2f ballLastPos;
	sf::Vector2f mBallLastPos;
	sf::RectangleShape mBlock;

	bool mIsAlive;

};