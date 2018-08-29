#ifndef RENDERING_SYSTEM_HPP
#define RENDERING_SYSTEM_HPP

#include "Common/EntityX_Include.hpp"

class RenderingSystem : public System<RenderingSystem>
{
public:
	RenderingSystem();
	~RenderingSystem();
	void update(EntityManager &es, EventManager &events, TimeDelta delta_time) override;

private:

};

RenderingSystem::RenderingSystem()
{
	Client::instance->rendering_engine->setSkybox(new Skybox("res/textures/Skybox/space", "res/shaders/Skybox"));
}

RenderingSystem::~RenderingSystem()
{
}

#include "Client/Client.hpp"
#include "Common/Rendering/Model.hpp"
#include "Common/Transforms.hpp"

inline void RenderingSystem::update(EntityManager& es, EventManager& events, TimeDelta delta_time)
{
	ComponentHandle<Model> model_pattern;
	for (Entity entity : es.entities_with_components(model_pattern))
	{
		WorldId world_id = Transforms::getWorldId(entity);
		if (world_id != INVALID_WORLD)
		{
			ComponentHandle<Model> model = entity.component<Model>();
			Transform global_transform = Transforms::getGlobalTransform(entity);

			Client::instance->rendering_engine->addModel(model.get(), global_transform);
		}
	}

	ComponentHandle<Camera> camera_pattern;
	for (Entity entity : es.entities_with_components(camera_pattern))
	{
		ComponentHandle<Camera> camera = entity.component<Camera>();

		Transform global_transform = Transforms::getGlobalTransform(entity);

		Client::instance->rendering_engine->setBufferSize(Client::instance->window->getWindowSize());

		Client::instance->window->clearBuffer();
		Client::instance->rendering_engine->render(0, camera.get(), global_transform);
		Client::instance->window->updateBuffer();
		Client::instance->rendering_engine->clearScene();


		break; //Use the first camera right now.
	}
}

#endif //PHYSICS_SYSTEM_HPP