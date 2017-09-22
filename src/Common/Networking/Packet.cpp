#include "Common/Networking/Packet.hpp"

PacketSend::PacketSend(RakNet::MessageID id, PacketPriority priority, PacketReliability reliability)
{
	this->packet_id = id;
	this->packet_priority = priority;
	this->packet_reliability = reliability;

	this->bitStream_out.Write(this->packet_id);
}

void PacketSend::write_String(string value)
{
	this->bitStream_out.Write(value.c_str());
}

void PacketSend::write_Int(int value)
{
	this->bitStream_out.Write(value);
}

void PacketSend::write_Float(float value)
{
	this->bitStream_out.Write(value);
}

void PacketSend::write_Double(double value)
{
	this->bitStream_out.Write(value);
}

PacketReceive::PacketReceive(RakNet::MessageID id, RakNet::BitStream bitStream)
{
	this->packet_id = id;
	//this->bitStream_in = bitStream;
}

string PacketReceive::read_String()
{
	string value;
	this->bitStream_in.Read(value);
	return value;
}

int PacketReceive::read_Int()
{
	int value;
	this->bitStream_in.Read(value);
	return value;
}

float PacketReceive::read_Float()
{
	float value;
	this->bitStream_in.Read(value);
	return value;
}

double PacketReceive::read_Double()
{
	double value;
	this->bitStream_in.Read(value);
	return value;
}
