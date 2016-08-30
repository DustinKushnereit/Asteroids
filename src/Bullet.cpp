#include "Bullet.h"
#include "gameObject.h"
#include <time.h>
#include "Game.h"

#include <math.h>
#include <cmath>
#include <cassert>

Bullet::Bullet(float screenWidth, float screenHeight)
{
	screenX = screenWidth;
	screenY = screenHeight;
	mIsAlive = true;
	DEG2RAD = 3.14159f / 180.0f;
	mSin = true;
	mCos = true;
}

void Bullet::Init()
{
	ballRadius = 7;

	mIsAlive = false;
	hasBeenFired = false;

	//mBullet.setPointCount(3);

	mBullet.setRadius(ballRadius);
	mBullet.setOrigin(ballRadius / 2, ballRadius / 2);

	sf::Vector2f origin = mBullet.getOrigin();
	velocity = sf::Vector2f(0.8f, 9.0f);

	mBullet.setOutlineThickness(3);
	mBullet.setOutlineColor(sf::Color::Green);
	mBullet.setFillColor(sf::Color(255, 0, 255));
	mBullet.setPosition(screenX / 2, screenY / 3);

	//wallBuffer.loadFromFile("../resources/ball.wav");
	wallSound.setVolume(20);
	wallSound.setBuffer(wallBuffer);
}

void Bullet::Update(sf::Time& deltaTime)
{
	if (mIsAlive)
	{
		position = mBullet.getPosition();
		sf::Vector2f mDirection(cos(mAngle * DEG2RAD), sin(mAngle * DEG2RAD));
		sf::Vector2f distance = mDirection * 0.9f * deltaTime.asSeconds();
		//mBullet.move(distance.x, distance.y * velocity.y);

		if (mSin)
			velocity.x = sin(mAngle);
		else if (!mSin)
			velocity.x = -sin(mAngle);

		if (mCos)
			velocity.y = cos(mAngle);
		else if (!mCos)
			velocity.y = -cos(mAngle);

		mBullet.move(velocity.x * deltaTime.asSeconds() * 500.0f, velocity.y * deltaTime.asSeconds() * 500.0f);
		CheckCollisions();
	}
}

void Bullet::Draw(sf::RenderWindow *window)
{
	if (mIsAlive)
		window->draw(mBullet);
}

void Bullet::setFillColor(sf::Color newColor)
{
	mBullet.setFillColor(newColor);
}

void Bullet::setOutlineColor(sf::Color newColor)
{
	mBullet.setOutlineColor(newColor);
}

void Bullet::CheckCollisions()
{
	if (mBullet.getPosition().y <= -200)
	{
		mBullet.setPosition(mBullet.getPosition().x, screenY + 100);
		wallSound.play();
		mIsAlive = false;
		hasBeenFired = false;
	}

	if (mBullet.getPosition().y >= screenY + 200)
	{
		mBullet.setPosition(mBullet.getPosition().x, -100);
		wallSound.play();
		mIsAlive = false;
		hasBeenFired = false;
	}

	if (mBullet.getPosition().x <= -200)
	{
		mBullet.setPosition(screenX + 100, mBullet.getPosition().y);
		wallSound.play();
		mIsAlive = false;
		hasBeenFired = false;
	}

	if (mBullet.getPosition().x >= screenX + 200)
	{
		mBullet.setPosition(-100, mBullet.getPosition().y);
		wallSound.play();
		mIsAlive = false;
		hasBeenFired = false;
	}
}

float Bullet::GetBallRadius()
{
	return ballRadius;
}