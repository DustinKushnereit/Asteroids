#include "paddle.h"
#include "gameObject.h"
#include "Game.h"
#include "Bullet.h"

Paddle::Paddle(float screenWidth, float screenHeight)
{
	screenX = screenWidth;
	screenY = 800;
	FKeyPressed = false;
}

void Paddle::Init(bool leftPaddle)
{
	mLeftPaddle = leftPaddle;

	mAliveStatus = true;

	mPauseTimer = 0;
	mAliveTimer = 0;

	paddleWidth = 20;
	paddleHeight = 20;

	velocity.x = 0.1f;
	velocity.y = 0.1f;

	paddleRadius = 20.0f;
	//mPaddle.setRadius(paddleRadius);
	mPaddle.setOrigin(15, 5);

	mPaddle.setPointCount(4);
	mPaddle.setPoint(0, sf::Vector2f(0, 0));
	mPaddle.setPoint(1, sf::Vector2f(40, 0));
	mPaddle.setPoint(2, sf::Vector2f(20, -40));
	mPaddle.setPoint(3, sf::Vector2f(1, 0));

	mPaddle.setRotation(0.0f);
	mPaddle.setOutlineThickness(3);

	if (mLeftPaddle)
	{
		mPaddle.setOutlineColor(sf::Color::Cyan);
		mPaddle.setFillColor(sf::Color::Black);
		mPaddle.setPosition(550, 500);
	}
	else
	{
		mPaddle.setOutlineColor(sf::Color::Red);
		mPaddle.setFillColor(sf::Color::Black);
		mPaddle.setPosition(600, 500);
	}
}


void Paddle::Update(sf::Time& time)
{
	position = mPaddle.getPosition();
	gpGame->setPlayerOnePos(position);
	gpGame->setPlayerOneAngle(mPaddle.getRotation());

	if (mLeftPaddle)
	{
		if (mAliveStatus)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) //Up
			{
				mPaddle.move(0 * time.asSeconds(), -250 * time.asSeconds());
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) //Down
			{
				mPaddle.move(0 * time.asSeconds(), 250 * time.asSeconds());
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) //Left
			{
				mPaddle.move(-250 * time.asSeconds(), 0 * time.asSeconds());
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) //Right
			{
				mPaddle.move(250 * time.asSeconds(), 0 * time.asSeconds());
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) //Rotation
			{
				mPaddle.rotate(-0.3f);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) //Rotation
			{
				mPaddle.rotate(0.3f);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && !FKeyPressed) //Shoot
			{
				gpGame->setFiredStatusPlayerOne(true);				
				FKeyPressed = true;

				int tmp = gpGame->getBulletIDPlayerOne();
				if (tmp >= 9)
				{
					tmp = 0;
					gpGame->setBulletIDPlayerOne(tmp);
				}			
				gpGame->setBulletIDPlayerOne(tmp++);
			}
		}
	}
	else
	{
		if (mAliveStatus)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //Up
			{
				mPaddle.move(0 * time.asSeconds(), -250 * time.asSeconds());
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) //Down
			{
				mPaddle.move(0 * time.asSeconds(), 250 * time.asSeconds());
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //Left
			{
				mPaddle.move(-250 * time.asSeconds(), 0 * time.asSeconds());
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //Right
			{
				mPaddle.move(250 * time.asSeconds(), 0 * time.asSeconds());
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))) //Rotation
			{
				mPaddle.rotate(-0.3f);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))) //Rotation
			{
				mPaddle.rotate(0.3f);
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && !FKeyPressed) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4) && !FKeyPressed)) //Shoot
			{
				gpGame->setFiredStatusPlayerOne(true);
				FKeyPressed = true;

				int tmp = gpGame->getBulletIDPlayerOne();
				if (tmp >= 9)
				{
					tmp = 0;
					gpGame->setBulletIDPlayerOne(tmp);
				}
				gpGame->setBulletIDPlayerOne(tmp++);
				/*gpGame->setFiredStatusPlayerTwo(true);
				FKeyPressed = true;

				int tmp = gpGame->getBulletIDPlayerTwo();
				if (tmp >= 9)
				{
					tmp = 0;
					gpGame->setBulletIDPlayerTwo(tmp);
				}
				gpGame->setBulletIDPlayerTwo(tmp++);*/
			}
		}
	}

	if (mPauseTimer >= 800)
	{
		FKeyPressed = false;
		mPauseTimer = 0;
	}
	else
		mPauseTimer++;

	respawnTimer(time);
}

void Paddle::respawnTimer(sf::Time& time)
{
	if (!mAliveStatus)
	{
		if (mAliveTimer >= 1000)
		{
			mAliveStatus = true;
			mAliveTimer = 0;
		}
		else
			mAliveTimer++;
	}
}

void Paddle::Draw(sf::RenderWindow *window)
{
	if (mAliveStatus)
		window->draw(mPaddle);
}

bool Paddle::CheckCollisions()
{
	bool didBallBounce = false;

	if (mPaddle.getPosition().y <= 60.0f)
		mPaddle.setPosition(sf::Vector2f(position.x, 60.1f));

	if (mPaddle.getPosition().y >= 710.0f)
		mPaddle.setPosition(sf::Vector2f(position.x, 709.9f));

	if (mPaddle.getPosition().x <= 20.0f)
		mPaddle.setPosition(sf::Vector2f(20.1f, mPaddle.getPosition().y));

	if (mPaddle.getPosition().x >= 1170.0f)
		mPaddle.setPosition(sf::Vector2f(1169.9f, mPaddle.getPosition().y));

	return didBallBounce;
}

bool Paddle::CheckCollisionsAsteroids(Asteroid* asteroid, int player)
{
	bool didBallBounce = false;

	if (mAliveStatus)
	{
		if (asteroid->getAliveStatus())
		{
			if (asteroid->getAsteroidType() == 0)
			{
				if ((asteroid->getPosition().x - 60 < mPaddle.getPosition().x + paddleWidth) && (asteroid->getPosition().x + 100 > mPaddle.getPosition().x - paddleWidth))
				{
					if ((asteroid->getPosition().y - 40 < mPaddle.getPosition().y + paddleHeight) && (asteroid->getPosition().y + 80 > mPaddle.getPosition().y))
					{
						setAliveStatus(false);

						float tmpX, tmpY;
						tmpX = asteroid->getPosition().x;
						tmpY = asteroid->getPosition().y;
						gpGame->createNewAsteroid(asteroid->getAsteroidID());
						asteroid->setAsteroidType(1);
						asteroid->Init(asteroid->getAsteroidID());
						asteroid->setPosition((int)tmpX - 40, (int)tmpY);

						if (asteroid->getGoingSideWays())
						{
							asteroid->setVelocity(sf::Vector2f(100.0f, 0.0f));
							asteroid->setVelocity(sf::Vector2f(-asteroid->getVelocity().x, 0.0f));
							asteroid->setVelocity(sf::Vector2f(asteroid->getVelocity().x * 1.8f, asteroid->getVelocity().y));
						}
						else
						{
							asteroid->setVelocity(sf::Vector2f(-1.0f, 100.0f));
							asteroid->setVelocity(sf::Vector2f(asteroid->getVelocity().x, asteroid->getVelocity().y * 1.8f));
						}

						if (mLeftPaddle)
							gpGame->reducePlayerOneLives();

						else
							gpGame->reducePlayerTwoLives();

						didBallBounce = true;
					}
				}
			}
			else if (asteroid->getAsteroidType() == 1)
			{
				if ((asteroid->getPosition().x - 30 < mPaddle.getPosition().x + paddleWidth) && (asteroid->getPosition().x + 30 > mPaddle.getPosition().x - paddleWidth))
				{
					if ((asteroid->getPosition().y - 20 < mPaddle.getPosition().y + paddleHeight) && (asteroid->getPosition().y + 100 > mPaddle.getPosition().y))
					{
						setAliveStatus(false);

						float tmpX, tmpY;
						tmpX = asteroid->getPosition().x;
						tmpY = asteroid->getPosition().y;
						gpGame->createNewAsteroid(asteroid->getAsteroidID());
						asteroid->setAsteroidType(2);
						asteroid->Init(asteroid->getAsteroidID());
						asteroid->setPosition((int)tmpX - 40, (int)tmpY);

						if (asteroid->getGoingSideWays())
						{
							asteroid->setVelocity(sf::Vector2f(100.0f, 0.0f));
							asteroid->setVelocity(sf::Vector2f(-asteroid->getVelocity().x, 0.0f));
							asteroid->setVelocity(sf::Vector2f(asteroid->getVelocity().x * 2.3f, asteroid->getVelocity().y));
						}
						else
						{
							asteroid->setVelocity(sf::Vector2f(-1.0f, 100.0f));
							asteroid->setVelocity(sf::Vector2f(asteroid->getVelocity().x, asteroid->getVelocity().y * 2.3f));
						}

						if (player == 1)
							gpGame->reducePlayerOneLives();

						if (player == 2)
							gpGame->reducePlayerTwoLives();

						didBallBounce = true;
					}
				}
			}
			else if (asteroid->getAsteroidType() == 2)
			{
				if ((asteroid->getPosition().x - 30 < mPaddle.getPosition().x + paddleWidth) && (asteroid->getPosition().x + 30 > mPaddle.getPosition().x - paddleWidth))
				{
					if ((asteroid->getPosition().y - 5 < mPaddle.getPosition().y + paddleHeight) && (asteroid->getPosition().y + 80 > mPaddle.getPosition().y ))
					{
						setAliveStatus(false);
						asteroid->setAsteroidType(3);
						asteroid->Init(asteroid->getAsteroidID());
						asteroid->setAliveStatus(false);
						asteroid->setGoingSideWays(false);

						if (mLeftPaddle)
							gpGame->reducePlayerOneLives();

						else
							gpGame->reducePlayerTwoLives();

						didBallBounce = true;
					}
				}
			}
		}
	}

	return didBallBounce;
}

void Paddle::resetPaddleSize()
{
	paddleHeight = 100;
}

float Paddle::getPositionX()
{
	return mPaddle.getPosition().x;
}

float Paddle::getPositionY()
{
	return mPaddle.getPosition().y;
}

void Paddle::setPosition(float x, float y)
{
	mPaddle.setPosition(x, y);
}