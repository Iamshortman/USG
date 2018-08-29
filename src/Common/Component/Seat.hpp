#ifndef SEAT_HPP
#define SEAT_HPP

#include "Common/EntityX_Include.hpp"

struct Seat
{
	Entity seated_entity;

	bool hasSeatedEntity()
	{
		return seated_entity.id() != Entity::INVALID;
	};
};

#endif //SEAT_HPP