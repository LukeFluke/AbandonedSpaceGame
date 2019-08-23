#pragma once
#include "game_object.h"

class Projectile : public PhysObject
{
public:
	Projectile(b2World *world, float32 x, float32 y, WTexture* &texture, bool fixedRotation);
	Projectile(b2World *world, b2FixtureDef *fixture, float32 x, float32 y, WTexture* &texture, bool fixedRotation);
	Projectile(b2World *world, std::vector<b2FixtureDef> fixtures, float32 x, float32 y, WTexture* &texture, bool fixedRotation);

	int getObjectType() { return OBJ_PROJECTILE; }

	void update();
	
	~Projectile();

	PhysObject* m_owner;
	std::string m_affiliation;
	double m_damage;

	int m_dying_state;
};

class Missile : public Projectile
{
	Missile(b2World *world, float32 x, float32 y, WTexture* &texture, bool fixedRotation);
	Missile(b2World *world, b2FixtureDef *fixture, float32 x, float32 y, WTexture* &texture, bool fixedRotation);
	Missile(b2World *world, std::vector<b2FixtureDef> fixtures, float32 x, float32 y, WTexture* &texture, bool fixedRotation);

};