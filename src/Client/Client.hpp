#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Client/Rendering/RenderingManager.hpp"
#include "Client/Rendering/Window.hpp"

class Client
{

public:
	//Instance for the Singleton design pattern;
	static Client* instance;

	Client();
	virtual ~Client();

	void update(double deltaTime);
	void exitGame();
	const bool getShouldClose();

	Window* window;
	RenderingManager* renderingManager;

	World* tempWorld = nullptr;
	Camera* tempCamera = nullptr;

protected:

private:
	bool shouldClose = false;

	Entity* debugCamera;
};

#endif //CLIENT_HPP
