#include "Game.h"
#include <string>

Game* gpGame = NULL;

Game::Game(float screenHeight, float screenWidth)
{
	mRenderWindow = new sf::RenderWindow(sf::VideoMode((unsigned int)screenWidth, (unsigned int)screenHeight), "Asteroids");
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	mNumberOfBulletsPlayerOne = 0;
	mNumberOfBulletsPlayerTwo = 0;
	mBulletIDPlayerOne = 0;
	mBulletIDPlayerTwo = 0;
	levelNumber = 1;
	mNumberOfAsteroids = 30;
	totalPoints = 0;

	otherPlayerConnected = false;
	playerOneHasFiredBullet = false;
	LKeyPressed = false;
	gameOver = false;
	isTitleOn = true;
}

Game::~Game()
{

}

void Game::StartClient(const char* serverIP, int serverPort, int clientPort)
{
	client.startClient(serverIP, serverPort, clientPort);
}

void Game::Init()
{
	playerOneShip = new Paddle(mScreenWidth, mScreenHeight);
	playerTwoShip = new Paddle(mScreenWidth, mScreenHeight);
	
	createAsteroids();
	createBullets();
	
	playerOneHealth = 3;
	playerTwoHealth = 3;
	
	font.loadFromFile("../resources/times.ttf");

	//music.openFromFile("Content/backgroundMusic.wav");
	//music.setPosition(0, 1, 10);
	//music.setPitch(2);
	//music.setVolume(100);
	//music.setLoop(true);
	//music.play();

	playerOneLivesLabel.setFont(font);
	playerOneLivesLabel.setCharacterSize(40);
	playerOneLivesLabel.setStyle(sf::Text::Bold);
	playerOneLivesLabel.setColor(sf::Color::Cyan);
	playerOneLivesLabel.setPosition(150, 5);
	playerOneLivesLabel.setString("P1 Lives " + std::to_string(playerOneHealth) + " Wins ");

	playerTwoLivesLabel.setFont(font);
	playerTwoLivesLabel.setCharacterSize(40);
	playerTwoLivesLabel.setStyle(sf::Text::Bold);
	playerTwoLivesLabel.setColor(sf::Color::Red);
	playerTwoLivesLabel.setPosition(750, 5);
	playerTwoLivesLabel.setString("P2 Lives " + std::to_string(playerTwoHealth) + " Wins ");

	titleLabel.setFont(font);
	titleLabel.setCharacterSize(100);
	titleLabel.setStyle(sf::Text::Bold);
	titleLabel.setColor(sf::Color::Red);
	titleLabel.setPosition(400, 150);
	titleLabel.setString("Asteroids");

	spaceLabel.setFont(font);
	spaceLabel.setCharacterSize(60);
	spaceLabel.setStyle(sf::Text::Bold);
	spaceLabel.setColor(sf::Color(128, 128, 128));
	spaceLabel.setPosition(450, 450);
	spaceLabel.setString("Connecting");

	winLabel.setFont(font);
	winLabel.setCharacterSize(100);
	winLabel.setStyle(sf::Text::Bold);
	winLabel.setColor(sf::Color::White);
	winLabel.setPosition(mScreenWidth / 2 - 130, mScreenHeight / 3);
	winLabel.setString("You Win!");

	//startBuffer.loadFromFile("Content/startBall.wav");
	//startSound.setVolume(80);
	startSound.setBuffer(startBuffer);

	//lossBuffer.loadFromFile("Content/enemyScore.wav");
	//lossSound.setVolume(80);
	lossSound.setBuffer(lossBuffer);

	isPlaying = false;
}

void Game::Update(sf::Clock& clock)
{
	while (mRenderWindow->isOpen())
	{
		client.waitForPackets();

		if ( isTitleOn )
		{
			otherPlayerConnected = client.isGameStarted();
			if (otherPlayerConnected)
			{
				playerOneShip->Init(client.isPlayer1());
				playerTwoShip->Init(!client.isPlayer1());

				isTitleOn = false;
			}

			mRenderWindow->draw(titleLabel);
			mRenderWindow->draw(spaceLabel);
		}

		deltaTime = clock.restart();

		mRenderWindow->display();
		mRenderWindow->clear();
			
		if (!isTitleOn)
		{
			LKeyPressed = false;

			if (gameOver)
			{
				playerTwoLivesLabel.setCharacterSize(80);
				playerTwoLivesLabel.setString("Game Over!\nPress Space!\n\nYour Score: " + std::to_string(totalPoints));
				playerTwoLivesLabel.setPosition(mScreenWidth / 4, mScreenHeight / 5);
				mRenderWindow->draw(playerTwoLivesLabel);
			}
			else
			{
				playerOneShip->Draw(mRenderWindow);
				playerTwoShip->Draw(mRenderWindow);

				for (int i = 0; i < mNumberOfAsteroids; i++)
					mAsteroidVector[i]->Draw(mRenderWindow);

				for (int i = 0; i < mNumberOfBulletsPlayerOne; i++)
					mBulletVectorPlayerOne[i]->Draw(mRenderWindow);

				for (int i = 0; i < mNumberOfBulletsPlayerTwo; i++)
					mBulletVectorPlayerTwo[i]->Draw(mRenderWindow);

				playerTwoLivesLabel.setCharacterSize(50);
				playerTwoLivesLabel.setString("Level:" + std::to_string(levelNumber));
				playerTwoLivesLabel.setPosition(10, 5);
				mRenderWindow->draw(playerTwoLivesLabel);

				playerTwoLivesLabel.setString("Points: " + std::to_string(totalPoints));
				playerTwoLivesLabel.setPosition(800, 5);
				mRenderWindow->draw(playerTwoLivesLabel);

				for (int i = 0; i < playerOneHealth; i++)
				{
					sf::ConvexShape playerOneShipLives;
					playerOneShipLives.setPointCount(4);
					playerOneShipLives.setPoint(0, sf::Vector2f(0, 0));
					playerOneShipLives.setPoint(1, sf::Vector2f(40, 0));
					playerOneShipLives.setPoint(2, sf::Vector2f(20, -40));
					playerOneShipLives.setPoint(3, sf::Vector2f(1, 0));
					playerOneShipLives.setOutlineColor(sf::Color::Cyan);
					playerOneShipLives.setFillColor(sf::Color::Black);
					playerOneShipLives.setPosition((float)i * 55 + 20, mScreenHeight - 20);
					playerOneShipLives.setOutlineThickness(3);

					mRenderWindow->draw(playerOneShipLives);
				}

				for (int i = 0; i < playerTwoHealth; i++)
				{
					sf::ConvexShape playerTwoShipLives;
					playerTwoShipLives.setPointCount(4);
					playerTwoShipLives.setPoint(0, sf::Vector2f(0, 0));
					playerTwoShipLives.setPoint(1, sf::Vector2f(40, 0));
					playerTwoShipLives.setPoint(2, sf::Vector2f(20, -40));
					playerTwoShipLives.setPoint(3, sf::Vector2f(1, 0));
					playerTwoShipLives.setOutlineColor(sf::Color::Red);
					playerTwoShipLives.setFillColor(sf::Color::Black);
					playerTwoShipLives.setPosition((float)i * 55 + 1030, mScreenHeight - 20);
					playerTwoShipLives.setOutlineThickness(3);

					mRenderWindow->draw(playerTwoShipLives);
				}
			}
		}

		sf::Event event;
		while (mRenderWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				client.disconnectFromServer();
				mRenderWindow->close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				client.disconnectFromServer();
				mRenderWindow->close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))//force start game
			{
				playerOneShip->Init(true);
				playerTwoShip->Init(true);
				client.setGameStarted(true);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))//restart Game
			{
				if (gameOver)
				{
					playerOneShip->Init(client.isPlayer1());
					playerTwoShip->Init(!client.isPlayer1());
					client.setGameStarted(true);
					gameOver = false;
					playerOneHealth = 3;
					playerTwoHealth = 3;
					levelNumber = 1;
					totalPoints = 0;

					for (int i = 0; i < mNumberOfAsteroids; i++)
						mAsteroidVector.pop_back();

					createAsteroids();
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))//game over
			{
				playerOneHealth = -1;
				playerTwoHealth = -1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && !LKeyPressed)//level increase
			{
				//levelNumber++;
				for (int i = 0; i < mNumberOfAsteroids; i++)
				{
					mAsteroidVector[i]->setAsteroidType(3);
					mAsteroidVector[i]->setAliveStatus(false);
				}
				LKeyPressed = true;
			}

		}

		if (!isPlaying && otherPlayerConnected)
		{
			for (int i = 0; i < mNumberOfAsteroids; i++)
			{
				mAsteroidVector[i]->setVelocity(sf::Vector2f(-0.0f, 100.0f));

				if (mAsteroidVector[i]->getAsteroidType() == 1)
					mAsteroidVector[i]->setVelocity(sf::Vector2f(mAsteroidVector[i]->getVelocity().x, mAsteroidVector[i]->getVelocity().y * 1.8f));
				else if (mAsteroidVector[i]->getAsteroidType() == 2)
					mAsteroidVector[i]->setVelocity(sf::Vector2f(mAsteroidVector[i]->getVelocity().x, mAsteroidVector[i]->getVelocity().y * 2.3f));
			}

			for (int i = 0; i < mNumberOfBulletsPlayerOne; i++)
				mBulletVectorPlayerOne[i]->setVelocity(sf::Vector2f(-0.0f, 100.0f));

			for (int i = 0; i < mNumberOfBulletsPlayerTwo; i++)
				mBulletVectorPlayerTwo[i]->setVelocity(sf::Vector2f(-0.0f, 100.0f));
			
			isPlaying = true;
		}

		playerOneShip->CheckCollisions();

		playerTwoShip->CheckCollisions();

		playerOneShip->Update(deltaTime);
		playerTwoShip->respawnTimer(deltaTime);
		playerTwoShip->setPosition(client.getOtherPosition().pos.x, client.getOtherPosition().pos.y);
		playerTwoShip->setRotation(client.getOtherPosition().rotation);
		client.update(playerOneShip->getPositionX(), playerOneShip->getPositionY(), playerOneShip->getRotation());

		for (int i = 0; i < mNumberOfAsteroids; i++)
			playerOneShip->CheckCollisionsAsteroids(mAsteroidVector[i], 1);

		for (int i = 0; i < mNumberOfAsteroids; i++)
			playerTwoShip->CheckCollisionsAsteroids(mAsteroidVector[i], 2);
		
		for (int i = 0; i < mNumberOfBulletsPlayerOne; i++)
		{
			for (int a = 0; a < mNumberOfAsteroids; a++)
			{
				mAsteroidVector[a]->CheckCollisionsAsteroids(mBulletVectorPlayerOne[i]);
			}
		}

		for (int i = 0; i < mNumberOfBulletsPlayerTwo; i++)
		{
			for (int a = 0; a < mNumberOfAsteroids; a++)
			{
				mAsteroidVector[a]->CheckCollisionsAsteroids(mBulletVectorPlayerTwo[i]);
			}
		}

		if (isPlaying)
		{
			for (int i = 0; i < mNumberOfAsteroids; i++)
				mAsteroidVector[i]->Update(deltaTime);

			for (int i = 0; i < mNumberOfBulletsPlayerOne; i++)
				mBulletVectorPlayerOne[i]->Update(deltaTime);

			for (int i = 0; i < mNumberOfBulletsPlayerTwo; i++)
				mBulletVectorPlayerTwo[i]->Update(deltaTime);

			if (playerOneHasFiredBullet)
			{
				if (!mBulletVectorPlayerOne[mBulletIDPlayerOne]->getAliveStatus())
				{
					if (!mBulletVectorPlayerOne[mBulletIDPlayerOne]->getFiredStatus())
					{
						PlayerOneBulletsFire();
						playerOneHasFiredBullet = false;
					}
				}
			}

			if (playerTwoHasFiredBullet)
			{
				if (!mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->getAliveStatus())
				{
					if (!mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->getFiredStatus())
					{
						PlayerTwoBulletsFire();
						playerTwoHasFiredBullet = false;
					}
				}
			}

			if (playerOneHealth < 0)
			{
				gameOver = true;
				isPlaying = false;
				playerOneHealth = 0;
				//client.sendGameOverMessage();
			}

			if (playerTwoHealth < 0)
			{
				gameOver = true;
				isPlaying = false;
				playerTwoHealth = 0;
				//client.sendGameOverMessage();
			}

			if (checkGameLevel())
			{
				levelNumber++;

				for (int i = 0; i < mNumberOfAsteroids; i++)
					mAsteroidVector.pop_back();

				createAsteroids();

				for (int i = 0; i < mNumberOfAsteroids; i++)
				{
					if (levelNumber > 2)
					{
						if (i > 3)
						{
							mAsteroidVector[i]->setVelocity(sf::Vector2f(100.0f, 0.0f));
							mAsteroidVector[i]->setVelocity(sf::Vector2f(-mAsteroidVector[i]->getVelocity().x, 0.0f));
							mAsteroidVector[i]->setGoingSideWays(true);

							if (mAsteroidVector[i]->getAsteroidType() == 1)
								mAsteroidVector[i]->setVelocity(sf::Vector2f(mAsteroidVector[i]->getVelocity().x  * 1.8f, mAsteroidVector[i]->getVelocity().y));
							else if (mAsteroidVector[i]->getAsteroidType() == 2)
								mAsteroidVector[i]->setVelocity(sf::Vector2f(mAsteroidVector[i]->getVelocity().x * 2.3f, mAsteroidVector[i]->getVelocity().y));
						}
						else
						{
							mAsteroidVector[i]->setVelocity(sf::Vector2f(-0.0f, 100.0f));

							if (mAsteroidVector[i]->getAsteroidType() == 1)
								mAsteroidVector[i]->setVelocity(sf::Vector2f(mAsteroidVector[i]->getVelocity().x, mAsteroidVector[i]->getVelocity().y * 1.8f));
							else if (mAsteroidVector[i]->getAsteroidType() == 2)
								mAsteroidVector[i]->setVelocity(sf::Vector2f(mAsteroidVector[i]->getVelocity().x, mAsteroidVector[i]->getVelocity().y * 2.3f));
						}
					}
					else
					{
						mAsteroidVector[i]->setVelocity(sf::Vector2f(-0.0f, 100.0f));

						if (mAsteroidVector[i]->getAsteroidType() == 1)
							mAsteroidVector[i]->setVelocity(sf::Vector2f(mAsteroidVector[i]->getVelocity().x, mAsteroidVector[i]->getVelocity().y * 1.8f));
						else if (mAsteroidVector[i]->getAsteroidType() == 2)
							mAsteroidVector[i]->setVelocity(sf::Vector2f(mAsteroidVector[i]->getVelocity().x, mAsteroidVector[i]->getVelocity().y * 2.3f));
					}
				}
			}

		}
	}
}

void Game::createAsteroids()
{
	for (int i = 0; i < mNumberOfAsteroids; i++)
		mAsteroidVector.push_back(new Asteroid(mScreenWidth, mScreenHeight));

	for (int i = 0; i < mNumberOfAsteroids; i++)
	{
		if (levelNumber == 1)
		{
			if (i == 1)
				mAsteroidVector[i]->setAsteroidType(0);
			else if (i == 3)
				mAsteroidVector[i]->setAsteroidType(1);
			else if (i == 0 || i == 2)
				mAsteroidVector[i]->setAsteroidType(2);
			else
				mAsteroidVector[i]->setAsteroidType(3);

			mAsteroidVector[i]->Init(i);
			mAsteroidVector[i]->setFillColor(sf::Color::Black);
			mAsteroidVector[i]->setOutlineColor(sf::Color::White);

			if (i <= 3)
			{
				mAsteroidVector[i]->setPosition(i * 300 + 100, i * 10 - 100);
				mAsteroidVector[i]->setAliveStatus(true);
			}
			else
			{
				mAsteroidVector[i]->setPosition(100, -100);
				mAsteroidVector[i]->setAliveStatus(false);
			}
		}
		else if (levelNumber == 2)
		{
			if (i <= 2)
				mAsteroidVector[i]->setAsteroidType(0);
			else if (i == 3)
				mAsteroidVector[i]->setAsteroidType(1);
			else
				mAsteroidVector[i]->setAsteroidType(3);

			mAsteroidVector[i]->Init(i);
			mAsteroidVector[i]->setFillColor(sf::Color::Black);
			mAsteroidVector[i]->setOutlineColor(sf::Color::White);

			if (i <= 3)
			{
				mAsteroidVector[i]->setPosition(i * 300 + 100, i * 10 - 100);
				mAsteroidVector[i]->setAliveStatus(true);
			}
			else
			{
				mAsteroidVector[i]->setPosition(100, -100);
				mAsteroidVector[i]->setAliveStatus(false);
			}
		}
		else if (levelNumber == 3)
		{
			if (i <= 3)
				mAsteroidVector[i]->setAsteroidType(0);
			else if (i > 3 && i <= 6)
				mAsteroidVector[i]->setAsteroidType(1);
			else
				mAsteroidVector[i]->setAsteroidType(3);

			mAsteroidVector[i]->Init(i);
			mAsteroidVector[i]->setFillColor(sf::Color::Black);
			mAsteroidVector[i]->setOutlineColor(sf::Color::White);

			if (i <= 3)
			{
				mAsteroidVector[i]->setPosition(i * 300 + 100, i * 10 - 100);
				mAsteroidVector[i]->setAliveStatus(true);
			}
			else if (i > 3 && i <= 6)
			{
				mAsteroidVector[i]->setPosition(-100, (i * 150) - 450);
				mAsteroidVector[i]->setAliveStatus(true);
			}
			else
			{
				mAsteroidVector[i]->setPosition(100, -100);
				mAsteroidVector[i]->setAliveStatus(false);
			}
		}
		else
		{
			if (i <= 7)
				mAsteroidVector[i]->setAsteroidType(0);
			else
				mAsteroidVector[i]->setAsteroidType(3);

			mAsteroidVector[i]->Init(i);
			mAsteroidVector[i]->setFillColor(sf::Color::Black);
			mAsteroidVector[i]->setOutlineColor(sf::Color::White);

			if (i <= 3)
			{
				mAsteroidVector[i]->setPosition(i * 300 + 100, i * 10 - 100);
				mAsteroidVector[i]->setAliveStatus(true);
			}
			else if (i > 3 && i <= 7)
			{
				mAsteroidVector[i]->setPosition(-100, (i * 150) - 450);
				mAsteroidVector[i]->setAliveStatus(true);
			}
			else
			{
				mAsteroidVector[i]->setPosition(100, -100);
				mAsteroidVector[i]->setAliveStatus(false);
			}
		}
	}
}

void Game::createBullets()
{
	sf::Color myColor;
	sf::Color otherPlayerColor;
	if (client.isPlayer1())
	{
		myColor = sf::Color::Cyan;
		otherPlayerColor = sf::Color::Red;
	}
	else
	{
		myColor = sf::Color::Red;
		otherPlayerColor = sf::Color::Cyan;
	}

	//Player One
	mNumberOfBulletsPlayerOne = 10;

	for (int i = 0; i < mNumberOfBulletsPlayerOne; i++)
		mBulletVectorPlayerOne.push_back(new Bullet(mScreenWidth, mScreenHeight));

	for (int i = 0; i < mNumberOfBulletsPlayerOne; i++)
	{
		mBulletVectorPlayerOne[i]->Init();
		mBulletVectorPlayerOne[i]->setFillColor(sf::Color::Black);
		mBulletVectorPlayerOne[i]->setOutlineColor(myColor);
		mBulletVectorPlayerOne[i]->setPosition((float)i * 10 + 250, 200);
		//mBulletVector[i]->setAliveStatus(true);
	}

	//Player Two
	mNumberOfBulletsPlayerTwo = 10;

	for (int i = 0; i < mNumberOfBulletsPlayerTwo; i++)
		mBulletVectorPlayerTwo.push_back(new Bullet(mScreenWidth, mScreenHeight));

	for (int i = 0; i < mNumberOfBulletsPlayerTwo; i++)
	{
		mBulletVectorPlayerTwo[i]->Init();
		mBulletVectorPlayerTwo[i]->setFillColor(sf::Color::Black);
		mBulletVectorPlayerTwo[i]->setOutlineColor(otherPlayerColor);
		mBulletVectorPlayerTwo[i]->setPosition((float)i * 10 + 250, 200);
		//mBulletVector[i]->setAliveStatus(true);
	}
}

void Game::PlayerOneBulletsFire()
{
	sf::Color bulletColor;
	if (client.isPlayer1())
	{
		bulletColor = sf::Color::Cyan;
	}
	else
	{
		bulletColor = sf::Color::Red;
	}

	mBulletVectorPlayerOne[mBulletIDPlayerOne]->setOutlineColor(bulletColor);
	mBulletVectorPlayerOne[mBulletIDPlayerOne]->setSin(false);
	mBulletVectorPlayerOne[mBulletIDPlayerOne]->setCos(false);
	mBulletVectorPlayerOne[mBulletIDPlayerOne]->setAngle(playerOneShip->getRotation());
	mBulletVectorPlayerOne[mBulletIDPlayerOne]->setVelocity(sf::Vector2f((mBulletVectorPlayerOne[mBulletIDPlayerOne]->getVelocity().x * 4.0f), (mBulletVectorPlayerOne[mBulletIDPlayerOne]->getVelocity().y * -4.0f)));
	mBulletVectorPlayerOne[mBulletIDPlayerOne]->setPosition(playerOneShip->getPositionX(), playerOneShip->getPositionY());
	mBulletVectorPlayerOne[mBulletIDPlayerOne]->setAliveStatus(true);
	mBulletVectorPlayerOne[mBulletIDPlayerOne]->setFiredStatus(true);

	mBulletVectorPlayerOne[mBulletIDPlayerOne + 1]->setOutlineColor(bulletColor);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 1]->setSin(true);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 1]->setCos(false);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 1]->setAngle(playerOneShip->getRotation());
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 1]->setVelocity(sf::Vector2f((mBulletVectorPlayerOne[mBulletIDPlayerOne + 1]->getVelocity().x * 4.0f), (mBulletVectorPlayerOne[mBulletIDPlayerOne + 1]->getVelocity().y * -4.0f)));
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 1]->setPosition(playerOneShip->getPositionX(), playerOneShip->getPositionY());
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 1]->setAliveStatus(true);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 1]->setFiredStatus(true);

	mBulletVectorPlayerOne[mBulletIDPlayerOne + 2]->setOutlineColor(bulletColor);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 2]->setSin(true);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 2]->setCos(true);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 2]->setAngle(playerOneShip->getRotation());
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 2]->setVelocity(sf::Vector2f((mBulletVectorPlayerOne[mBulletIDPlayerOne + 2]->getVelocity().x * 4.0f), (mBulletVectorPlayerOne[mBulletIDPlayerOne + 2]->getVelocity().y * -4.0f)));
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 2]->setPosition(playerOneShip->getPositionX(), playerOneShip->getPositionY());
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 2]->setAliveStatus(true);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 2]->setFiredStatus(true);

	mBulletVectorPlayerOne[mBulletIDPlayerOne + 3]->setOutlineColor(bulletColor);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 3]->setSin(false);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 3]->setCos(true);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 3]->setAngle(playerOneShip->getRotation());
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 3]->setVelocity(sf::Vector2f((mBulletVectorPlayerOne[mBulletIDPlayerOne + 3]->getVelocity().x * 4.0f), (mBulletVectorPlayerOne[mBulletIDPlayerOne + 3]->getVelocity().y * -4.0f)));
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 3]->setPosition(playerOneShip->getPositionX(), playerOneShip->getPositionY());
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 3]->setAliveStatus(true);
	mBulletVectorPlayerOne[mBulletIDPlayerOne + 3]->setFiredStatus(true);

	client.sendShootMessage();
}

void Game::PlayerTwoBulletsFire()
{
	sf::Color otherPlayerColor;
	if (client.isPlayer1())
	{
		otherPlayerColor = sf::Color::Red;
	}
	else
	{
		otherPlayerColor = sf::Color::Cyan;
	}

	mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->setOutlineColor(otherPlayerColor);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->setSin(false);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->setCos(false);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->setAngle(playerTwoShip->getRotation());
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->setVelocity(sf::Vector2f((mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->getVelocity().x * 4.0f), (mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->getVelocity().y * -4.0f)));
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->setPosition(playerTwoShip->getPositionX(), playerTwoShip->getPositionY());
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->setAliveStatus(true);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo]->setFiredStatus(true);

	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 1]->setOutlineColor(otherPlayerColor);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 1]->setSin(true);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 1]->setCos(false);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 1]->setAngle(playerTwoShip->getRotation());
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 1]->setVelocity(sf::Vector2f((mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 1]->getVelocity().x * 4.0f), (mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 1]->getVelocity().y * -4.0f)));
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 1]->setPosition(playerTwoShip->getPositionX(), playerTwoShip->getPositionY());
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 1]->setAliveStatus(true);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 1]->setFiredStatus(true);

	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 2]->setOutlineColor(otherPlayerColor);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 2]->setSin(true);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 2]->setCos(true);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 2]->setAngle(playerTwoShip->getRotation());
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 2]->setVelocity(sf::Vector2f((mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 2]->getVelocity().x * 4.0f), (mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 2]->getVelocity().y * -4.0f)));
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 2]->setPosition(playerTwoShip->getPositionX(), playerTwoShip->getPositionY());
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 2]->setAliveStatus(true);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 2]->setFiredStatus(true);

	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 3]->setOutlineColor(otherPlayerColor);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 3]->setSin(false);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 3]->setCos(true);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 3]->setAngle(playerTwoShip->getRotation());
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 3]->setVelocity(sf::Vector2f((mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 3]->getVelocity().x * 4.0f), (mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 3]->getVelocity().y * -4.0f)));
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 3]->setPosition(playerTwoShip->getPositionX(), playerTwoShip->getPositionY());
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 3]->setAliveStatus(true);
	mBulletVectorPlayerTwo[mBulletIDPlayerTwo + 3]->setFiredStatus(true);
}

void Game::createNewAsteroid(int ID)
{
	bool keepGoing = true;

	for (int i = 0; i < mNumberOfAsteroids; i++)
	{
		if (keepGoing)
		{
			if (mAsteroidVector[ID]->getAsteroidType() == 0)
			{
				if (mAsteroidVector[i]->getAsteroidType() == 3)
				{
					if (mAsteroidVector[ID]->getGoingSideWays())
					{
						mAsteroidVector[i]->setAsteroidType(1);
						mAsteroidVector[i]->Init(i);
						mAsteroidVector[i]->setPosition((int)mAsteroidVector[ID]->getPosition().x, (int)mAsteroidVector[ID]->getPosition().y - 50);
						mAsteroidVector[i]->setVelocity(sf::Vector2f(100.0f, 0.0f));
						mAsteroidVector[i]->setVelocity(sf::Vector2f(-mAsteroidVector[i]->getVelocity().x * 1.8f, 0.0f));
						mAsteroidVector[i]->setGoingSideWays(true);
					}
					else
					{
						mAsteroidVector[i]->setAsteroidType(1);
						mAsteroidVector[i]->Init(i);
						mAsteroidVector[i]->setPosition((int)mAsteroidVector[ID]->getPosition().x + 50, (int)mAsteroidVector[ID]->getPosition().y);
						mAsteroidVector[i]->setVelocity(sf::Vector2f(-1.0f, 100.0f));
						mAsteroidVector[i]->setVelocity(sf::Vector2f(mAsteroidVector[i]->getVelocity().x, mAsteroidVector[i]->getVelocity().y * 1.8f));
					}
					keepGoing = false;
				}
			}
			else if (mAsteroidVector[ID]->getAsteroidType() == 1)
			{
				if (mAsteroidVector[i]->getAsteroidType() == 3)
				{
					if (mAsteroidVector[ID]->getGoingSideWays())
					{
						mAsteroidVector[i]->setAsteroidType(2);
						mAsteroidVector[i]->Init(i);
						mAsteroidVector[i]->setPosition((int)mAsteroidVector[ID]->getPosition().x, (int)mAsteroidVector[ID]->getPosition().y - 50);
						mAsteroidVector[i]->setVelocity(sf::Vector2f(100.0f, 0.0f));
						mAsteroidVector[i]->setVelocity(sf::Vector2f(-mAsteroidVector[i]->getVelocity().x  * 2.3f, 0.0f));
						mAsteroidVector[i]->setGoingSideWays(true);
					}
					else
					{
						mAsteroidVector[i]->setAsteroidType(2);
						mAsteroidVector[i]->Init(i);
						mAsteroidVector[i]->setPosition((int)mAsteroidVector[ID]->getPosition().x + 50, (int)mAsteroidVector[ID]->getPosition().y);
						mAsteroidVector[i]->setVelocity(sf::Vector2f(-1.0f, 100.0f));
						mAsteroidVector[i]->setVelocity(sf::Vector2f(mAsteroidVector[i]->getVelocity().x, mAsteroidVector[i]->getVelocity().y * 2.3f));
					}
					keepGoing = false;
				}
			}
		}
	}
}

bool Game::checkGameLevel()
{
	int counter = 0;

	for (int i = 0; i < mNumberOfAsteroids; i++)
	{
		if (mAsteroidVector[i]->getAsteroidType() == 3)
			counter++;
	}

	if (counter >= mNumberOfAsteroids)
		return true;

	return false;
}