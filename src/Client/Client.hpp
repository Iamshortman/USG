#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Client/Rendering/RenderingManager.hpp"
#include "Client/Rendering/Window.hpp"

#include "Client/GameState/GameState.hpp"

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

	void setGameState(GameState* state);

	Window* window;
	RenderingManager* renderingManager;

private:
	bool shouldClose = false;

	GameState* currentState = nullptr;
	GameState* previousState = nullptr;
};

#endif //CLIENT_HPP
