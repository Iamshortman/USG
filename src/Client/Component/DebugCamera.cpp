#include "DebugCamera.hpp"

#include "Common/Transforms.hpp"
#include "Client/Input/InputManager.hpp"

void DebugCameraSystem::update(EntityManager& es, EventManager& events, TimeDelta delta_time)
{
	ComponentHandle<DebugCamera> debug_pattern;
	for (Entity entity : es.entities_with_components(debug_pattern))
	{
		ComponentHandle<DebugCamera> debug_camera = entity.component<DebugCamera>();
		Transform transform = Transforms::getLocalTransform(entity);

		vector3D position = transform.getPosition();
		position += (transform.getForward() * InputManager::getInstance()->getButtonAxisCombo("Flight_ForwardBackward", "Flight_Forward", "Flight_Backward")) * debug_camera->linear_speed * delta_time;
		position += (transform.getUp() * InputManager::getInstance()->getButtonAxisCombo("Flight_UpDown", "Flight_Up", "Flight_Down")) * debug_camera->linear_speed * delta_time;
		position += (transform.getLeft() * InputManager::getInstance()->getButtonAxisCombo("Flight_LeftRight", "Flight_Left", "Flight_Right")) * debug_camera->linear_speed * delta_time;
		transform.setPosition(position);

		quaternionD orientation = transform.getOrientation();
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("Flight_Pitch", "Flight_PitchUp", "Flight_PitchDown") * debug_camera->angular_speed * (M_PI * 2.0) * delta_time, transform.getLeft()) * orientation;
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("Flight_Yaw", "Flight_YawLeft", "Flight_YawRight") * debug_camera->angular_speed * (M_PI * 2.0) * delta_time, transform.getUp()) * orientation;
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("Flight_Roll", "Flight_RollRight", "Flight_RollLeft") * debug_camera->angular_speed * (M_PI)* delta_time, transform.getForward()) * orientation;
		transform.setOrientation(orientation);

		Transforms::setLocalTransform(entity, transform);
	}
}
