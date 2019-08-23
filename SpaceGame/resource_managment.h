#pragma once
#include <unordered_map>

#include <iostream>
#include "texture.h"
#include "window.h"

//Renderer
extern SDL_Renderer* gRenderer;
//Window
extern Window gWindow;

//Text rendering stuff
extern TTF_Font* gFont;
extern WTexture gDebugText[15];
extern SDL_Color textColor;

//Hash of all textures
extern std::unordered_map<std::string, WTexture*> gTextures;

//
//Function used to load a texture from a filepath into gTextures
//The name variable is the key to access the texture
//
bool loadIntoHash(std::string filepath, std::string name);

//Used to deload a texture from the hash
void removeFromHash(std::string name);