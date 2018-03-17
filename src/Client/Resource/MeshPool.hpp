#ifndef MODELPOOL_HPP
#define MODELPOOL_HPP

#include <unordered_map>
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

//Loads and stores all shaders using in game.
class MeshPool
{

public:
	static MeshPool* instance;

	MeshPool();
	~MeshPool();
	void loadMesh(string filename);
	void unloadMesh(string filename);

	void setUsing(string filename);
	int getUsing(string filename);
	void releaseUsing(string filename);
	Mesh* getMesh(string filename);

private:
	bool hasMesh(string filename);


	std::unordered_map<string, StaticMeshResource> staticMeshes;
	//std::unordered_map<string, AnimatedMeshResource> animatedMesh;
};


#endif //MODELPOOL_HPP