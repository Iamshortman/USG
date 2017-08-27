#ifndef SHADERPOOL_HPP
#define SHADERPOOL_HPP

#include <unordered_map>
#include <string>

#include "Client/Rendering/ShaderProgram.hpp"

using std::string;

struct ShaderResource
{
	int usingCount = 0;
	ShaderProgram* program;
};

//Loads and stores all shaders using in game.
class ShaderPool
{

public:
	static ShaderPool* instance;

	ShaderPool();
	~ShaderPool();
	void loadShader(string name, string vertex, string fragment, vector<AttributeLocation> attributeLocation);
	void unloadShader(string name);

	void setUsing(string name);
	int getUsing(string name);
	void releaseUsing(string name);
	ShaderProgram* getShader(string name);

private:
	std::unordered_map<string, ShaderResource> shaders;
};


#endif //SHADERPOOL_HPP