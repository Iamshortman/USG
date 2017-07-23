#ifndef SDL2_INCLUDE_HPP
#define SDL2_INCLUDE_HPP

//Get rid of the SDL_main that messes up the build
#include "SDL2/SDL_main.h"
#undef main

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include <SDL2\SDL_GameController.h>
#include <SDL2\SDL_joystick.h>

#endif //SDL2_INCLUDE_HPP