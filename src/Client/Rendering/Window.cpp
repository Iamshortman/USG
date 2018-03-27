#include "Client/Rendering/Window.hpp"

Window::Window(int width, int height, string windowTitle)
{
	//SDL_InitSubSystem(SDL_INIT_VIDEO);

	//Anti Aliasing
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	// Create an application window with the following settings:
    window = SDL_CreateWindow(
        windowTitle.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
    );
	//SDL_MaximizeWindow(window);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if( window == NULL )
    {
        exit(1);
    }

	// Create an OpenGL context associated with the window.
	glcontext = SDL_GL_CreateContext(window);

	if (glcontext == NULL)
	{
		printf("Error: Failed to initialize OpenGL\n");
		exit(1);
	}

	//SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    // Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		printf("Error: Failed to initialize GLEW\n");
		exit(1);
	}

	initGL();
	resizeWindow(width, height);

	setVsync(1);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	if ((major > 4 || (major == 4 && minor >= 5)) ||
		SDL_GL_ExtensionSupported("GL_ARB_clip_control"))
	{
		glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	}
	else
	{
		fprintf(stderr, "glClipControl required, sorry.\n");
		exit(1);
	}

}


Window::~Window()
{
	if (window != 0)
	{
		closeWindow();
	}
}

void Window::initGL()
{
    // init OpenGL here
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
}

void Window::set3dRendering()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void Window::set2dRendering()
{
	//disable the Depth test.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void Window::resetGlViewport()
{
	int width, height;
	this->getWindowSize(width, height);
	this->resizeWindow(width, height);
}

//Needs to be renamed
void Window::resizeWindow(int width, int height)
{
    if (height == 0) // Prevent A Divide By Zero By
    {
        height = 1;// Making Height Equal One
    }

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height); // Reset The Current Viewport

}

void Window::getWindowSize(int &width, int &height)
{
    SDL_GetWindowSize(window, &width, &height);
}

void Window::HandleEvent(SDL_Event& e)
{
     if( e.type == SDL_WINDOWEVENT )
     {
        if(e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
			this->resizeWindow(e.window.data1, e.window.data2);
        }
        else if(e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
        {
            keyboardFocus = true;
        }
        else if(e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
        {
            keyboardFocus = false;
        }

     }
}

void Window::updateBuffer()
{
	SDL_GL_SwapWindow(window);
}

void Window::clearBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setBufferClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	glClearColor(red, green, blue, alpha);
}

void Window::setTitleString(string title)
{
	SDL_SetWindowTitle(window, title.c_str());
}

const string Window::getTitleString()
{
	return SDL_GetWindowTitle(window);
}

bool Window::isWindowActive()
{
    return keyboardFocus;
}

void Window::setMousePos(int x, int y)
{
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_WarpMouseInWindow(window, x, y);
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
}

void Window::getMousePos(int &x, int &y)
{
     SDL_GetMouseState(&x, &y);
}

void Window::getCenteredMousePos(int &x, int &y)
{
	int xPos, yPos;
	int width, height;
	getWindowSize(width, height);

	SDL_GetMouseState(&xPos, &yPos);

	x = xPos - (width / 2);
	y = yPos - (height / 2);
	//x = (double)(xPos) / (double)(width);
	//y = (double)(yPos) / (double)(height);
}

void Window::closeWindow()
{
	SDL_DestroyWindow(window);
	window = 0;
}

/*
0 = Vsync off
1 = Vsync on
2 = Vsync half rate
*/
void Window::setVsync(int syncRate)
{
	if (syncRate >= 0 && syncRate <= 2)
	SDL_GL_SetSwapInterval(syncRate);
}

int Window::getVsync()
{
	return SDL_GL_GetSwapInterval();
}

