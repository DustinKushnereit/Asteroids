#include "Common.h"

//As seen in http://www.raknet.net/raknet/manual/receivingpackets.html
RakNet::MessageID CommonNetwork::GetPacketIdentifier(RakNet::Packet *p)
{
	if ((RakNet::MessageID)p->data[0] == ID_TIMESTAMP)
		return (RakNet::MessageID)p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	else
		return (RakNet::MessageID)p->data[0];
}