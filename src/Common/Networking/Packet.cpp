#include "Common/Networking/Packet.hpp"

PacketSend::PacketSend(RakNet::MessageID id, PacketPriority priority, PacketReliability reliability)
{
	this->packet_id = id;
	this->packet_priority = priority;
	this->packet_reliability = reliability;

	this->bitStream_out.Write(this->packet_id);
}