#include "Common/GameObjectManager.hpp"

#include "Common/Component/ComponentModel.hpp"
#include "Common/Physics/RigidBody.hpp"
#include "Common/Component/ComponentShipFlight.hpp"
#include "Common/Component/ComponentMass.hpp"

#include "jsoncons/json.hpp"
using jsoncons::json;

GameObjectManager* GameObjectManager::instance = nullptr;

GameObjectManager* GameObjectManager::getInstance()
{
	if (GameObjectManager::instance == nullptr)
	{
		GameObjectManager::instance = new GameObjectManager();
	}

	return GameObjectManager::instance;
}

GameObjectManager::GameObjectManager()
{
}

void GameObjectManager::update()
{
	while (!this->game_objects_to_delete.empty())
	{
		GameObjectId gameObjectId = this->game_objects_to_delete.top();

		if (this->game_objects.count(gameObjectId))
		{
			delete this->game_objects[gameObjectId];
			this->game_objects.erase(gameObjectId);
		}

		this->game_objects_to_delete.pop();
	}
}

GameObjectId GameObjectManager::getNextId()
{
	GameObjectId game_object_id = this->next_id;

	while (this->game_objects.find(game_object_id) != this->game_objects.end())
	{
		this->next_id++;
		game_object_id = this->next_id;
	}

	this->next_id++;
	return game_object_id;
}

GameObject* GameObjectManager::createGameObject()
{
	GameObjectId game_object_id = this->getNextId();

	GameObject* game_object = new GameObject(game_object_id);
	this->game_objects[game_object_id] = game_object;
	return game_object;
}
 
GameObject* GameObjectManager::getGameObject(GameObjectId game_object_id)
{
	if (this->game_objects.find(game_object_id) == this->game_objects.end())
	{
		return nullptr;
	}

	return this->game_objects[game_object_id];
}

void GameObjectManager::destroyGameObject(GameObjectId game_object_id)
{
	this->game_objects_to_delete.push(game_object_id);
}

void parseComponents(GameObject* game_object, json json_component)
{
	if (json_component.has_member("model"))
	{
		json json_model = json_component["model"];
		//string mesh, string texture, string ambient_shader
		string file_path = json_model["file_path"].as_string();
		string texture = json_model["texture"].as_string();
		string ambient_shader = json_model["ambient_shader"].as_string();
		game_object->addComponentNoEnable<ComponentModel>(file_path, texture, ambient_shader, "", "");
	}
	
	if (json_component.has_member("rigidBody"))
	{
		game_object->addComponentNoEnable<RigidBody>();

		if (json_component.has_member("inertiaTensor"))
		{
			vector<double> pos = json_component["rigidBody"]["inertiaTensor"].as<vector<double>>();
			//game_object->getComponent<RigidBody>()->setInertiaTensor(vector3D(pos[0], pos[1], pos[2]));
		}
	}

	if (json_component.has_member("mass"))
	{
		double mass = json_component["mass"].as_double();
		game_object->addComponentNoEnable<ComponentMass>(mass);
	}

	if (json_component.has_member("ComponentShipFlight"))
	{
		game_object->addComponent<ComponentShipFlight>();
	}

	
}

GameObject* parseGameObject(json json_game_object)
{
	GameObject* game_object = GameObjectManager::getInstance()->createGameObject();

	if (json_game_object.has_member("transform"))
	{
		Transform transform = game_object->getLocalTransform();
		json json_transform = json_game_object["transform"];

		if (json_transform.has_member("position"))
		{
			vector<double> pos = json_transform["position"].as<vector<double>>();
			transform.setPosition(vector3D(pos[0], pos[1], pos[2]));
		}
		
		if (json_transform.has_member("orientation"))
		{
			vector<double> orientation = json_transform["orientation"].as<vector<double>>();
			transform.setOrientation(quaternionD(orientation[0], orientation[1], orientation[2], orientation[3]));
		}

		game_object->setLocalTransform(transform);
	}

	if (json_game_object.has_member("components"))
	{
		json components = json_game_object["components"];
		parseComponents(game_object, components);
	}

	if (json_game_object.has_member("children"))
	{
		json children = json_game_object["children"];
		for (int i = 0; i < children.size(); i++)
		{
			game_object->addChild(parseGameObject(children[i]));
		}
	}

	return game_object;
}

GameObject* GameObjectManager::createGameObjectFromJson(string file)
{
	json json_file = json::parse_file(file);

	if (json_file.has_member("GameObject"))
	{
		json game_object = json_file["GameObject"];
		return parseGameObject(game_object);
	}

	return nullptr;
}
