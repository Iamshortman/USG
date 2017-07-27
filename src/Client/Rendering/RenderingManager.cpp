#include "RenderingManager.hpp"

#include "Common/Entity/EntityManager.hpp"
#include "Client/Rendering/LightShaderUtil.hpp"

#include "Client/Component/ComponentModel.hpp"

#include "Client/Rendering/LightManager.hpp"

#include <stack>
#include "Client/Resource/AnimatedMesh.hpp"

RenderingManager::RenderingManager()
{

}

RenderingManager::~RenderingManager()
{

}

void RenderingManager::setWindow(Window* win)
{
	this->window = win;
}

Window* RenderingManager::getWindow()
{
	return this->window;
}

void RenderingManager::Render(Camera* cam)
{
	this->window->clearBuffer();



	this->window->updateBuffer();
}

void RenderingManager::RenderWorld(World* world, Camera* cam)
{
	if (world == nullptr)
	{
		return;
	}

	//Render the larger world first
	//Entity* parent = world->getParent();
	//if (parent != nullptr)
	//{
		//this->RenderWorld(parent->getWorld(), cam);
	//}

	//Now render world
	auto entities = world->getEntitiesInWorld();

	for (auto it = entities->begin(); it != entities->end(); it++)
	{
		Entity* entity = *it;

		if (entity != nullptr)
		{
			if (entity->hasComponent("model"))
			{
				this->RenderMesh(&((ComponentModel*)entity->getComponent("model"))->model, entity->getRenderTransform(), cam, world);
			}
		}

	}


	//TODO Rendering Sub Worlds
	auto subWorlds = world->getSubWorlds();

	for (auto it = subWorlds->begin(); it != subWorlds->end(); it++)
	{
		World* subWorld = *it;
		this->RenderWorld(subWorld, cam);
	}

}

void RenderingManager::RenderMesh(Model* model, Transform globalPos, Camera* cam, World* world)
{
	ShaderProgram* program = model->getShader();
	Mesh* mesh = model->getMesh();
	RenderController* controller = model->getController();
	vector3F ambientLight = world->ambientLight;

	if (mesh == nullptr || program == nullptr)
	{
		return;
	}

	matrix4 projection = cam->getProjectionMatrix(this->window);
	matrix4 view = cam->getOriginViewMatrix();
	matrix4 mvp = projection * view * globalPos.getModleMatrix(cam->getPosition());

	program->setActiveProgram();
	program->setUniform("MVP", mvp);
	program->setUniform("normalMatrix", globalPos.getNormalMatrix());
	program->setUniform("ambientLight", ambientLight);

	GLuint texture = model->getTexture(0);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (controller != nullptr)
	{
		controller->setGLSLUniform(program);
	}

	/*if (model->skeleton != nullptr)
	{
		AnimatedMesh* animMesh = (AnimatedMesh*)mesh;
		std::stack<Bone*> bones;
		bones.push(model->skeleton->rootBone);

		while (!bones.empty())
		{
			Bone* bone = bones.top();
			bones.pop();

			string index = std::to_string(animMesh->boneMap[bone->name]);

			string name = "boneTransforms[" + index + "]";

			program->setUniform(name, bone->animatedTransform);

			for (int i = 0; i < bone->children.size(); i++)
			{
				bones.push(bone->children[i]);
			}
		}
	}*/

	mesh->draw(program);

	program->deactivateProgram();

	//Switch to lighting Shading
	LightSet* lights = LightManager::instance->getLightsForWorld(world->worldId);
	program = model->getLightingShader();
	if (lights != nullptr && program != nullptr)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(false);
		glDepthFunc(GL_EQUAL);

		program->setActiveProgram();

		int directionalCount = lights->directionalLights.size();
		int pointCount = lights->pointLights.size();
		int spotCount = lights->spotLights.size();

		while (directionalCount > 0 || pointCount > 0 || spotCount > 0)
		{
			int count = 0;

			for (count = 0; (count < 8) && (directionalCount > 0); count++)
			{
				setDirectionalLight("directinalLights[0]", program, lights->directionalLights[directionalCount - 1]);
				directionalCount--;
			}
			program->setUniform("directinalCount", count);

			for (count = 0; (count < 8) && (pointCount > 0); count++)
			{
				setPointLight("pointLights[0]", program, lights->pointLights[pointCount - 1], cam->getPosition());
				pointCount--;
			}
			program->setUniform("pointCount", count);


			for (count = 0; (count < 8) && (spotCount > 0); count++)
			{
				setSpotLight("spotLights[0]", program, lights->spotLights[spotCount - 1], cam->getPosition());
				spotCount--;
			}
			program->setUniform("spotCount", count);

			program->setActiveProgram();
			program->setUniform("MVP", mvp);
			program->setUniform("normalMatrix", globalPos.getNormalMatrix());
			program->setUniform("ambientLight", ambientLight);

			GLuint texture = model->getTexture(0);
			glBindTexture(GL_TEXTURE_2D, texture);

			if (controller != nullptr)
			{
				controller->setGLSLUniform(program);
			}

			mesh->draw(program);
		}

		program->deactivateProgram();

		glDepthFunc(GL_LESS);
		glDepthMask(true);
		glDisable(GL_BLEND);
	}

}