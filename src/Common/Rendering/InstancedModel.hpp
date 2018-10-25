#ifndef INSTANCED_MODEL_HPP
#define INSTANCED_MODEL_HPP 

#include "Common/Transform.hpp"
#include "Common/Types.hpp"

class InstancedModel
{
public:
	InstancedModel(string instanced_model_name);
	virtual ~InstancedModel();

	string getInstancedModel() const { return this->instanced_model_name; };

protected:
	string instanced_model_name;
};

#endif //INSTANCED_MODEL_HPP