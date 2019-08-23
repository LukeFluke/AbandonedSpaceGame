#pragma once
#include "Box2D/Box2D.h"
#include <unordered_map>
#include <vector>

#include "texture.h"
#include "camera.h"

class GameObject
{
public:
	GameObject(float32 x, float32 y, WTexture* &texture);

	virtual void render(SDL_Renderer* renderer, Camera &camera);

	void SetPosition(float32 x, float32 y);

	void changeSprite(WTexture* texture);

	//Center of object
	float32 m_X;
	float32 m_Y;

	//Angle of rotation
	float32 m_angle;

	//box dimensions
	float32 m_half_width;
	float32 m_half_height;

	WTexture* m_main_sprite;

};

class Background : public GameObject
{
public:
	Background(float32 x, float32 y, WTexture* &texture);

	void renderTiled();
};



class PhysObject : public GameObject
{
public:
	enum ObjectType
	{
		OBJ_SHIP,
		OBJ_PROJECTILE
	};

	PhysObject(b2World *world, float32 x, float32 y, WTexture* &texture, bool fixedRotation, b2BodyType b2type);
	PhysObject(b2World *world, b2FixtureDef *fixture, float32 x, float32 y, WTexture* &texture, bool fixedRotation, b2BodyType b2type);
	PhysObject(b2World *world, std::vector<b2FixtureDef> fixtures, float32 x, float32 y, WTexture* &texture, bool fixedRotation, b2BodyType b2type);

	virtual int getObjectType() {
		return 0;
	}

	virtual void update();

	void startContact();

	void endContact();

	int m_numContacts;
	bool m_destroyed = false;
	b2World* m_world;
	b2Body* m_body;
};