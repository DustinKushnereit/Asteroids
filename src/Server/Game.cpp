#include "Game.h"
#include <time.h>

Game::Game()
{
	player1 = nullptr;
	player2 = nullptr;
	gameOn = false;

	time(&lastTime);
}

Game::~Game()
{

}

void Game::connectPlayer(RakNet::RakNetGUID guid, float posX, float posY)
{
	if (player1 == nullptr)
	{
		startPlayer1(guid, posX, posY);
	}
	else
	{
		startPlayer2(guid, posX, posY);
	}
}

void Game::disconnectPlayer(RakNet::RakNetGUID guid)
{
	if (player1 != nullptr && player1->getGUID() == guid)
	{
		player1.reset();
		
		gameOn = false;
	}
	else if (player2 != nullptr && player2->getGUID() == guid)
	{
		player2.reset();

		gameOn = false;
	}
}

void Game::startPlayer1(RakNet::RakNetGUID guid, float posX, float posY)
{
	player1 = std::make_unique<Player>(guid);
	player1->setPosition(posX, posY);

	if (player2 != nullptr)
	{
		gameOn = true;
	}
}

void Game::startPlayer2(RakNet::RakNetGUID guid, float posX, float posY)
{
	player2 = std::make_unique<Player>(guid);
	player2->setPosition(posX, posY);

	if (player1 != nullptr)
	{
		gameOn = true;
	}
}

bool Game::isGameOn()
{
	return gameOn;
}

void Game::update()
{
	if (gameOn)
	{
		printf("Player 1 Position %f %f\n", player1->getX(), player1->getY());
		printf("Player 2 Position %f %f\n", player2->getX(), player2->getY());
	}
}

void Game::updatePlayer(RakNet::RakNetGUID playerGuid, PlayerVariables newPos)
{
	if (player1->getGUID() == playerGuid)
	{
		player1->setPosition(newPos.pos);
		player1->setRotation(newPos.rotation);
	}
	else if (player2->getGUID() == playerGuid)
	{
		player2->setPosition(newPos.pos);
		player2->setRotation(newPos.rotation);
	}
}

RakNet::RakNetGUID Game::getPlayer1GUID()
{
	return player1->getGUID();
}
RakNet::RakNetGUID Game::getPlayer2GUID()
{
	return player2->getGUID();
}
Position Game::getPlayer1Position()
{
	return player1->getPosition();
}
Position Game::getPlayer2Position()
{
	return player2->getPosition();
}
float Game::getPlayer1Rotation()
{
	return player1->getRotation();
}
float Game::getPlayer2rotation()
{
	return player2->getRotation();
}