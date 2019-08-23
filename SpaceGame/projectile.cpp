#include "projectile.h"
#include "defs_n_types.h"
#include "resource_managment.h"

Projectile::Projectile(b2World *world, float32 x, float32 y, WTexture* &texture, bool fixedRotation)
	:PhysObject(world, x, y, texture, fixedRotation, b2_dynamicBody)
{
	m_body->SetUserData(this);
}

Projectile::Projectile(b2World *world, b2FixtureDef *fixture, float32 x, float32 y, WTexture *& texture, bool fixedRotation)
	: PhysObject(world, fixture, x, y, texture, fixedRotation, b2_dynamicBody)
{
	m_body->SetUserData(this);
}

Projectile::Projectile(b2World *world, std::vector<b2FixtureDef> fixtures, float32 x, float32 y, WTexture *& texture, bool fixedRotation)
	: PhysObject(world, fixtures, x, y, texture, fixedRotation, b2_dynamicBody)
{
	m_body->SetUserData(this);
}

void Projectile::update()
{
	if (m_dying_state > 0) {
		m_dying_state++;
	}

	if (m_body)
	{
		PhysObject::update();

		if (m_numContacts > 0) 
		{
			m_world->DestroyBody(m_body);
			m_body = NULL;
			m_dying_state = 1;
			this->changeSprite(gTextures["laser_dead"]);
		}
	}

	if (m_dying_state > 5)
	{
		m_destroyed = true;
	}

	if (m_X > 1000 || m_X < -1000 || m_Y > 1000 || m_Y < -1000) {
		m_destroyed = true;
	}
}

Projectile::~Projectile()
{
	if (m_body)
	{
		m_world->DestroyBody(m_body);
	}
}
