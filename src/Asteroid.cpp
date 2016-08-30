#include "Asteroid.h"
#include "gameObject.h"
#include "Game.h"
#include <time.h>

Asteroid::Asteroid(float screenWidth, float screenHeight)
{
	screenX = screenWidth;
	screenY = screenHeight;
	mIsAlive = true;
	asteroidType = 1;
	goingSideWays = false;
}

void Asteroid::Init(int ID)
{
	mIsAlive = true;

	asteroidID = ID;

	if (asteroidType == 0)
	{
		asteroid.setPointCount(5);

		// define the points
		asteroid.setPoint(0, sf::Vector2f(0, 0));
		asteroid.setPoint(1, sf::Vector2f(110, 10));
		asteroid.setPoint(2, sf::Vector2f(130, 90));
		asteroid.setPoint(3, sf::Vector2f(30, 100));
		asteroid.setPoint(4, sf::Vector2f(0, 50));

		asteroid.setOrigin(60, 80);
	}
	else if (asteroidType == 1)
	{
		asteroid.setPointCount(5);

		// define the points
		asteroid.setPoint(0, sf::Vector2f(0, 0));
		asteroid.setPoint(1, sf::Vector2f(40, 10));
		asteroid.setPoint(2, sf::Vector2f(60, 30));
		asteroid.setPoint(3, sf::Vector2f(30, 80));
		asteroid.setPoint(4, sf::Vector2f(0, 20));

		asteroid.setOrigin(30, 40);
	}
	else if (asteroidType == 2)
	{
		asteroid.setPointCount(5);

		// define the points
		asteroid.setPoint(0, sf::Vector2f(0, 0));
		asteroid.setPoint(1, sf::Vector2f(40, 10));
		asteroid.setPoint(2, sf::Vector2f(30, 40));
		asteroid.setPoint(3, sf::Vector2f(20, 20));
		asteroid.setPoint(4, sf::Vector2f(0, 10));

		asteroid.setOrigin(30, 20);
	}
	else if (asteroidType == 3)
	{
		asteroid.setPointCount(5);

		// define the points
		asteroid.setPoint(0, sf::Vector2f(0, 0));
		asteroid.setPoint(1, sf::Vector2f(40, 10));
		asteroid.setPoint(2, sf::Vector2f(30, 40));
		asteroid.setPoint(3, sf::Vector2f(20, 20));
		asteroid.setPoint(4, sf::Vector2f(0, 10));

		asteroid.setOrigin(30, 20);
	}

	sf::Vector2f origin = asteroid.getOrigin();
	velocity = sf::Vector2f(0.08f, 0.1f);

	asteroid.setOutlineThickness(3);

	//wallBuffer.loadFromFile("../resources/ball.wav");
	wallSound.setVolume(20);
	wallSound.setBuffer(wallBuffer);
}

void Asteroid::Update(sf::Time& deltaTime)
{
	if (mIsAlive)
	{
		position = asteroid.getPosition();
		//if (position.y <= 300)
			asteroid.move(velocity.x * deltaTime.asSeconds(), velocity.y * deltaTime.asSeconds());
		CheckCollisions();
	}
	else
		asteroid.setPosition(asteroid.getPosition().x, -100);
}

void Asteroid::Draw(sf::RenderWindow *window)
{
	asteroid.rotate(0.09f);

	if (mIsAlive)
		window->draw(asteroid);
}

void Asteroid::setFillColor(sf::Color newColor)
{
	asteroid.setFillColor(newColor);
}

void Asteroid::setOutlineColor(sf::Color newColor)
{
	asteroid.setOutlineColor(newColor);
}

void Asteroid::CheckCollisions()
{

	/*if (asteroid.getPosition().y - ballRadius <= -200)
	{
		velocity.y *= -1;
		asteroid.setPosition(asteroid.getPosition().x, ballRadius - 200);
		wallSound.play();
	}

	if (asteroid.getPosition().y - ballRadius >= screenY + 100)
	{
		velocity.y *= -1;
		asteroid.setPosition(asteroid.getPosition().x, screenY + 100 - ballRadius);
		wallSound.play();
	}

	if (asteroid.getPosition().x <= ballRadius -200)
	{
		velocity.x *= -1;
		asteroid.setPosition(ballRadius -200, asteroid.getPosition().y);
		wallSound.play();
	}

	if (asteroid.getPosition().x >= screenX - ballRadius + 200)
	{
		velocity.x *= -1;
		asteroid.setPosition(screenX - ballRadius + 200, asteroid.getPosition().y);
		wallSound.play();
	}*/

	if (asteroid.getPosition().y <= -200)
	{
		asteroid.setPosition(asteroid.getPosition().x, screenY + 100);
		wallSound.play();
	}

	if (asteroid.getPosition().y >= screenY + 200)
	{
		asteroid.setPosition(asteroid.getPosition().x, -100);
		wallSound.play();
	}

	if (asteroid.getPosition().x <= -50)
	{
		asteroid.setPosition(screenX + 25, asteroid.getPosition().y);
		wallSound.play();
	}

	if (asteroid.getPosition().x >= screenX + 50)
	{
		asteroid.setPosition(-25, asteroid.getPosition().y);
		wallSound.play();
	}
}

bool Asteroid::CheckCollisionsAsteroids(Bullet* bullet)
{
	bool asteroidHit = false;

	if (mIsAlive)
	{
		if (bullet->getAliveStatus())
		{
			if (asteroidType == 0)
			{
				if ((asteroid.getPosition().x - 60 < bullet->getPosition().x + bullet->GetBallRadius()) && (asteroid.getPosition().x + 70 > bullet->getPosition().x - bullet->GetBallRadius()))
				{
					if ((asteroid.getPosition().y - 60 < bullet->getPosition().y + bullet->GetBallRadius()) && (asteroid.getPosition().y + 80 > bullet->getPosition().y))
					{
						float tmpX, tmpY;
						tmpX = asteroid.getPosition().x;
						tmpY = asteroid.getPosition().y;

						//std::cout << asteroidID << "\n";
						gpGame->createNewAsteroid(asteroidID);
						asteroidType = 1;
						Init(asteroidID);
						asteroid.setPosition(tmpX - 40, tmpY);
						gpGame->increaseTotalPoints(50 * gpGame->getLevelNumber());

						if (goingSideWays)
						{
							setVelocity(sf::Vector2f(100.0f, 0.0f));
							setVelocity(sf::Vector2f(-velocity.x, 0.0f));
							setVelocity(sf::Vector2f(velocity.x * 1.8f, velocity.y));
						}
						else
						{
							setVelocity(sf::Vector2f(-1.0f, 100.0f));
							setVelocity(sf::Vector2f(velocity.x, velocity.y * 1.8f));
						}
						
						bullet->setAliveStatus(false);
						bullet->setFiredStatus(false);
						asteroidHit = true;
					}
				}
			}
			else if (asteroidType == 1)
			{
				if ((asteroid.getPosition().x - 30 < bullet->getPosition().x + bullet->GetBallRadius()) && (asteroid.getPosition().x + 30 > bullet->getPosition().x - bullet->GetBallRadius()))
				{
					if ((asteroid.getPosition().y - 20 < bullet->getPosition().y + bullet->GetBallRadius()) && (asteroid.getPosition().y + 100 > bullet->getPosition().y))
					{
						float tmpX, tmpY;
						tmpX = asteroid.getPosition().x;
						tmpY = asteroid.getPosition().y;

						//std::cout << asteroidID << "\n";
						gpGame->createNewAsteroid(asteroidID);
						asteroidType = 2;
						Init(asteroidID);
						asteroid.setPosition(tmpX - 40, tmpY);
						gpGame->increaseTotalPoints(100 * gpGame->getLevelNumber());
						
						if (goingSideWays)
						{
							setVelocity(sf::Vector2f(100.0f, 0.0f));
							setVelocity(sf::Vector2f(-velocity.x, 0.0f));
							setVelocity(sf::Vector2f(velocity.x * 2.3f, velocity.y));
						}
						else
						{
							setVelocity(sf::Vector2f(-1.0f, 100.0f));
							setVelocity(sf::Vector2f(velocity.x, velocity.y * 2.3f));
						}

						bullet->setAliveStatus(false);
						bullet->setFiredStatus(false);
						asteroidHit = true;
					}
				}
			}
			else if (asteroidType == 2)
			{
				if ((asteroid.getPosition().x - 30 < bullet->getPosition().x + bullet->GetBallRadius()) && (asteroid.getPosition().x + 30 > bullet->getPosition().x - bullet->GetBallRadius()))
				{
					if ((asteroid.getPosition().y - 5 < bullet->getPosition().y + bullet->GetBallRadius()) && (asteroid.getPosition().y + 80 > bullet->getPosition().y))
					{
						//std::cout << asteroidID << "\n";
						asteroidType = 3;
						Init(asteroidID);
						gpGame->increaseTotalPoints(150 * gpGame->getLevelNumber());

						bullet->setAliveStatus(false);
						bullet->setFiredStatus(false);
						asteroidHit = true;

						goingSideWays = false;
						setAliveStatus(false);
					}
				}
			}
		}
	}

	return asteroidHit;
}