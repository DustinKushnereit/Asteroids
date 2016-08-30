#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Bullet.h"

class Asteroid
{
public:
	sf::ConvexShape asteroid;

	Asteroid(float screenWidth, float screenHeight);
	Asteroid() {};
	~Asteroid() {};

	virtual void Init(int ID);
	virtual void Update(sf::Time& deltaTime);
	virtual void Draw(sf::RenderWindow *window);
	virtual void CheckCollisions();

	void setFillColor(sf::Color newColor);
	void setOutlineColor(sf::Color newColor);
	void setPosition(int tmpX, int tmpY) { asteroid.setPosition((float)tmpX, (float)tmpY); };
	void setAliveStatus(bool aliveStatus) { mIsAlive = aliveStatus; };
	void setVelocity(sf::Vector2f temp) { velocity = temp; };
	void setAsteroidType(int type) { asteroidType = type; };
	void setGoingSideWays(bool status) { goingSideWays = status; };

	sf::Vector2f getPosition() { return position; };
	sf::Vector2f getVelocity() { return velocity; };
	bool getAliveStatus() { return mIsAlive; };
	int getAsteroidType() { return asteroidType; };
	int getAsteroidID() { return asteroidID; };
	bool getGoingSideWays() { return goingSideWays; };

	bool CheckCollisionsAsteroids(Bullet* bullet);

private:
	float ballRadius;
	bool mIsAlive;
	int asteroidType;
	int asteroidID;
	bool goingSideWays;

	sf::SoundBuffer wallBuffer;
	sf::Sound wallSound;

	float screenX;
	float screenY;
	sf::Vector2f position;
	sf::Vector2f velocity;
};

