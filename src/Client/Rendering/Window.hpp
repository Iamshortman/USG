#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Client/Rendering/OpenGL_Include.hpp"
#include "Client/SDL2_Include.hpp"

#include <string>

using namespace std;

class Window
{
	public:
		Window(int width = 1600, int height = 900, string windowTitle = "USG");
		virtual ~Window();
		const string getTitleString();
		void setTitleString(string title);
		void closeWindow();
		void HandleEvent(SDL_Event& e);
		void updateBuffer();
		void clearBuffer();
		void setBufferClearColor(GLclampf red, GLclampf green, GLclampf blue,
GLclampf alpha);
        void resizeWindow(int width, int height);
        void getWindowSize(int &width, int &height);
        SDL_Window *window;
        bool isWindowActive();
        void setMousePos(int x, int y);
        void getMousePos(int &x, int &y);
		void getCenteredMousePos(double &x, double &y);
		void setVsync(int syncRate);
		int getVsync();
		void set3dRendering();
		void set2dRendering();

	private:
        bool keyboardFocus;
		SDL_GLContext glcontext;
		void initGL();

};

#endif
