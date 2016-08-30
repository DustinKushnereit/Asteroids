#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Bullet
{
public:
	sf::CircleShape mBullet;

	Bullet(float screenWidth, float screenHeight);
	Bullet() {};
	~Bullet() {};

	virtual void Init();
	virtual void Update(sf::Time& deltaTime);
	virtual void Draw(sf::RenderWindow *window);
	virtual void CheckCollisions();
	virtual float GetBallRadius();

	void setFillColor(sf::Color newColor);
	void setOutlineColor(sf::Color newColor);
	void setPosition(float tmpX, float tmpY) { mBullet.setPosition((float)tmpX, (float)tmpY); };
	void setRotation(float rotation) { mBullet.setRotation(rotation); };
	void setFiredStatus(bool status) { hasBeenFired = status; };
	void setAliveStatus(bool aliveStatus) { mIsAlive = aliveStatus; };
	void setVelocity(sf::Vector2f temp) { velocity = temp; };
	void setAngle(float angle) { mAngle = angle; };
	void setSin(bool sin) { mSin = sin; };
	void setCos(bool cos) { mCos = cos; };

	sf::Vector2f getPosition() { return position; };
	sf::Vector2f getVelocity() { return velocity; };
	bool getFiredStatus() { return hasBeenFired; };
	bool getAliveStatus() { return mIsAlive; };

private:
	float ballRadius;

	bool mIsAlive;
	bool hasBeenFired;
	bool mSin;
	bool mCos;

	sf::SoundBuffer wallBuffer;
	sf::Sound wallSound;

	float screenX;
	float screenY;
	sf::Vector2f position;
	sf::Vector2f velocity;
	float mAngle;
	float DEG2RAD;
};

