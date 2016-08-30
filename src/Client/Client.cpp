#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

#include "../Ball.h"
#include "../Game.h"

#include "GetTime.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include <array>

#include "Client.h"

Client::Client()
{
	peer = RakNet::RakPeerInterface::GetInstance();

	lastTime = RakNet::GetTimeMS();
	timer = 0;

	otherPlayer.pos.x = 575;
	otherPlayer.pos.y = 500;
	otherPlayer.rotation = 0;
	interpolation = InterpolatedPosition(otherPlayer.pos.x, otherPlayer.pos.y, otherPlayer.rotation, RakNet::GetTime());

	pingTime = 10;

	gameStarted = false;
	newBallUpdates = false;
	updatedBlocks = NO_UPDATE;
}

Client::~Client()
{
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void Client::startClient(const char* serverIP, int serverPort, int clientPort)
{
	RakNet::SocketDescriptor sd(clientPort, 0);
	sd.socketFamily = AF_INET;

	peer->Startup(8, &sd, 1);
	peer->SetOccasionalPing(true);

	if (peer->Connect(serverIP, serverPort, 0, 0) !=
		RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		printf("\nAttempt to connect to server FAILED");
	}

	printf("\nCLIENT IP addresses:");
	for (unsigned int i = 0; i < peer->GetNumberOfAddresses(); i++)
	{
		printf("\n%i. %s\n", i + 1, peer->GetLocalIP(i));
	}
}

void Client::disconnectFromServer()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_DISCONNECTION_NOTIFICATION);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::waitForPackets()
{
	for (RakNet::Packet *packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
	{
		RakNet::MessageID packetID = CommonNetwork::GetPacketIdentifier(packet);
		switch (packetID)
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			printf("Another client has disconnected.\n");
			break;
		case ID_REMOTE_CONNECTION_LOST:
			printf("Another client has lost the connection.\n");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
		{
			printf("Another client has connected.\n");

		}
		break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			printf("Our connection request has been accepted.\n");
		}

		break;
		case ID_NEW_INCOMING_CONNECTION:
		{
			printf("A connection is incoming.\n");
		}
		break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The server is full.\n");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
				printf("We have been disconnected.\n");
			break;
		case ID_CONNECTION_LOST:
				printf("Connection lost.\n");
			break;

		case ID_GAME_MESSAGE_1:
		{
			RakNet::RakString rs;
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(rs);
			printf("%s\n", rs.C_String());
		}
		break;

		case START_GAME_MESSAGE:
		{
			gameStarted = true;
			bool player1;
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(player1);
			amIPlayer1 = player1;
		}
			break;
		case ID_TIMESTAMP://assuming movement is the onlt thing time stamped
		case PLAYER_MOVE_UPDATE:
		{
			PlayerUpdatePacket packetRecieved;
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.Read(packetRecieved.useTimeStamp);
			bsIn.Read(packetRecieved.timeStamp);
			bsIn.Read(packetRecieved.typeId);
			bsIn.Read(packetRecieved.playerState);
			//otherPlayer = newPos;
			RakNet::Time curr = RakNet::GetTime();

			interpolation.setNewTarget(packetRecieved.playerState, curr, packetRecieved.timeStamp + INTERPOLATION_TIME);
		}
			break;

		case UPDATE_BALL:
			break;

		case REMOVE_ALL_BLOCKS_MESSAGE:
			updatedBlocks = REMOVE_BLOCKS;
			break;
		case REVIVE_ALL_BLOCKS_MESSAGE:
			updatedBlocks = REVIVE_BLOCKS;
			break;

		case PLAYER_SHOOT:
		{
			gpGame->setFiredStatusPlayerTwo(true);

			int tmp = gpGame->getBulletIDPlayerTwo();
			if (tmp >= 9)
			{
				tmp = 0;
				gpGame->setBulletIDPlayerTwo(tmp);
			}
			gpGame->setBulletIDPlayerTwo(tmp++);
		}
			break;

		case GAME_OVER:
		{
			gpGame->setGameOver(true);
		}
			break;

		default:
			printf("Message with identifier %i has arrived.\n", packet->data[0]);
			break;
		}
	}
}

void Client::update(float posX, float posY, float playerRotation)
{
	if (timer > TIMEMS_BETWEEN_RAKNET_UPDATE)
	{
		PlayerUpdatePacket packet;
		packet.useTimeStamp = (RakNet::MessageID)ID_TIMESTAMP;
		packet.timeStamp = RakNet::GetTime();
		packet.typeId = (RakNet::MessageID)PLAYER_MOVE_UPDATE;

		PlayerVariables player;
		player.pos.x = posX;
		player.pos.y = posY;
		player.rotation = playerRotation;

		packet.playerState = player;

		RakNet::BitStream bsOut;
		bsOut.Write(packet.useTimeStamp);
		bsOut.Write(packet.timeStamp);
		bsOut.Write(packet.typeId);
		bsOut.Write(packet.playerState);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

		timer = 0;
	}
	timer += RakNet::GetTimeMS() - lastTime;
	lastTime = RakNet::GetTimeMS();

	otherPlayer = interpolation.getNextPosition(RakNet::GetTime());
}

PlayerVariables Client::getOtherPosition()
{
	return otherPlayer;
}
void Client::setGameStarted(bool hasGameStarted)
{
	gameStarted = hasGameStarted;
}
bool Client::isGameStarted()
{
	return gameStarted;
}
bool Client::isPlayer1()
{
	return amIPlayer1;
}

void Client::checkForBallUpdates(Ball* gameBall)
{
}

void Client::sendBallUpdate(float ballPosX, float ballPosY, float ballVelX, float ballVelY)
{
	BallVariables ballState;
	ballState.position.x = ballPosX;
	ballState.position.y = ballPosY;
	ballState.velocity.xVelocity = ballVelX;
	ballState.velocity.yVelocity = ballVelY;

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)UPDATE_BALL);
	bsOut.Write(ballState);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

int Client::checkForBlockUpdates()
{
	int updateToGive = updatedBlocks;
	updatedBlocks = NO_UPDATE;
	return updateToGive;
}

void Client::sendRemoveBlocksMessage()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)REMOVE_ALL_BLOCKS_MESSAGE);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}
void Client::sendReviveBlocksMessage()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)REVIVE_ALL_BLOCKS_MESSAGE);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::sendGameOverMessage()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)GAME_OVER);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::sendShootMessage()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)PLAYER_SHOOT);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::PrintUsage()
{
	printf("\n-- USAGE --");
	printf("\nAsteroids <clientPort> <serverIP> <serverPort>");
}

/*int main(int argc, char** argv)
{
	int width = 400;
	int height = 300;

	if (argc == 4) // clientPort serverIP serverPort
	{
		const int   clientPort = atoi(argv[1]);
		const char* serverIP = argv[2];
		const int   serverPort = atoi(argv[3]);

		player.startClient(serverIP, serverPort, clientPort);
	}
	else
	{
		PrintUsage();
		return 0;
	}

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{


			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				player.disconnectFromServer();
				window.close();
			}
		}

		player.waitForPackets();
		player.update(position.x, position.y);

		Position newPos = player.getOtherPosition();
		otherPlayerShape.setPosition(newPos.x-otherPlayerShape.getRadius(), newPos.y-otherPlayerShape.getRadius());

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		// window.draw(...);
		window.draw(player.getShape());
		if (player.isGameStarted())
		{
			window.draw(otherPlayerShape);
		}

		// end the current frame
		window.display();
	}

	return 0;
}*/