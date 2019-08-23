#include "resource_managment.h"

SDL_Renderer* gRenderer;
Window gWindow;

TTF_Font* gFont = NULL;
WTexture gDebugText[15];
SDL_Color textColor;

std::unordered_map<std::string, WTexture*> gTextures;

bool loadIntoHash(std::string filepath, std::string name)
{
	bool success = true;
	WTexture* newTexture = new WTexture;

	if (!newTexture->loadFromFile(filepath, gRenderer))
	{
		std::cout << "Error, could not load " << name << " texture!" << std::endl;
		success = false;
	}
	else
	{
		gTextures[name] = newTexture;
	}

	return success;
}

void removeFromHash(std::string name)
{
	gTextures[name]->free();
	gTextures.erase(name);
}
