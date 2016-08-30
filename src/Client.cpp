#include "client.h"
#include "RakSleep.h"
#include "RakNetTypes.h"
#include "GetTime.h"
#include "Gets.h"
#include "Kbhit.h"

#include "SFML/Window/Mouse.hpp"
#include <iostream>

Client::Client()
{
	//mpRenderWindow = new RenderGame();
}

Client::~Client()
{
	//delete mpRenderWindow;
}

int main()
{
	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	Client* pClient = new Client();

	//gets the port 
	int port;
	std::cout << "Enter in Client port: "; //When Client launches type 201 in.
	std::cin >> port;

	if (pClient->LaunchClient(peer, "192.168.132.1", 200, port) == false)
		return 0;


	while (pClient->HandlePackets(peer))
	{
		RakSleep(30);
	}

	system("Pause");
	peer->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}

unsigned char Client::GetPacketIdentifier(RakNet::Packet *p)
{
	if (p == 0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
		return (unsigned char)p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
		return (unsigned char)p->data[0];
}

bool Client::LaunchClient(RakNet::RakPeerInterface* a_client, const char* a_serverIP, int a_serverPort, int a_clientPort)
{
	// IPV4 socket
	RakNet::SocketDescriptor sd(a_clientPort, 0);
	sd.socketFamily = AF_INET;

	int startupResult = a_client->Startup(1, &sd, 1);
	a_client->SetOccasionalPing(true);

	if (a_client->Connect(a_serverIP, a_serverPort, 0, 0) !=
		RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		printf("Attempt to connect to server FAILED\n");
		return false;
	}

	printf("\nCLIENT IP addresses:\n");
	for (unsigned int i = 0; i < a_client->GetNumberOfAddresses(); i++)
	{
		printf("%i. %s\n", i + 1, a_client->GetLocalIP(i));
	}

	return true;
}

bool Client::HandlePackets(RakNet::RakPeerInterface* a_peer)
{
	if (_kbhit())
	{
		//chat messages
		chatMessage chat2;
		Gets(mChat.message, sizeof(mChat.message));
		std::string prefix = "Player" + std::to_string(mPlayerID) + ": ";

		strncpy_s(chat2.message, prefix.c_str(), sizeof(chat2.message));
		strncat_s(chat2.message, mChat.message, sizeof(chat2.message) - strlen(prefix.c_str()) - 1);

		chat2.packetID = (unsigned char)messages::ID_TTT_CHAT;
		a_peer->Send((const char*)&(chat2), sizeof(chat2), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}

	if (mPlayerID == 1)
	{
		//mPlayer1.x = sf::Mouse::getPosition(mpRenderWindow->getWindow()).x;
		//mPlayer1.y = sf::Mouse::getPosition(mpRenderWindow->getWindow()).y;

		//mPlayer1Circle.setPosition(mpRenderWindow->update(mPlayer1Circle, mPlayer1.x, mPlayer1.y));
		//mPlayer1.packetID = (unsigned char)messages::ID_TTT_MOVE_UNIT;
		//a_peer->Send((const char*)&(mPlayer1), sizeof(mPlayer1), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	else if (mPlayerID == 2)
	{
		//mPlayer2.x = sf::Mouse::getPosition(mpRenderWindow->getWindow()).x;
		//mPlayer2.y = sf::Mouse::getPosition(mpRenderWindow->getWindow()).y;

		//mPlayer2Circle.setPosition(mpRenderWindow->update(mPlayer2Circle, mPlayer2.x, mPlayer2.y));
		//mPlayer2.packetID = (unsigned char)messages::ID_TTT_MOVE_UNIT;
		//a_peer->Send((const char*)&(mPlayer1), sizeof(mPlayer1), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}

	//mpRenderWindow->clearScreen();
	for (RakNet::Packet* p = a_peer->Receive();	p; a_peer->DeallocatePacket(p), p = a_peer->Receive())
	{
		auto packetID = GetPacketIdentifier(p);

		switch (packetID)
		{
			// Handle client packets
			case ID_CONNECTION_LOST:
			{ printf("\nConnection lost from %s", p->systemAddress.ToString(true)); break; }
			case ID_ALREADY_CONNECTED:
			{ printf("\nAlready connected with guid %s", p->guid); break; }
			case ID_CONNECTION_BANNED:
			{ printf("\nWe are banned from this server"); return false; }
			case ID_CONNECTION_ATTEMPT_FAILED:
			{ printf("\nConnection to server failed"); return false; }
			case ID_INVALID_PASSWORD:
			{ printf("\nPassword invalid"); return false; }
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Connection require ACCEPTED to %s with GUID %s\n",
					p->systemAddress.ToString(true), p->guid.ToString());
				printf("My external address is %s\n",
					a_peer->GetExternalID(p->systemAddress).ToString(true));

				unsigned char packet = (unsigned char)messages::ID_TTT_INIT_PLAYER;
				a_peer->Send((const char*)&(packet), sizeof(packet), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

				break;
			}

			case messages::ID_TTT_INIT_PLAYER:
			{
				player *tmpPlayer = (player*)p->data;
				assert(p->length == sizeof(player));
				if (p->length != sizeof(player))
					return false;

				if (tmpPlayer->playerNum == 1)
				{
					//inits player data
					mPlayerID = 1;
					mPlayer1.x = 100;// sf::Mouse::getPosition(mpRenderWindow->getWindow());
					mPlayer1.y = 100;// sf::Mouse::getPosition(mpRenderWindow->getWindow());
					//mPlayer1Circle = mpRenderWindow->addShape(150, 150, 150);
					//mPlayer1Circle.setPosition(mpRenderWindow->update(mPlayer1Circle, mPlayer1.x, mPlayer1.y));
					printf("\nWaiting for player 2.\n");
				}
				else if (tmpPlayer->playerNum == 2)
				{
					//inits player data
					mPlayerID = 2;
					mPlayer1.x = 200;// sf::Mouse::getPosition(mpRenderWindow->getWindow());
					mPlayer1.y = 200;// sf::Mouse::getPosition(mpRenderWindow->getWindow());
					//mPlayer2Circle = mpRenderWindow->addShape(150, 150, 150);
					//mPlayer2Circle.setPosition(mpRenderWindow->update(mPlayer2Circle, mPlayer2.x, mPlayer2.y));
				}
			}

			case messages::ID_TTT_DISPLAY_PLAYER:
			{
				player *tmpPlayers = (player*)p->data;
				assert(p->length == sizeof(player));
				if (p->length != sizeof(player))
					return false;

				//printf("client player 1 num %d\n", tmpPlayers->playerNum);
				//printf("client player 2 num %d\n", tmpPlayers->playerNum);

				//if (tmpPlayers->playerNum == 1)
				//{
					//mpRenderWindow->clearScreen();
					//printf("before draw\n");
					//mpRenderWindow->draw(mPlayer1Circle);
					//printf("after draw\n");
				//}
				//if (tmpPlayers->playerNum == 2)
				//{
					//printf("before draw\n");
					//mpRenderWindow->clearScreen();
					//mpRenderWindow->draw(mPlayer2Circle);
					//mpRenderWindow->displayScreen();
					//printf("after draw\n");
					//mpRenderWindow->displayScreen();
				//}
				break;
			}

			case messages::ID_TTT_MOVE_UNIT:
			{
				/*player *tmpPlayers = (player*)p->data;
				if (tmpPlayers->playerNum == 1)
					mpRenderWindow->update(mPlayer1Circle, mPlayer1.x, mPlayer1.y);
				else if (tmpPlayers->playerNum == 2)
					mpRenderWindow->update(mPlayer2Circle, mPlayer2.x, mPlayer2.y);*/
			}

			case messages::ID_TTT_CHAT:
			{
				chatMessage *chat = (chatMessage*)p->data;
				//a_peer->Send(mMessage, (const int)strlen(mMessage) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
				printf("%s\n", chat->message);
				break;
			}

			default:
			{
				printf("\nReceived unhandled packet");
				break;
			}
		}
	}
	//mpRenderWindow->displayScreen();
	return true;
}
