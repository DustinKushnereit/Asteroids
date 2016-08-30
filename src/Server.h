#pragma once
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "RakPeer.h"
#include "RenderGame.h"

namespace messages {
	enum value_type : unsigned char
	{
		ID_TTT_TURN_AND_BOARD = ID_USER_PACKET_ENUM,
		ID_TTT_OTHER_USER_WON, 
		ID_TTT_MOVE_UNIT,
		ID_TTT_DISPLAY_PLAYER,
		ID_TTT_INIT_PLAYER,
		ID_TTT_CHAT
	};
};

#pragma pack(push, 1)
struct player
{
	unsigned char packetID;
	int playerNum = 0;
	int x, y;
};
struct chatMessage
{
	unsigned char packetID;
	char message[2028];
};
struct ClientState
{
	long unsigned int m_id;
	int x, y;
};
struct SeverState
{

};
//struct players
//{
//	unsigned char packetID;
//	//player player1;
//	//player player2;
//};
#pragma pop


class Server
{
public:
	Server();
	~Server();
	int main();
	unsigned char GetPacketIdentifier(RakNet::Packet *p);
	bool LaunchServer(RakNet::RakPeerInterface* a_peer, int a_port);
	bool HandlePackets(RakNet::RakPeerInterface* a_peer);

private:
	RenderGame* mpRenderWindow;
	//players mPlayers;
	player mPlayer1;
	player mPlayer2;
	chatMessage mChat;
};