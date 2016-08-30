#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Asteroid.h"
#include "ball.h"
#include "gameObject.h"

class Paddle : public GameObject
{
public:
	sf::ConvexShape mPaddle;

	Paddle(float screenWidth, float screenHeight);
	~Paddle();

	virtual void Init(bool leftPaddle);
	virtual void Update(sf::Time& time);
	void respawnTimer(sf::Time& time);
	virtual void Draw(sf::RenderWindow *window);
	bool CheckCollisions();

	bool CheckCollisionsAsteroids(Asteroid* asteroid, int player);
	void resetPaddleSize();

	void setPosition(float x, float y);
	void setRotation(float degrees) { mPaddle.setRotation(degrees); };
	void setAliveStatus(bool status) { mAliveStatus = status; };
	
	float getPositionX();
	float getPositionY();
	bool getAliveStatus() { return mAliveStatus; };
	float getRotation() { return mPaddle.getRotation(); };

private:
	float paddleWidth;
	float paddleHeight;
	float paddleRadius;
	sf::SoundBuffer paddleBuffer;
	sf::Sound paddleSound;
	sf::Vector2f ballLastPos;

	bool mLeftPaddle;
	bool mAliveStatus;
	bool FKeyPressed;

	int mPauseTimer;
	int mAliveTimer;
};
