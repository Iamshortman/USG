#ifndef MODELPOOL_HPP
#define MODELPOOL_HPP

#include <hash_map>
#include <string>
#include "Common/Resource/Mesh.hpp"

#include "Client/Resource/TexturedMesh.hpp"
#include "Client/Resource/AnimatedMesh.hpp"

//Temp class
class AnimatedMesh;

using std::string;

struct StaticMeshResource
{
	int usingCount = 0;
	Mesh* mesh;
};

struct AnimatedMeshResource
{
	int usingCount = 0;
	AnimatedMesh* mesh;
};

//Loads and stores all shaders using in game.
class MeshPool
{

public:
	static MeshPool* instance;

	MeshPool();
	~MeshPool();
	void loadModel(string name, string filename, bool isStatic);
	void unloadModel(string name);

	void setUsing(string name);
	int getUsing(string name);
	void releaseUsing(string name);
	Mesh* getModel(string name);

private:
	std::hash_map<string, StaticMeshResource> staticMeshes;
	std::hash_map<string, AnimatedMeshResource> animatedMesh;
};


#endif //MODELPOOL_HPP