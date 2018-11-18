#include "DebugCamera.hpp"

#include "Common/Entity/Entity.hpp"
#include "Client/Input/InputManager.hpp"

//Temp
#include "Common/Physics/PhysicsWorld.hpp"
#include "Common/World/World.hpp"
#include "Common/Entity/NodeEntity.hpp"

DebugCamera::DebugCamera(Entity* entity, double linear, double angular)
	:ComponentEntity(entity)
{
	linearSpeed = linear;
	angularSpeed = angular;
}
void DebugCamera::update(double delta_time)
{
	if (this->enabled)
	{
		Transform transform = this->parent_entity->getLocalTransform();
		
		vector3D position = transform.getPosition();
		position += (transform.getForward() * InputManager::getInstance()->getButtonAxisCombo("Debug_ForwardBackward", "Debug_Forward", "Debug_Backward")) * this->linearSpeed * delta_time;
		position += (transform.getUp() * InputManager::getInstance()->getButtonAxisCombo("Debug_UpDown", "Debug_Up", "Debug_Down")) * this->linearSpeed * delta_time;
		position += (transform.getLeft() * InputManager::getInstance()->getButtonAxisCombo("Debug_LeftRight", "Debug_Left", "Debug_Right")) * this->linearSpeed * delta_time;
		transform.setPosition(position);

		quaternionD orientation = transform.getOrientation();
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("Debug_Pitch", "Debug_PitchUp", "Debug_PitchDown", false) * this->angularSpeed * (M_PI * 2.0) * delta_time, transform.getLeft()) * orientation;
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("Debug_Yaw", "Debug_YawLeft", "Debug_YawRight", false) * this->angularSpeed * (M_PI * 2.0) * delta_time, transform.getUp()) * orientation;
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("Debug_Roll", "Debug_RollRight", "Debug_RollLeft", false) * this->angularSpeed * (M_PI) * delta_time, transform.getForward()) * orientation;
		transform.setOrientation(orientation);

		this->parent_entity->setLocalTransform(transform);


		//TEMP CODE
		if (InputManager::getInstance()->getButtonPressed("Debug_Interact"))
		{
			World* world = this->parent_entity->getWorld();

			double rayDistance = 10.0;
			vector3D startPos = transform.getPosition();
			vector3D endPos = startPos + (transform.getForward() * rayDistance);
			SingleRayTestResult result = world->singleRayTest(startPos, endPos);

			if (result.hasHit)
			{
				if (result.entity != nullptr)
				{
					printf("Hit");

					if (result.node != nullptr && result.entity->getType() == EntityType::NODE_ENTITY)
					{						
						printf(" Node");

						NodeEntity* node_entity = (NodeEntity*)result.entity;
						
						Node* node = result.node->getParent();

						if (node != nullptr)
						{
							//node->removeChild(result.node);
						}
						else
						{
							//node_entity->removeChild(result.node);
						}

					}

				}
			}
		}

	}
}


