#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Box2D/Box2D.h"


#include <stdio.h>
#include <string>
#include <cmath>
#include <sstream>
#include <list>
#include <iomanip>

#include "defs_n_types.h"
#include "resource_managment.h"
#include "texture.h"
#include "contact_listener.h"
#include "window.h"
#include "ship.h"
#include "projectile.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

//Initializes SDL, the windows, and other stuff
bool init();

//Loads textures and other media
bool loadMedia();

//Frees memory
bool close();

//void drawPolygon(b2PolygonShape *polygon, b2Body *body, bool filled);

std::unordered_map<std::string, bool> keyPresses;

Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);

int main()
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Create Box2D world
			b2Vec2 gravity(0.0f, 0.0f);
			b2World world(gravity);

			//Set contact listener
			SContactListener contact;
			world.SetContactListener(&contact);

			//background
			Background background(0, 0, gTextures["space"]);

			//create the box object
			Ship player(&world, 400, 200, gTextures["player"], true, b2_dynamicBody);

			//create the floor object
			PhysObject floor(&world, 400, 300, gTextures["floor"], false, b2_dynamicBody);



			//Simulation stuff
			float32 timeStep = 1.0f / 60.0f;
			int32 velIter = 6;
			int32 posIter = 2;
			world.Step(timeStep, velIter, posIter);

			//Reset player
			player.update();

			//Reset floor
			floor.update();

			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//debug text
			std::stringstream debugText;

			//reset camera
			camera.setPosition(player.m_X, player.m_Y, player.m_angle);

			std::list<Projectile*> projectiles;

			//While application is running
			while (!quit)
			{

				//
				//Handle events on queue
				//

				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
					{
						quit = true;
					}

					//Handle window events
					gWindow.handleEvent(e, gRenderer);
					
					if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_w:
							keyPresses["w"] = true; break;
						case SDLK_s:
							keyPresses["s"] = true; break;
						case SDLK_a:
							keyPresses["a"] = true; break;
						case SDLK_d:
							keyPresses["d"] = true; break;
						case SDLK_q:
							keyPresses["q"] = true; break;
						case SDLK_e:
							keyPresses["e"] = true; break;
						case SDLK_UP:
							keyPresses["up"] = true; break;
						case SDLK_DOWN:
							keyPresses["down"] = true; break;
						case SDLK_LEFT:
							keyPresses["left"] = true; break;
						case SDLK_SPACE:
							keyPresses["space"] = true; break;
						case SDLK_TAB:
							keyPresses["tab"] = true; break;
						}
					}

					if (e.type == SDL_KEYUP && e.key.repeat == 0)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_w:
							keyPresses["w"] = false; break;
						case SDLK_s:
							keyPresses["s"] = false; break;
						case SDLK_a:
							keyPresses["a"] = false; break;
						case SDLK_d:
							keyPresses["d"] = false; break;
						case SDLK_q:
							keyPresses["q"] = false; break;
						case SDLK_e:
							keyPresses["e"] = false; break;
						case SDLK_UP:
							keyPresses["up"] = false; break;
						case SDLK_DOWN:
							keyPresses["down"] = false; break;
						case SDLK_LEFT:
							keyPresses["left"] = false; break;
						case SDLK_SPACE:
							keyPresses["space"] = false; break;
						case SDLK_TAB:
							keyPresses["tab"] = false; break;
						}
					}
				}				

				//Only do stuff when not minimized
				if (!gWindow.isMinimized())
				{

					/////////
					//Physics
					/////////

					//reading inputs for player physics
					player.input(keyPresses);

					debugText.str("");
					debugText << "Speed:" << player.m_body->GetLinearVelocity().Length();
					gDebugText[0].loadFromRenderedText(debugText.str(), textColor, gFont, gRenderer);


					//simulate environment for one step
					world.Step(timeStep, velIter, posIter);

					//Update positions of objects
					player.update();
					floor.update();

					if (player.m_shoot > 0)
					{
						if (player.m_currentWeapon == Ship::W_PELLETS)
						{
							projectiles.push_back(player.createProjectile());
						}
					}

					for (auto iter = projectiles.begin(); iter != projectiles.end();)
					{
						(*iter)->update();
						if ((*iter)->m_destroyed == true)
						{
							delete (*iter);
							iter = projectiles.erase(iter);
						}
						else
						{
							iter++;
						}
					}

					camera.setPosition(player.m_X, player.m_Y, player.m_angle);

					debugText.str("");
					debugText << "Charge: " << std::fixed << std::setprecision(0) << 100.0 * (float)player.m_charge / 120 << "%";
					gDebugText[1].loadFromRenderedText(debugText.str(), textColor, gFont, gRenderer);


					debugText.str("");
					debugText << "Weapon: ";
					if (player.m_currentWeapon == Ship::W_LASERS)
					{
						debugText << "LASER";
					}
					else
					{
						debugText << "PELLETS";
					}
					gDebugText[2].loadFromRenderedText(debugText.str(), textColor, gFont, gRenderer);


					//
					//Rendering
					//


					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
					SDL_RenderClear(gRenderer);

					background.render(gRenderer, camera);

					player.render(gRenderer, camera);
					floor.render(gRenderer, camera);

					for (auto iter : projectiles)
					{
						iter->render(gRenderer, camera);
					}

					gDebugText[0].render(gRenderer, 0, 0);
					gDebugText[1].render(gRenderer, 0, 15);
					gDebugText[2].render(gRenderer, 0, 30);

					SDL_Rect screen_clip;
					screen_clip.x = 0;
					screen_clip.y = 0;
					screen_clip.w = camera.m_screenWidth;
					screen_clip.h = camera.m_screenHeight;
					SDL_RenderSetClipRect(gRenderer, &screen_clip);

					//Update screen
					SDL_RenderPresent(gRenderer);
				}
			}
		}

		//Free resources and close SDL
		close();
	}
	return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Decide resolution
		{

		}

		//Create window
		if (!gWindow.init("SpaceGame", SCREEN_WIDTH, SCREEN_HEIGHT))
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = gWindow.createRenderer();

			if (!TTF_Init() == -1)
			{
				printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
				success = false;
			}
		}
	}
	return success;
}

bool loadMedia()
{
	//Load success flag
	bool success = true;

	//Load Textures
	if (!loadIntoHash("img/player.png", "player"))
		success = false;
	if (!loadIntoHash("img/floor.png", "floor"))
		success = false;
	if (!loadIntoHash("img/sp.png", "space"))
		success = false;
	if (!loadIntoHash("img/box.png", "box"))
		success = false;
	if (!loadIntoHash("img/laser.png", "laser"))
		success = false;
	if (!loadIntoHash("img/laser_dead.png", "laser_dead"))
		success = false;

	gFont = TTF_OpenFont("arialbd.ttf", 12);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Set text color as black
		textColor = { 255, 255, 255, 255 };
	}


	return success;
}

bool close()
{
	//Free loaded images

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	//Quit SDl subsystems
	IMG_Quit();
	SDL_Quit();
	return false;
}