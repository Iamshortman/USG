#ifndef PACKET_HPP
#define PACKET_HPP

#include "Common/Types.hpp"
#include "Common/GLM_Include.hpp"

#include "BitStream.h"
#include "RakNetTypes.h"
#include "PacketPriority.h"
#include "MessageIdentifiers.h"

using RakNet::BitStream;

#include "Common/Transform.hpp"

enum PacketTypes
{
	UpdateEntity = ID_USER_PACKET_ENUM + 1,
	UpdateClientEntity,
	ClientBindEntity,
	CreateEntity,
	CreateWorld,
	DeleteEntity,
	DeleteWorld,
};

class PacketSend
{
public:
	PacketSend(RakNet::MessageID id, PacketPriority priority, PacketReliability reliability);

	RakNet::BitStream bitStream_out;
	RakNet::MessageID packet_id;
	PacketPriority packet_priority;
	PacketReliability packet_reliability;
};
#endif //PACKET_HPP