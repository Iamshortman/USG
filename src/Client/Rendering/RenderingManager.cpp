#include "RenderingManager.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"

#include "Common/World/World.hpp"

#include "Common/Entity/NodeEntity.hpp"

#include <stack>
#include <set>

RenderingManager::RenderingManager(Window* window)
{
	this->window = window;

	vector2I window_size = this->window->getWindowSize();

	this->skybox = new Skybox("res/textures/Skybox/space", "res/shaders/Skybox");

	this->rendering_system = new RenderingSystem();
	this->rendering_system->setBufferSize(window_size);
}

RenderingManager::~RenderingManager()
{
	delete this->rendering_system;
}

void RenderingManager::SubmitWorld(World* world)
{
	if (world == nullptr)
	{
		return;
	}

	auto entities = world->getEntitiesInWorld();
	for (int i = 0; i < entities.size(); i++)
	{
		Entity* entity = entities[i];
		if (entity == nullptr)
		{
			continue;
		}

		if (entity->hasSubWorld())
		{
			this->SubmitWorld(entity->getSubWorld());
		}

		if (entity != nullptr)
		{

			Model* model = entity->getNodeComponent<Model>();
			if (model != nullptr)
			{
				this->rendering_system->addModel(model, model->parent_node->getGlobalTransform());
			}

			PointLight* point = entity->getNodeComponent<PointLight>();
			if (point != nullptr && point->getEnabled())
			{
				this->rendering_system->addPointLight(point, point->parent_node->getGlobalTransform());
			}

			SpotLight* spot = entity->getNodeComponent<SpotLight>();
			if (spot != nullptr && spot->getEnabled())
			{
				this->rendering_system->addSpotLight(spot, spot->parent_node->getGlobalTransform());
			}

			if (entity->getType() == EntityType::NODE_ENTITY)
			{
				NodeEntity* node_entity = (NodeEntity*)entity;
				//TODO: something clever

				std::stack<Node*> nodes;
				for (Node* child : node_entity->getChildNodes())
				{
					nodes.push(child);
				}

				while (!nodes.empty())
				{
					Node* node = nodes.top();
					nodes.pop();
					
					Model* model = node->getNodeComponent<Model>();
					if (model != nullptr)
					{
						this->rendering_system->addModel(model, model->parent_node->getGlobalTransform());
					}

					PointLight* point = node->getNodeComponent<PointLight>();
					if (point != nullptr && point->getEnabled())
					{
						this->rendering_system->addPointLight(point, point->parent_node->getGlobalTransform());
					}

					SpotLight* spot = node->getNodeComponent<SpotLight>();
					if (spot != nullptr && spot->getEnabled())
					{
						this->rendering_system->addSpotLight(spot, spot->parent_node->getGlobalTransform());
					}

					for (Node* child : node->getChildNodes())
					{
						nodes.push(child);
					}
				}

			}
		}
	}
}

void RenderingManager::Render(World* world, Camera* camera)
{
	DirectionalLight light = DirectionalLight(vector3F(0.1f, -1.0f, -0.25f), vector3F(1.0f), 0.4f);

	this->rendering_system->clearScene();
	this->rendering_system->addDirectionalLight(&light);

	vector2I window_size = this->window->getWindowSize();

	this->rendering_system->setBufferSize(window_size);

	World* base_world = world;

	while (base_world->getParent() != nullptr)
	{
		base_world = base_world->getParent()->getWorld();
	}

	this->SubmitWorld(base_world);
	
	this->rendering_system->setSkybox(this->skybox);

	this->rendering_system->render(0, camera);

	this->window->updateBuffer();
}