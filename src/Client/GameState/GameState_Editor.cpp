#include "Client/GameState/GameState_Editor.hpp"
#include "Client/Client.hpp"

#include "Common/GameObject.hpp"
#include "Common/GameObjectManager.hpp"
#include "Client/Rendering/Camera.hpp"
#include "Common/Component/ComponentModel.hpp"
#include "Client/Component/DebugCamera.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Input/InputManager.hpp"

GameState_Editor::GameState_Editor()
{
	this->scene_root = GameObjectManager::createGameObject();

	scene_camera = GameObjectManager::createGameObject();
	this->scene_root->addChild(scene_camera);
	scene_camera->addComponent<Camera>();
	scene_camera->addComponent<DebugCamera>(5.0, 1.0);
	scene_camera->setLocalTransform(Transform(vector3D(0.0, 2.0, -8.0)));

	cursor = GameObjectManager::createGameObject();

	MeshPool::instance->loadMesh("res/editor/cursor.obj");
	cursor->addComponent<ComponentModel>("res/editor/cursor.obj", "res/textures/1K_Grid.png", "Textured", "Textured_Lighting", "Textured_Shadow");

	this->scene_root->addChild(cursor);

	this->ship_template = new ShipTemplate(this->cube_size);
}

GameState_Editor::~GameState_Editor()
{
	delete this->scene_root;
}

void GameState_Editor::update(Client* client, double delta_time)
{
	if (InputManager::instance->getButtonPressed("editor_forward"))
	{
		this->cursor_position.z++;
	}
	if (InputManager::instance->getButtonPressed("editor_backward"))
	{
		this->cursor_position.z--;
	}
	if (InputManager::instance->getButtonPressed("editor_left"))
	{
		this->cursor_position.x++;
	}
	if (InputManager::instance->getButtonPressed("editor_right"))
	{
		this->cursor_position.x--;
	}
	if (InputManager::instance->getButtonPressed("editor_up"))
	{
		this->cursor_position.y++;
	}
	if (InputManager::instance->getButtonPressed("editor_down"))
	{
		this->cursor_position.y--;
	}

	vector3D position = ((vector3D)this->cursor_position) * this->cube_size;
	this->cursor->setLocalTransform(Transform(position));

	if (InputManager::instance->getButtonPressed("editor_place"))
	{
		if (!this->ship_template->hasCellAt(this->cursor_position))
		{
			ShipCell* cell = new ShipCell();
			cell->points.insert(vector3B(0));
			Connection connection;
			for (int i = 0; i < 6; i++)
				connection.direction[i] = true;

			cell->connection_points[vector3B(0)] = connection;

			if (this->ship_template->canPlaceCell(this->cursor_position, cell))
			{
				this->ship_template->addCell(this->cursor_position, cell);
			}
			else
			{
				delete cell;
			}
		}
		else
		{
			this->ship_template->removeCell(this->cursor_position);
		}
	}

	if (this->ship_template->hasChanged())
	{
		if (this->ship_template->getRenderObject() != nullptr)
		{
			this->scene_root->removeChild(this->ship_template->getRenderObject());
		}
		this->ship_template->rebuildRenderObject();
		this->scene_root->addChild(this->ship_template->getRenderObject());
	}

	this->scene_root->update(delta_time);

	client->renderingManager->renderScene(scene_root, scene_camera->getComponent<Camera>());
}