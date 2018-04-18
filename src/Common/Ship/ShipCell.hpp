#ifndef SHIPCELL_HPP
#define SHIPCELL_HPP

#include "Common/Types.hpp"
#include "Common/GLM_Include.hpp"
#include "Common/Rotation.hpp"

#include <unordered_map>
#include <unordered_set>

struct Connection
{
	bool direction[6];
};

class ShipCell
{
public:
	ShipCell();
	~ShipCell();

	string mesh;

	std::unordered_map<vector3B, Connection, HashVector3B> connection_points;
	std::unordered_set<vector3B, HashVector3B> points;

	vector3B center_pos;
private:
};

#endif //SHIPCELL_HPP