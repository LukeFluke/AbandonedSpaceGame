#include "window.h"
#include <sstream>

Window::Window()
{
	//Initializes non-existant window
	mWindow = NULL;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
	mWidth = 0;
	mHeight = 0;
	mName = "";
}

bool Window::init(std::string winName, int winWidth, int winHeight)
{
	mName = winName;
	//Create window
	mWindow = SDL_CreateWindow(winName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, SDL_WINDOW_SHOWN);
	if (mWindow != NULL)
	{
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = winWidth;
		mHeight = winHeight;
	}

	return mWindow != NULL;
}

SDL_Renderer* Window::createRenderer()
{
	return SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Window::handleEvent(SDL_Event & e, SDL_Renderer* renderer)
{
	//Window event occured
	if (e.type == SDL_WINDOWEVENT)
	{
		//Caption update flag
		bool updateCaption = false;
		switch (e.window.event)
		{
			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(renderer);
			break;

			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			updateCaption = true;
			break;

			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			updateCaption = true;
			break;

			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;
			break;

			//Window maximized
		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
			break;
		}
		if (updateCaption)
		{
			std::stringstream caption;
			caption << mName << " - MouseFocus: " << ((mMouseFocus) ? "On" : "Off") << "KeyboardFocus: " << ((mKeyboardFocus) ? "On" : "Off");
			SDL_SetWindowTitle( mWindow, caption.str().c_str() );
		}
	}
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
	{
		if (mFullScreen)
		{
			SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
			mFullScreen = false;
		}
		else
		{
			SDL_SetWindowFullscreen(mWindow, SDL_TRUE);

			mFullScreen = true;
			mMinimized = false;
		}
	}
}

void Window::free()
{
	if (mWindow != NULL)
	{
		SDL_DestroyWindow(mWindow);
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

int Window::getWidth()
{
	return mWidth;
}

int Window::getHeight()
{
	return mHeight;
}

bool Window::hasMouseFocus()
{
	return mMouseFocus;
}

bool Window::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool Window::isMinimized()
{
	return mMinimized;
}
