#include "Common/Entity/EntityNode.hpp"

#include "Common/Entity/ComponentModel.hpp"
#include "Common/Entity/ComponentCollisionShape.hpp"
#include "Common/Physics/CollisionShapes/ConvexMeshShape.hpp"
#include "Common/Entity/ComponentRotate.hpp"

EntityNode::EntityNode(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, RIGIDBODYTYPE::COMPOUND);

	this->rootNode = new Node();
	this->rootNode->addComponent(new ComponentModel("res/models/Cobra/Hull.obj", "Textured", "", "res/textures/1K_Grid.png"));
	this->rootNode->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Hull.obj"), 10000.0));

	Node* right_engine = new Node();
	right_engine->setLocalTransform(Transform(vector3D(-1.7, 0.0, -4.25)));
	right_engine->addComponent(new ComponentModel("res/models/Cobra/Engine.obj", "Textured", "", "res/textures/1K_Grid.png"));
	right_engine->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Engine.obj"), 5000.0));
	{
		Node* right_wing = new Node();
		right_wing->setLocalTransform(Transform(vector3D(-2.5, 0.0, 0.7)));
		right_wing->addComponent(new ComponentModel("res/models/Cobra/Wing.obj", "Textured", "", "res/textures/1K_Grid.png"));
		right_wing->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Wing.obj"), 500.0));
		right_engine->addChild(right_wing);
	}
	this->rootNode->addChild(right_engine);

	Node* left_engine = new Node();
	left_engine->setLocalTransform(Transform(vector3D(1.7, 0.0, -4.25), quaternionD(0.0, 0.0, 0.0, 1.0)));
	left_engine->addComponent(new ComponentModel("res/models/Cobra/Engine.obj", "Textured", "", "res/textures/1K_Grid.png"));
	left_engine->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Engine.obj"), 5000.0));
	{
		Node* left_wing = new Node();
		left_wing->setLocalTransform(Transform(vector3D(-2.5, 0.0, 0.7)));
		left_wing->addComponent(new ComponentModel("res/models/Cobra/Wing.obj", "Textured", "", "res/textures/1K_Grid.png"));
		left_wing->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Wing.obj"), 500.0));
		left_engine->addChild(left_wing);
	}
	this->rootNode->addChild(left_engine);

	Node* canopy = new Node();
	canopy->setLocalTransform(Transform(vector3D(0.0, 1.3, -2.6)));
	canopy->addComponent(new ComponentModel("res/models/Cobra/Canopy_Outside.obj", "Textured", "", "res/textures/1K_Grid.png"));
	canopy->addComponent(new ComponentRotate(quaternionD(), quaternionD(0.906, -0.423, 0.0, 0.0), 5.0));
	canopy->addComponent(new ComponentCollisionShape(new ConvexMeshShape("res/models/Cobra/Canopy_Outside.obj"), 100.0));
	this->rootNode->addChild(canopy);

	Node* cockpit =  new Node();
	cockpit->addComponent(new ComponentModel("res/models/Cobra/Cockpit.obj", "Textured", "", "res/textures/1K_Grid.png"));
	this->rootNode->addChild(cockpit);

	//this->rigidBody->setMass(10000.0);
	this->rootNode->addToEntity(this);
}

EntityNode::~EntityNode()
{
	delete this->rootNode;
}

void EntityNode::update(double deltaTime)
{
	this->rootNode->update(deltaTime);
}

void EntityNode::interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal, int bodyId)
{
	printf("BodyId: %d\n", bodyId);

	Node* node = this->rigidBody->getChildNode(bodyId);
	if (node != nullptr)
	{
		printf("Help!\n");
		if (node->getParent() != nullptr)
		{
			Transform temp = node->getTransform().transformBy(this->getTransform());
			node->getParent()->removeChild(node);

			EntityNode* newNode = (EntityNode*)EntityManager::instance->createEntity(ENTITYTYPE::ENTITY_NODE);

			delete newNode->rootNode;
			newNode->rootNode = node;
			node->setLocalTransform(Transform());
			newNode->rootNode->addToEntity(newNode);

			newNode->setTransform(temp);
			newNode->addToWorld(this->world);
		}
	}
}

ENTITYTYPE EntityNode::getEntityType() const
{
	return ENTITYTYPE::ENTITY_NODE;
}