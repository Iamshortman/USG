#include "NodeSystem.hpp"

#include "Common/Logger/Logger.hpp"

void NodeSystem::update(EntityManager& es, EventManager& events, TimeDelta dt)
{
}

void NodeSystem::configure(entityx::EventManager& event_manager)
{
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

	if (parent.has_component<NodeHost>())
	{
		ComponentHandle<NodeHost> node_host = parent.component<NodeHost>();
		node_host->child_list.erase(entity);
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
		child.destroy();
	}
}
