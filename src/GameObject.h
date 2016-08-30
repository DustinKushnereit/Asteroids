#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class GameObject
{
public:

	float screenX;
	float screenY;

	sf::Vector2f Position() const { return position; }
	void Position(sf::Vector2f val) { position = val; }

	sf::Vector2f Velocity() const { return velocity; }
	void Velocity(sf::Vector2f val) { velocity = val; }

	virtual void Init();
	virtual void Update();
	virtual void Draw(sf::RenderWindow *window);
	//virtual void CheckCollisions() {};

protected:
	sf::Vector2f position;
	sf::Vector2f velocity;

};