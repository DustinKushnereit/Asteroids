#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
//#include "ball.h"
#include "paddle.h"
//#include "Block.h"
#include "Asteroid.h"
#include "Common\InterpolatedPosition.h"
#include "Client\Client.h"
#include "Bullet.h"
#include <sstream>

class Game
{
public:
	Game(float screenHeight, float screenWidth);
	Game() {};
	~Game();

	void Init();
	void StartClient(const char* serverIP, int serverPort, int clientPort);
	void Update(sf::Clock& clock);

	void createAsteroids();
	void createBullets();

	void PlayerOneBulletsFire();
	void PlayerTwoBulletsFire();

	void createNewAsteroid(int ID);
	bool checkGameLevel();

	sf::RenderWindow* getWindow() { return mRenderWindow; }
	float getScreenWidth() { return mScreenWidth; }
	float getScreenHeight() { return mScreenHeight; }
	void increaseTotalPoints(int points) { totalPoints += points; };
	int getLevelNumber() { return levelNumber; };

	//Player One
	void setPlayerOneAngle(float angle) { playerOneAngle = angle; };
	void setPlayerOnePos(sf::Vector2f pos) { playerOnePosition = pos; };
	void setBulletIDPlayerOne(int bulletID) { mBulletIDPlayerOne = bulletID; };
	void setFiredStatusPlayerOne(bool status) { playerOneHasFiredBullet = status; };

	float getPlayerOneAngle() { return playerOneAngle; };
	sf::Vector2f getPlayerOnePos() { return playerOnePosition; };
	int getBulletIDPlayerOne() { return mBulletIDPlayerOne; };
	bool getFiredStatusPlayerOne() { return playerOneHasFiredBullet; };

	void reducePlayerOneLives() { playerOneHealth--; };

	//Player Two
	void setPlayerTwoAngle(float angle) { playerTwoAngle = angle; };
	void setPlayerTwoPos(sf::Vector2f pos) { playerTwoPosition = pos; };
	void setBulletIDPlayerTwo(int bulletID) { mBulletIDPlayerTwo = bulletID; };
	void setFiredStatusPlayerTwo(bool status) { playerTwoHasFiredBullet = status; };

	float getPlayerTwoAngle() { return playerTwoAngle; };
	sf::Vector2f getPlayerTwoPos() { return playerTwoPosition; };
	int getBulletIDPlayerTwo() { return mBulletIDPlayerTwo; };
	bool getFiredStatusPlayerTwo() { return playerTwoHasFiredBullet; };

	void reducePlayerTwoLives() { playerTwoHealth--;};

	void setGameOver(bool isGameOver) { gameOver = isGameOver; };
	
private:
	float mScreenWidth;
	float mScreenHeight;
	bool isPlaying;
	bool isTitleOn;
	bool otherPlayerConnected;
	int levelNumber;
	bool LKeyPressed;
	bool gameOver;

	int playerOneHealth;
	int playerTwoHealth;
	int totalPoints;

	Client client;

	//Ball* gameBall;
	Paddle* playerOneShip;
	Paddle* playerTwoShip;

	int mNumberOfAsteroids;
	std::vector<Asteroid*> mAsteroidVector;

	//Player One
	int mNumberOfBulletsPlayerOne;
	int mBulletIDPlayerOne;
	bool playerOneHasFiredBullet;
	std::vector<Bullet*> mBulletVectorPlayerOne;
	sf::Vector2f playerOnePosition;
	float playerOneAngle;

	//Player Two
	int mNumberOfBulletsPlayerTwo;
	int mBulletIDPlayerTwo;
	bool playerTwoHasFiredBullet;
	std::vector<Bullet*> mBulletVectorPlayerTwo;
	sf::Vector2f playerTwoPosition;
	float playerTwoAngle;

	sf::Time deltaTime;
	sf::RenderWindow* mRenderWindow;
	sf::Font font;

	sf::SoundBuffer startBuffer;
	sf::Sound startSound;
	sf::SoundBuffer lossBuffer;
	sf::Sound lossSound;
	sf::Music music;

	sf::Text playerOneLivesLabel;
	sf::Text playerTwoLivesLabel;

	sf::Text spaceLabel;
	sf::Text titleLabel;
	sf::Text winLabel;
};

extern Game* gpGame;