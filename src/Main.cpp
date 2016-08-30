#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ball.h"
#include "gameObject.h"
#include "paddle.h"
#include "Game.h"
#include <vector>

int main(int argc, char** argv)
{
	float const screenWidth = 1200.0f;
	float const screenHeight = 700.0f;

	sf::Clock clock;

	//gpGame = new Game;

	gpGame = new Game(screenHeight, screenWidth);

	gpGame->Init();

	if (argc == 4) // clientPort serverIP serverPort
	{
		const int   clientPort = atoi(argv[1]);
		const char* serverIP = argv[2];
		const int   serverPort = atoi(argv[3]);

		gpGame->StartClient(serverIP, serverPort, clientPort);
	}
	else
	{
		Client::PrintUsage();
		return 0;
	}

	gpGame->Update(clock);

	return 0;
}