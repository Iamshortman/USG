#include "NodeSystem.hpp"

#include "Common/Logger/Logger.hpp"

void NodeSystem::update(EntityManager& es, EventManager& events, TimeDelta dt)
{
}

void NodeSystem::configure(entityx::EventManager& event_manager)
{
	event_manager.subscribe<ComponentAddedEvent<Node>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<Node>>(*this);

	event_manager.subscribe<ComponentAddedEvent<NodeHost>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<NodeHost>>(*this);
}

void NodeSystem::receive(const ComponentAddedEvent<Node>& event)
{
	Entity entity = event.entity;
	ComponentHandle<Node> node = event.component;
	Entity parent = node->parent_entity;

	if (parent.has_component<NodeHost>())
	{
		ComponentHandle<NodeHost> node_host = parent.component<NodeHost>();
		node_host->child_list.insert(entity);
	}
	else
	{
		Logger::getInstance()->logError("No Node on Parent\n");
		entity.remove<Node>();
	}
}

void NodeSystem::receive(const ComponentRemovedEvent<Node>& event)
{
	Entity entity = event.entity;
	ComponentHandle<Node> node = event.component;
	Entity parent = node->parent_entity;

	if (parent.valid())
	{
		if (parent.has_component<NodeHost>())
		{
			ComponentHandle<NodeHost> node_host = parent.component<NodeHost>();
			node_host->child_list.erase(entity);
		}
	}
}

void NodeSystem::receive(const ComponentAddedEvent<NodeHost>& event)
{
	//Do nothing
}

void NodeSystem::receive(const ComponentRemovedEvent<NodeHost>& event)
{
	ComponentHandle<NodeHost> node_host = event.component;

	for (Entity child : node_host->child_list)
	{
		if (child.valid())
		{
			EntityDestroyer::destroyEntity(child);
		}
	}
}
