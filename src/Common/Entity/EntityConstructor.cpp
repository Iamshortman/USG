#include "EntityConstructor.hpp"

#include "Common/Entity/I_Node.hpp"

#include "jsoncons/json.hpp"
using jsoncons::json;

//Node Components
#include "Common/Rendering/Model.hpp"
#include "Common/Physics/CollisionShape.hpp"
#include "Common/Component/Mass.hpp"
#include "Common/Component/ProjectileLauncher.hpp"
#include "Common/Component/SeatInteract.hpp"

//Entity Componets
#include "Common/Component/CharacterController.hpp"
#include "Common/Component/ComponentShipFlight.hpp"
#include "Common/Component/ShipController.hpp"

Transform jsonTransform(json json_transform)
{
	Transform transform;
	if (json_transform.has_member("position"))
	{
		vector<double> pos = json_transform["position"].as<vector<double>>();
		transform.setPosition(vector3D(pos[0], pos[1], pos[2]));
	}

	if (json_transform.has_member("orientation"))
	{
		vector<double> pos = json_transform["orientation"].as<vector<double>>();
		transform.setOrientation(quaternionD(pos[0], pos[1], pos[2], pos[3]));
	}

	return transform;
}

void jsonNodeComponents(I_Node* node, json json_node_components)
{
	if (json_node_components.has_member("model"))
	{
		json json_model = json_node_components["model"];
		//string mesh, string texture, string ambient_shader
		string file_path = json_model["file_path"].as_string();
		string texture = json_model["texture"].as_string();
		string ambient_shader = json_model["ambient_shader"].as_string();
		string shadow_shader = json_model["shadow_shader"].as_string();
		node->addNodeComponent<Model>(file_path, texture, ambient_shader, shadow_shader);
	}

	if (json_node_components.has_member("mass"))
	{
		node->addNodeComponent<Mass>(json_node_components["mass"].as_double());
	}

	if (json_node_components.has_member("collision_shape"))
	{
		json json_shape = json_node_components["collision_shape"];

		if (json_shape.has_member("box"))
		{
			vector<double> size = json_shape["box"].as<vector<double>>();
			node->addNodeComponent<CollisionShape>(CollisionShapeType::Box, vector3D(size[0], size[1], size[2]));
		}
		else if (json_shape.has_member("capsule"))
		{
			vector<double> size = json_shape["capsule"].as<vector<double>>();
			node->addNodeComponent<CollisionShape>(CollisionShapeType::Capsule, vector2D(size[0], size[1]));
		}
		else if (json_shape.has_member("convex"))
		{
			string file = json_shape["convex"].as_string();
			node->addNodeComponent<CollisionShape>(CollisionShapeType::ConvexMesh, file);
		}
		else if (json_shape.has_member("concave"))
		{
			string file = json_shape["concave"].as_string();
			node->addNodeComponent<CollisionShape>(CollisionShapeType::ConcaveMesh, file);
		}
	}

	if (json_node_components.has_member("projectile_launcher"))
	{
		node->addNodeComponent<ProjectileLauncher>();
	}

	if (json_node_components.has_member("seat_interact"))
	{
		node->addNodeComponent<SeatInteract>();
	}
}

void jsonEntityComponents(Entity* entity, json json_entity_components)
{
	if (json_entity_components.has_member("character_controller"))
	{
		entity->addComponent<CharacterController>();
	}

	if (json_entity_components.has_member("flight_controller"))
	{
		entity->addComponent<ShipFlightController>();
	}

	if (json_entity_components.has_member("ship_controller"))
	{
		entity->addComponent<ShipController>();
	}
}

Node* jsonNode(json json_node)
{
	Node* node = new Node();

	if (json_node.has_member("transform"))
	{
		node->setLocalTransform(jsonTransform(json_node["transform"]));
	}

	if (json_node.has_member("node_components"))
	{
		jsonNodeComponents(node, json_node["node_components"]);
	}

	if (json_node.has_member("children"))
	{
		json children = json_node["children"];
		for (int i = 0; i < children.size(); i++)
		{
			node->addChild(jsonNode(children[i]));
		}
	}

	return node;
}

Entity* EntityConstructor::buildEntityFromJson(string file_path)
{
	Entity* entity = nullptr;
	json json_file;
	string entity_string;

	try
	{
		json temp = json::parse_file(file_path);
		json_file = temp;
	}
	catch (jsoncons::parse_error e)
	{
		printf("Error: %s\n", e.what());
		return nullptr;
	}

	if (json_file.has_member("Entity"))
	{
		entity = EntityManager::getInstance()->createEntity();
		entity_string = "Entity";
	}
	else if (json_file.has_member("NodeEntity"))
	{
		entity = EntityManager::getInstance()->createNodeEntity();
		entity_string = "NodeEntity";
	}

	if (entity != nullptr)
	{
		json json_entity = json_file[entity_string];
		if (json_entity.has_member("transform"))
		{
			entity->setLocalTransform(jsonTransform(json_entity["transform"]));
		}

		if (json_entity.has_member("rigidBody"))
		{
			json rigid_body = json_entity["rigidBody"];
			entity->addRigidBody(); //TODO rigidBody settings

			if (rigid_body.has_member("mass"))
			{
				entity->getRigidBody()->setMass(rigid_body["mass"].as_double());
			}

			if (rigid_body.has_member("inertia_tensor"))
			{
				vector<double> pos = rigid_body["inertia_tensor"].as<vector<double>>();
				entity->getRigidBody()->setInertiaTensor(vector3D(pos[0], pos[1], pos[2]));
			}

		}

		if (json_entity.has_member("node_components"))
		{
			jsonNodeComponents(entity, json_entity["node_components"]);
		}

		if (json_entity.has_member("entity_components"))
		{
			jsonEntityComponents(entity, json_entity["entity_components"]);
		}

		if (entity->getType() == EntityType::NODE_ENTITY)
		{
			NodeEntity* node_entity = (NodeEntity*)entity;
			if (json_entity.has_member("children"))
			{
				json children = json_entity["children"];
				for (int i = 0; i < children.size(); i++)
				{
					node_entity->addChild(jsonNode(children[i]));
				}
			}
		}

	}

	return entity;
}
