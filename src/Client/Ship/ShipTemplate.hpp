#ifndef SHIPTEMPLATE_HPP
#define SHIPTEMPLATE_HPP

#include "Common/Ship/ShipCell.hpp"

#include "Common/GameObject.hpp"

#include "Common/GLM_Include.hpp"
#include <unordered_map>

class ShipTemplate
{
public:
	ShipTemplate(float cube_size);
	~ShipTemplate();

	bool hasCellAt(vector3B pos);

	bool canPlaceCell(vector3B pos, ShipCell* cell);
	void addCell(vector3B pos, ShipCell* cell);
	void removeCell(vector3B pos);
	ShipCell* getCell(vector3B pos);

	bool hasConnectionPoint(vector3B pos, Direction direction);

	bool hasChanged();
	void rebuildRenderObject();
	GameObject* getRenderObject();

private:
	float cell_size = 3.0f;

	std::unordered_map<vector3B, ShipCell*, HashVector3B> cells;

	bool has_changed = false;

	GameObject* render_object = nullptr;
};

#endif //SHIPTEMPLATE_HPP