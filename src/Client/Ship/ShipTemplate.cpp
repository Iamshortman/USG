#include "ShipTemplate.hpp"

#include "Common/GameObjectManager.hpp"
#include "Client/Resource/TexturedMesh.hpp"
#include "Common/Component/ComponentModel.hpp"

ShipTemplate::ShipTemplate(float cube_size)
{
	this->cell_size = cube_size;
}

ShipTemplate::~ShipTemplate()
{

}

bool ShipTemplate::hasCellAt(vector3B pos)
{
	return this->cells.find(pos) != this->cells.end();
}

bool ShipTemplate::canPlaceCell(vector3B pos, ShipCell* cell)
{
	for (vector3B point : cell->points)
	{
		if (this->hasCellAt(point + pos))
		{
			return false;
		}
	}

	return true;
}

void ShipTemplate::addCell(vector3B pos, ShipCell* cell)
{
	if (this->canPlaceCell(pos, cell))
	{
		cell->center_pos = pos;

		for (vector3B point : cell->points)
		{
			this->cells[pos + point] = cell;
		}

		has_changed = true;
	}
}

void ShipTemplate::removeCell(vector3B pos)
{
	if (this->hasCellAt(pos))
	{
		ShipCell* cell = this->getCell(pos);

		for (vector3B point : cell->points)
		{
			this->cells.erase(cell->center_pos + point);
		}

		delete cell;

		this->has_changed = true;
	}
}

ShipCell* ShipTemplate::getCell(vector3B pos)
{
	if (this->hasCellAt(pos))
	{
		return this->cells[pos];
	}

	return nullptr;
}

bool ShipTemplate::hasConnectionPoint(vector3B pos, Direction direction)
{
	if (this->hasCellAt(pos))
	{
		ShipCell* cell = this->getCell(pos);
		return cell->connection_points[cell->center_pos - pos].direction[direction];
	}

	return false;
}

bool ShipTemplate::hasChanged()
{
	return this->has_changed;
}

void ShipTemplate::rebuildRenderObject()
{
	if (this->has_changed)
	{
		this->has_changed = false;

		if (this->render_object != nullptr)
		{
			GameObjectManager::destroyGameObject(this->render_object->object_Id);
		}

		this->render_object = GameObjectManager::instance->createGameObject();

		vector3F vertsCube[] =
		{
			vector3F(-0.5f, -0.5f, -0.5f) * cell_size,
			vector3F(-0.5f, -0.5f, 0.5f) * cell_size,
			vector3F(-0.5f, 0.5f, -0.5f) * cell_size,
			vector3F(-0.5f, 0.5f, 0.5f) * cell_size,
			vector3F(0.5f, -0.5f, -0.5f) * cell_size,
			vector3F(0.5f, -0.5f, 0.5f) * cell_size,
			vector3F(0.5f, 0.5f, -0.5f) * cell_size,
			vector3F(0.5f, 0.5f, 0.5f) * cell_size,
		};

		int mesh_indices[6][4] =
		{
			{ 7, 3, 1, 5 }, //forward
			{ 2, 6, 4, 0 }, //backward
			{ 3, 7, 6, 2 }, //upward
			{ 5, 1, 0, 4 }, //downward
			{ 6, 7, 5, 4 }, //leftward
			{ 3, 2, 0, 1 }, //rightward
		};

		vector<TexturedVertex> vertices;
		vector<unsigned int> indices;

		for (auto cell : this->cells)
		{
			if (cell.first != cell.second->center_pos)
			{
				//Only run mesh gen for the center pos
				continue;
			}

			if (cell.second->mesh != "")
			{
				GameObject* object =  GameObjectManager::instance->createGameObject();
				object->addComponent<ComponentModel>("cell.second->mesh", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow");
				object->setLocalTransform(Transform((vector3D)cell.second->center_pos * (double)this->cell_size));
				this->render_object->addChild(object);
			}

			for (auto connection : cell.second->connection_points)
			{
				vector3F offset = (vector3F)(connection.first + cell.second->center_pos) * this->cell_size;
				
				for (Direction direction = 0; direction < 6; direction++)
				{
					if (connection.second.direction[direction] == true)
					{
						Direction flipped_direction = flipDirection(direction);
						if (!this->hasConnectionPoint(connection.first + cell.second->center_pos + Directions3B[direction], flipped_direction))
						{
							unsigned int current_index = (unsigned int)vertices.size();
							vertices.push_back({ vertsCube[mesh_indices[direction][0]] + offset, (vector3F) Directions3B[direction], vector2F(0.0f, 0.0f) });
							vertices.push_back({ vertsCube[mesh_indices[direction][1]] + offset, (vector3F) Directions3B[direction], vector2F(1.0f, 0.0f) });
							vertices.push_back({ vertsCube[mesh_indices[direction][2]] + offset, (vector3F) Directions3B[direction], vector2F(1.0f, 1.0f) });
							vertices.push_back({ vertsCube[mesh_indices[direction][3]] + offset, (vector3F) Directions3B[direction], vector2F(0.0f, 1.0f) });

							indices.push_back(current_index + 0);
							indices.push_back(current_index + 1);
							indices.push_back(current_index + 2);

							indices.push_back(current_index + 2);
							indices.push_back(current_index + 3);
							indices.push_back(current_index + 0);
						}
					}
				}
			}
		}

		if (indices.size() != 0)
		{
			TexturedMesh* mesh = new TexturedMesh(vertices, indices);

			this->render_object->addComponent<ComponentModel>("", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow");
			this->render_object->getComponent<ComponentModel>()->temp_mesh = mesh;
		}
	}
}

GameObject* ShipTemplate::getRenderObject()
{
	return this->render_object;
}
