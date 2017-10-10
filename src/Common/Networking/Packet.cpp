#include "Common/Networking/Packet.hpp"

void write_Tranform(BitStream* bitStream, Transform transform)
{
	write_vector3d(bitStream, transform.position);
	write_quaternionD(bitStream, transform.orientation);
}

void read_Tranform(BitStream* bitStream, Transform* transform)
{
	read_vector3d(bitStream, &transform->position);
	read_quaternionD(bitStream, &transform->orientation);
}

void write_vector3d(BitStream* bitStream, vector3D vector)
{
	bitStream->Write(vector.x);
	bitStream->Write(vector.y);
	bitStream->Write(vector.z);
}
void read_vector3d(BitStream* bitStream, vector3D* vector)
{
	bitStream->Read(vector->x);
	bitStream->Read(vector->y);
	bitStream->Read(vector->z);
}

void write_quaternionD(BitStream* bitStream, quaternionD quaternion)
{
	bitStream->Write(quaternion.w);
	bitStream->Write(quaternion.x);
	bitStream->Write(quaternion.y);
	bitStream->Write(quaternion.z);
}

void read_quaternionD(BitStream* bitStream, quaternionD* quaternion)
{
	bitStream->Write(quaternion->w);
	bitStream->Write(quaternion->x);
	bitStream->Write(quaternion->y);
	bitStream->Write(quaternion->z);
}

PacketSend::PacketSend(RakNet::MessageID id, PacketPriority priority, PacketReliability reliability)
{
	this->packet_id = id;
	this->packet_priority = priority;
	this->packet_reliability = reliability;

	this->bitStream_out.Write(this->packet_id);
}

/*void PacketSend::write_String(string value)
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

void PacketSend::write_Vector3D(vector3D value)
{
	this->bitStream_out.Write(value);
}*/

/*PacketReceive::PacketReceive(RakNet::MessageID id, RakNet::BitStream bitStream)
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
}*/
