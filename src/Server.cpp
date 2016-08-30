#include "server.h"
#include "RakSleep.h"
#include "RakNetTypes.h"
#include "GetTime.h"
#include "Gets.h"

#include "SFML/Window/Mouse.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

Server::Server()
{
	mPlayer1.playerNum = 0;
	mPlayer2.playerNum = 0;
	//mpRenderWindow = new RenderGame();
}

Server::~Server()
{
	//delete mpRenderWindow;
}

int main()
{
	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	Server* pServer = new Server();

	if (pServer->LaunchServer(peer, 200) == false)
		return 0;

	while (pServer->HandlePackets(peer))
	{
		RakSleep(30);
	}

	system("Pause");
	peer->Shutdown(300);

	RakNet::RakPeerInterface::DestroyInstance(peer);
	delete pServer;

	return 0;
}

unsigned char Server::GetPacketIdentifier(RakNet::Packet *p)
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

bool Server::LaunchServer(RakNet::RakPeerInterface* a_server, int a_port)
{
	a_server->SetIncomingPassword(0, 0);

	// IPV4 socket
	RakNet::SocketDescriptor  sd;
	sd.port = a_port;
	sd.socketFamily = AF_INET;

	if (a_server->Startup(4, &sd, 1) != RakNet::RAKNET_STARTED)
	{
		printf("\nFailed to start server with IPV4 ports");
		return false;
	}

	a_server->SetOccasionalPing(true);
	a_server->SetUnreliableTimeout(1000);
	a_server->SetMaximumIncomingConnections(4);

	printf("\nSERVER IP addresses:");
	for (unsigned int i = 0; i < a_server->GetNumberOfAddresses(); i++)
	{
		RakNet::SystemAddress sa = a_server->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS, i);
		printf("\n%i. %s (LAN=%i)", i + 1, sa.ToString(false), sa.IsLANAddress());
	}

	return true;
}

bool Server::HandlePackets(RakNet::RakPeerInterface* a_peer)
{
	//send packets
	mPlayer1.packetID = (unsigned char)messages::ID_TTT_DISPLAY_PLAYER;
	//a_peer->Send((const char*)&(mPlayer1), sizeof(mPlayer1), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	mPlayer2.packetID = (unsigned char)messages::ID_TTT_DISPLAY_PLAYER;
	//a_peer->Send((const char*)&(mPlayer2), sizeof(mPlayer2), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	for (RakNet::Packet* p = a_peer->Receive(); p; a_peer->DeallocatePacket(p), p = a_peer->Receive())
	{
		auto packetID = GetPacketIdentifier(p);

		switch (packetID)
		{
			case ID_CONNECTION_LOST:
			{ printf("\nConnection lost from %s", p->systemAddress.ToString(true)); break; }
			case ID_NEW_INCOMING_CONNECTION:
			{
				printf("\nNew incoming connection from %s with GUID %s",
					p->systemAddress.ToString(true), p->guid.ToString());
				break;
			}

			case messages::ID_TTT_INIT_PLAYER:
			{
				//printf("init player\n");
				if (mPlayer1.playerNum != 1)//(mPlayers.player1.playerNum == 0)
				{
					mPlayer1.playerNum = 1;
					mPlayer2.x = 0;// = sf::Mouse::getPosition(mpRenderWindow->getWindow());
					mPlayer2.y = 0;

					mPlayer1.packetID = (unsigned char)messages::ID_TTT_INIT_PLAYER;
					a_peer->Send((const char*)&(mPlayer1), sizeof(mPlayer1), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				}
				else if (mPlayer2.playerNum != 2)//(mPlayers.player2.playerNum == 0)
				{
					//inits player data
					mPlayer2.playerNum = 2;
					mPlayer2.x = 0;// = sf::Mouse::getPosition(mpRenderWindow->getWindow());
					mPlayer2.y = 0;

					mPlayer2.packetID = (unsigned char)messages::ID_TTT_INIT_PLAYER;
					a_peer->Send((const char*)&(mPlayer2), sizeof(mPlayer2), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				}
				break;
			}

			case messages::ID_TTT_MOVE_UNIT:
			{
				player *pPlayer = (player*)p->data;

				pPlayer->packetID = (unsigned char)messages::ID_TTT_MOVE_UNIT;
				//a_peer->Send((const char*)&(pPlayer), sizeof(pPlayer), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				//mPlayerVec[index].pos = pPlayer->pos;
				//mPlayerVec[index].packetID = (unsigned char)messages::ID_TTT_DISPLAY_PLAYER;

				//a_peer->Send((const char*)&(mPlayerVec[index]), sizeof(mPlayerVec[index]), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				break;
			}

			case messages::ID_TTT_CHAT:
			{
				chatMessage *chat = (chatMessage*)p->data;
				mChat = *chat;
				printf("%s\n", chat->message);
				mChat.packetID = (unsigned char)messages::ID_TTT_CHAT;
				a_peer->Send((const char*)&(mChat), sizeof(mChat), HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
				break;
			}

			default:
			{
				printf("%s\n", p->data);
				break;
			}
		}
	}

	return true;
}