#ifndef RESOURCE_POOL_HPP
#define RESOURCE_POOL_HPP

#include <unordered_map>

#include "Common/Types.hpp"

template <class T>
struct Resource
{
	int usingCount = 0;
	T resource;
};

template <class T>
class ResourcePool
{
public:

	void setUsing(string name)
	{
		if (this->hasResource(name))
		{
			this->resources[name].usingCount++;
		}
		else
		{
			Resource<T> resource;
			resource.usingCount = 1;
			resource.resource = this->loadResource(name);

			this->resources[name] = resource;
		}
	};

	int getUsing(string name)
	{
		//Return -1 if it is not loaded
		if (!this->hasResource(name))
		{
			return -1;
		}

		return this->resources[name].usingCount;
	};

	void releaseUsing(string name)
	{
		if (this->hasResource(name))
		{
			this->resources[name].usingCount--;

			//If no one is using it, unload it
			if (this->resources[name].usingCount <= 0)
			{
				this->unloadResource(this->resources[name].resource);
				this->resources.erase(name);
			}
		}
	};

	T get(string name)
	{
		T value = T();

		if (this->hasResource(name))
		{
			value = this->resources[name].resource;
		}

		return value;
	};

protected:
	bool hasResource(string name)
	{
		return this->resources.find(name) != this->resources.end();
	};

	virtual T loadResource(string name) = 0;
	virtual void unloadResource(T resource) = 0;

	std::unordered_map<string, Resource<T>> resources;
};


#endif //RESOURCE_POOL_HPP