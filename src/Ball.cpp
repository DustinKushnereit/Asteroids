#include "ball.h"
#include "gameObject.h"

Ball::Ball(float screenWidth, float screenHeight)
{
	screenX = screenWidth;
	screenY = screenHeight;
	mIsAlive = true;
}

void Ball::Init()
{
	ballRadius = 12;

	mIsAlive = true;

	gameBall.setRadius(ballRadius);
	gameBall.setOrigin(ballRadius / 2, ballRadius / 2);

	sf::Vector2f origin = gameBall.getOrigin();
	velocity = sf::Vector2f(0.08f, 0.1f);

	gameBall.setFillColor(sf::Color(255, 0, 255));
	gameBall.setPosition(screenX / 2, screenY / 3);

	//wallBuffer.loadFromFile("../resources/ball.wav");
	wallSound.setVolume(20);
	wallSound.setBuffer(wallBuffer);
}

void Ball::Update(sf::Time& deltaTime)
{
	position = gameBall.getPosition();
	gameBall.move(velocity.x * deltaTime.asSeconds(), velocity.y * deltaTime.asSeconds());
	CheckCollisions();
}

void Ball::Draw(sf::RenderWindow *window)
{
	window->draw(gameBall);
}

void Ball::setFillColor(sf::Color newColor)
{
	gameBall.setFillColor(newColor);
}

void Ball::setOutlineColor(sf::Color newColor)
{
	gameBall.setOutlineColor(newColor);
}

void Ball::CheckCollisions()
{

	if (gameBall.getPosition().y - ballRadius <= 0)
	{
		velocity.y *= -1;
		gameBall.setPosition(gameBall.getPosition().x, ballRadius);
		wallSound.play();
	}

	if (gameBall.getPosition().y - ballRadius >= screenY)
	{
		velocity.y *= -1;
		gameBall.setPosition(gameBall.getPosition().x, screenY - ballRadius);
		wallSound.play();
	}

	if (gameBall.getPosition().x <= ballRadius)
	{
		velocity.x *= -1;
		gameBall.setPosition(ballRadius, gameBall.getPosition().y);
		wallSound.play();
	}

	if (gameBall.getPosition().x >= screenX - ballRadius)
	{
		velocity.x *= -1;
		gameBall.setPosition(screenX - ballRadius, gameBall.getPosition().y);
		wallSound.play();
	}
}

float Ball::GetBallRadius()
{
	return ballRadius;
}