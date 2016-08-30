#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window.hpp>
#include <vector>

class RenderGame 
{
public:
	RenderGame();
	~RenderGame();
	sf::CircleShape addShape(int red, int blue, int green);
	sf::Vector2f update(sf::CircleShape circle, int x, int y);
	void draw(sf::CircleShape circle);
	void clearScreen();
	void displayScreen();

	inline sf::CircleShape& getCircle(){ return mCircle; };
	inline sf::RenderWindow& getWindow(){ return mWindow; };

private:
	const int WINDOW_HEIGHT = 600;
	const int WINDOW_WIDTH = 800;
	const std::string WINDOW_NAME = "Game Window";

	sf::CircleShape mCircle;
	sf::RenderWindow mWindow;
	sf::Event mEvent;
};