#pragma once
#include "game_object.h"
#include "projectile.h"

class Ship : public PhysObject
{
public:
	Ship(b2World *world, float32 x, float32 y, WTexture* &texture, bool fixedRotation, b2BodyType b2type);
	Ship(b2World *world, b2FixtureDef *fixture, float32 x, float32 y, WTexture* &texture, bool fixedRotation, b2BodyType b2type);
	Ship(b2World *world, std::vector<b2FixtureDef> fixtures, float32 x, float32 y, WTexture* &texture, bool fixedRotation, b2BodyType b2type);

	void input(std::unordered_map<std::string, bool> &input);

	void update();

	void render(SDL_Renderer* renderer, Camera &camera);

	Projectile* createProjectile();
	void shootLaser();

	int getObjectType() { return OBJ_SHIP; }

	int m_charge;
	int m_shoot;
	float32 m_angularAcceleration;
	b2Vec2 m_projectileOrigin;

	enum WEAPON_UP {
		W_PELLETS,
		W_LASERS,
		MAX
	};
	WEAPON_UP m_currentWeapon;
	int m_switchFreeze = 0;

	struct Laser
	{
		b2Vec2 start;
		b2Vec2 end;
		bool active = false;
	};
	Laser m_laser;
};