#pragma once

#include "RakPeerInterface.h"
#include "../Common/Common.h"

class Ball;

class Client
{
public:
	Client();
	~Client();

	void startClient(const char* serverIP, int serverPort, int clientPort);
	void disconnectFromServer();

	void waitForPackets();
	void update(float posX, float posY, float playerRotation);
	PlayerVariables getOtherPosition();
	void setGameStarted(bool hasGameStarted);
	bool isGameStarted();
	bool isPlayer1();

	void checkForBallUpdates(Ball* gameBall);
	void sendBallUpdate(float ballPosX, float ballPosY, float ballVelX, float ballVelY);
	int checkForBlockUpdates();

	void sendRemoveBlocksMessage();
	void sendReviveBlocksMessage();
	void sendGameOverMessage();
	void sendShootMessage();

	static void PrintUsage();
private:
	RakNet::RakPeerInterface *peer;
	int pingTime;

	PlayerVariables otherPlayer;
	InterpolatedPosition interpolation;
	int updatedBlocks;
	bool newBallUpdates;
	bool gameStarted;
	bool amIPlayer1;

	RakNet::TimeMS lastTime;
	RakNet::TimeMS timer;
};