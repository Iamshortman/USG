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
	UserConnect = ID_USER_PACKET_ENUM + 1,
	UserRequest,
	UpdateEntity,
	UpdateClientEntity,
	ClientBindEntity,
	CreateEntity,
};

void write_Tranform(BitStream* bitStream, Transform transform);
void read_Tranform(BitStream* bitStream, Transform* transform);

void write_vector3d(BitStream* bitStream, vector3D vector);
void read_vector3d(BitStream* bitStream, vector3D* vector);

void write_quaternionD(BitStream* bitStream, quaternionD quaternion);
void read_quaternionD(BitStream* bitStream, quaternionD* quaternion);

class PacketSend
{
public:
	PacketSend(RakNet::MessageID id, PacketPriority priority, PacketReliability reliability);

	/*void write_String(string value);
	void write_Int(int value);
	void write_Float(float value);
	void write_Double(double value);
	void write_Vector3D(vector3D value);*/

	RakNet::BitStream bitStream_out;
	RakNet::MessageID packet_id;
	PacketPriority packet_priority;
	PacketReliability packet_reliability;
};

/*class PacketReceive
{
public:
	PacketReceive(RakNet::MessageID id, RakNet::BitStream bitStream);

	string read_String();
	int read_Int();
	float read_Float();
	double read_Double()

	RakNet::MessageID packet_id;
	RakNet::BitStream bitStream_in;
};*/

#endif //PACKET_HPP