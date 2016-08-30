#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "gameObject.h"

class Ball : public GameObject
{
public:
	sf::CircleShape gameBall;

	Ball(float screenWidth, float screenHeight);
	~Ball() {};

	virtual void Init();
	virtual void Update(sf::Time& deltaTime);
	virtual void Draw(sf::RenderWindow *window);
	virtual void CheckCollisions();
	virtual float GetBallRadius();

	void setFillColor(sf::Color newColor);
	void setOutlineColor(sf::Color newColor);
	void setPosition(int tmpX, int tmpY) { gameBall.setPosition((float)tmpX, (float)tmpY); };
	void setAliveStatus(bool aliveStatus) { mIsAlive = aliveStatus; };

private:
	float ballRadius;

	bool mIsAlive;

	sf::SoundBuffer wallBuffer;
	sf::Sound wallSound;
};

