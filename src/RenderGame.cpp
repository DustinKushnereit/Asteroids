#include "renderGame.h"
#include "SFML\System\Vector2.hpp"

RenderGame::RenderGame()
{
	mWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);
	mWindow.setFramerateLimit(60);
}

RenderGame::~RenderGame(){}

sf::CircleShape RenderGame::addShape(int red, int blue, int green)
{
	sf::CircleShape circle(50);
	circle.setOrigin(sf::Vector2f(50, 50));
	circle.setFillColor(sf::Color(red, blue, green));

	mCircle = circle;
	return mCircle;
}
//int Lerp(int a, int b, double a_time)
//{
//	return (1 - a_time)* a + a_time * b;
//}
//ClientState Lerp(ClientState a, ClientState b, double a_time)
//{
//	ClientState ls;
//	ls.x = Lerp(a.x, b.x, a_time);
//	ls.y = Lerp(a.y, b.y, a_time);
//}
//
//void RenderGame::update(double a_deltaT)
//{
//	updateRakNet();
//	getUserState();
//
//	ClientState cs;
//	cs.x = 10;
//	cs.y = 11;
//
//	ClientState csFromServer;
//	csFromServer.x = 11;
//	csFromServer.y = 15;
//
//
//	//cap delta time to 30
//
//	/*for (unsigned int i = 1; i <= mCircleVector.size(); i++)
//	{
//		mCircleVector[i - 1].setPosition(static_cast<sf::Vector2f>(pos));
//	}*/
//}
sf::Vector2f RenderGame::update(sf::CircleShape circle, int x, int y)
{
	sf::Vector2f vec;
	vec.x = x;
	vec.y = y;
	return vec;
}

void RenderGame::draw(sf::CircleShape circle)
{
	mWindow.draw(circle);
}

void RenderGame::clearScreen()
{
	mWindow.clear(sf::Color::White);
}

void RenderGame::displayScreen()
{
	mWindow.display();
}