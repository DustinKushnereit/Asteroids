#pragma once

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "GetTime.h"
#include <vector>

#define MAX_CLIENTS 8
#define MAX_GAMES MAX_CLIENTS/2
#define TIMEMS_BETWEEN_RAKNET_UPDATE 50
#define INTERPOLATION_TIME 50.0

class Block;

typedef std::vector<Block> BlockVector;

namespace CommonNetwork
{
	RakNet::MessageID GetPacketIdentifier(RakNet::Packet *p);
}

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
	START_GAME_MESSAGE,
	PLAYER_MOVE_UPDATE,
	UPDATE_BALL,
	REMOVE_ALL_BLOCKS_MESSAGE,
	REVIVE_ALL_BLOCKS_MESSAGE,
	GAME_OVER,
	PLAYER_SHOOT
};

enum BlockUpdateMessages
{
	REMOVE_BLOCKS = -3,
	REVIVE_BLOCKS,
	NO_UPDATE
};

struct Position
{
	float x;
	float y;
};

struct Velocity
{
	float xVelocity;
	float yVelocity;
};

struct BallVariables
{
	Position position;
	Velocity velocity;
};

struct PlayerVariables
{
	Position pos;
	float rotation;
};

struct PlayerUpdatePacket
{
	//done according to jenkinssoftware.com/raknet/manual
	unsigned char useTimeStamp; // Assign ID_TIMESTAMP to this
	RakNet::Time timeStamp; // Put the system time in here returned by RakNet::GetTime()
	unsigned char typeId; // You should put here an enum you defined after the last one defined in MessageIdentifiers.h, lets say ID_SET_TIMED_MINE
	PlayerVariables playerState;
};