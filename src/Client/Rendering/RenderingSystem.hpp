#ifndef RENDERING_SYSTEM_HPP
#define RENDERING_SYSTEM_HPP

#include "Common/EntityX_Include.hpp"

class RenderingSystem : public System<RenderingSystem>
{
public:
	RenderingSystem();
	~RenderingSystem();
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

private:

};

RenderingSystem::RenderingSystem()
{
}

RenderingSystem::~RenderingSystem()
{
}

#include "Client/Client.hpp"
#include "Common/Rendering/Model.hpp"
#include "Common/Transforms.hpp"


inline void RenderingSystem::update(EntityManager& es, EventManager& events, TimeDelta dt)
{
	ComponentHandle<Model> model_pattern;
	for (Entity entity : es.entities_with_components(model_pattern))
	{
		ComponentHandle<Model> model = entity.component<Model>();
		Transform global_transform = Transforms::getLocalTransform(entity);

		Client::instance->renderingManager->addModel(model.get(), global_transform);
	}
}

#endif //PHYSICS_SYSTEM_HPP