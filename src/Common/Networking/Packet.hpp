#ifndef PACKET_HPP
#define PACKET_HPP

#include "Common/Types.hpp"

#include "BitStream.h"
#include "RakNetTypes.h"
#include "PacketPriority.h"
#include "MessageIdentifiers.h"

enum PacketTypes
{
	UserConnect = ID_USER_PACKET_ENUM + 1,
	EntitySpawn = ID_USER_PACKET_ENUM + 2,
	EntityUpdate = ID_USER_PACKET_ENUM + 3,
	EntitySpawnRequest = ID_USER_PACKET_ENUM + 4,
};


class PacketSend
{
public:
	PacketSend(RakNet::MessageID id, PacketPriority priority, PacketReliability reliability);

	void write_String(string value);
	void write_Int(int value);
	void write_Float(float value);
	void write_Double(double value);

	RakNet::BitStream bitStream_out;
	RakNet::MessageID packet_id;
	PacketPriority packet_priority;
	PacketReliability packet_reliability;
};

class PacketReceive
{
public:
	PacketReceive(RakNet::MessageID id, RakNet::BitStream bitStream);

	string read_String();
	int read_Int();
	float read_Float();
	double read_Double();

	RakNet::MessageID packet_id;
	RakNet::BitStream bitStream_in;
};

#endif //PACKET_HPP