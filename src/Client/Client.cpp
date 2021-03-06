#include "Client/Client.hpp"

#include "Client/Input/InputManager.hpp"

#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/TexturedMesh.hpp"

#include "Client/GameState/GameState_Singleplayer.hpp"

Client* Client::instance = nullptr;

Client::Client()
{
	Client::instance = this;

	//Adjust resolution for screen size
	SDL_DisplayMode dm;
	SDL_GetDesktopDisplayMode(0, &dm);
	if (dm.w > 1920 && dm.h > 1080)
	{
		this->window = new Window(2560, 1440, "USG");
	}
	else if (dm.w > 1600 && dm.h > 900)
	{
		this->window = new Window(1600, 900, "USG");
	}
	else
	{
		this->window = new Window(800, 400, "USG");
	}



	this->renderingManager = new RenderingManager(this->window);

	this->setGameState(new GameState_Singleplayer());
}

Client::~Client()
{
	delete this->currentState;
	delete this->window;
	delete this->renderingManager;
}

void Client::update(double deltaTime)
{
	InputManager::getInstance()->resetPreviousValues();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			InputManager::getInstance()->setMouseLock(!InputManager::getInstance()->getMouseLock());
			continue;
		}

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			exitGame();
			continue;
		}

		InputManager::getInstance()->processEvent(event);

		this->window->HandleEvent(event);
	}

	InputManager::getInstance()->update(deltaTime);

	if (this->previousState != nullptr)
	{
		delete this->previousState;
	}

	if (this->currentState != nullptr)
	{
		this->currentState->update(this, deltaTime);
	}
}

void Client::exitGame()
{
	shouldClose = true;
}
 
const bool Client::getShouldClose()
{
	return shouldClose;
}

void Client::setGameState(GameState* state)
{
	if (this->previousState != nullptr)
	{
		delete this->previousState;
	}

	this->previousState = this->currentState;
	this->currentState = state;
}
