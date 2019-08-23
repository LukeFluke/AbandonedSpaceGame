#pragma once
#include <SDL.h>
#include <string>


class Window
{
public:
	//Initializes internals
	Window();

	//Creates window
	bool init(std::string winName, int winWidth, int winHeight);

	//Creates renderer from internal window
	SDL_Renderer* createRenderer();

	//Handles window events
	void handleEvent(SDL_Event &e, SDL_Renderer* renderer);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();

	//Window focuses
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();

private:
	//Window data
	SDL_Window* mWindow;
	std::string mName;

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
};