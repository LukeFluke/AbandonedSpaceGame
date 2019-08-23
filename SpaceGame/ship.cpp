#include "ship.h"

#include "defs_n_types.h"
#include "body_types.h"
#include "resource_managment.h"
#include "raycaster.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using std::cout;
using std::endl;

void drawScaledLine(b2Vec2 start, b2Vec2 end, Camera camera) {

	glm::vec4 startVec(start.x, start.y, 1.0, 1.0);
	glm::vec4 endVec(end.x, end.y, 1.0, 1.0);

	glm::mat4 transformation;
	transformation = glm::scale(transformation, glm::vec3(PIXELS_PER_METER, PIXELS_PER_METER, 1.0));
	transformation = glm::translate(transformation, glm::vec3(camera.halfWidth, camera.halfHeight, 1.0));
	transformation = glm::rotate(transformation, glm::radians(-camera.angle), glm::vec3(0.0, 0.0, 1.0));
	transformation = glm::translate(transformation, glm::vec3(-camera.center.x, -camera.center.y, 1.0));

	startVec = transformation * startVec;
	endVec = transformation * endVec;

	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(gRenderer, startVec.x, startVec.y, endVec.x, endVec.y);
}

Ship::Ship(b2World * world, float32 x, float32 y, WTexture *& texture, bool fixedRotation, b2BodyType b2type)
	:PhysObject(world, x, y, texture, fixedRotation, b2type)
{
	m_angularAcceleration = 0;
	m_charge = 120;
	m_shoot = 0;
	m_projectileOrigin.x = 0;
	m_projectileOrigin.y = -1;
}

Ship::Ship(b2World * world, b2FixtureDef *fixture, float32 x, float32 y, WTexture *& texture, bool fixedRotation, b2BodyType b2type)
	:PhysObject(world, fixture, x, y, texture, fixedRotation, b2type)
{
	m_angularAcceleration = 0;
	m_charge = 120;
	m_shoot = 0;
	m_projectileOrigin.x = 0;
	m_projectileOrigin.y = -1;
	//set projectile origin later
}

Ship::Ship(b2World * world, std::vector<b2FixtureDef> fixtures, float32 x, float32 y, WTexture *& texture, bool fixedRotation, b2BodyType b2type)
	:PhysObject(world, fixtures, x, y, texture, fixedRotation, b2type)
{
	m_angularAcceleration = 0;
	m_charge = 120;
	m_shoot = 0;
	m_projectileOrigin.x = 0;
	m_projectileOrigin.y = -1;
	//set projectile origin later
}

void Ship::input(std::unordered_map<std::string, bool>& keyPresses)
{
	b2Vec2 desiredForce = b2Vec2(0, 0);

	//
	//Thrusting jets
	//
	//forwards and backwards
	if (keyPresses["w"])
	{
		desiredForce += b2Vec2(0, -0.1f);
	}
	if (keyPresses["s"])
	{
		desiredForce += b2Vec2(0, 0.05f);

	}
	//left and right
	if (keyPresses["a"])
	{
		desiredForce += b2Vec2(-0.05f, 0);

	}
	if (keyPresses["d"])
	{
		desiredForce += b2Vec2(0.05f, 0);

	}

	//
	//Rotation Jets
	//
	if (keyPresses["q"])
	{
		m_angularAcceleration -= 0.001;
		if (m_angularAcceleration < -0.05)
			m_angularAcceleration = -0.05;
	}
	else if (keyPresses["e"])
	{
		m_angularAcceleration += 0.001;
		if (m_angularAcceleration > 0.05)
			m_angularAcceleration = 0.05;
	}
	else
	{
		if (m_angularAcceleration > 0)
			m_angularAcceleration -= 0.001;
		else if (m_angularAcceleration < 0)
			m_angularAcceleration += 0.001;
	}
	//update object body's rotation
	m_body->SetTransform(m_body->GetPosition(), m_body->GetAngle() + m_angularAcceleration);

	//cout << desiredForce.x << ", " << desiredForce.y << endl;

	//Rotate jet force
	b2Vec2 desiredForceRotated;
	desiredForceRotated.x = desiredForce.x * cos(m_body->GetAngle()) - desiredForce.y * sin(m_body->GetAngle());
	desiredForceRotated.y = desiredForce.x * sin(m_body->GetAngle()) + desiredForce.y * cos(m_body->GetAngle());

	//
	//Diminish jet capability as the maximum speed is reached
	//
	b2Vec2 finalForce = desiredForceRotated;
	b2Vec2 vel = m_body->GetLinearVelocity();
	if (vel.Length() > 0)
	{
		//The amount of force going in the direction of the current velocity
		b2Vec2 desiredForceProjection = (b2Dot(finalForce, vel) / b2Dot(vel, vel)) * vel;

		//The amount of force that isn't going in the direction of the current velocity
		b2Vec2 desiredForceRejection = finalForce - desiredForceProjection;

		//Putting a limit on the amount of force that the player can put in the current direction of velocity
		float32 maxSpeed = 25.0f;
		float32 speedLimiter = (maxSpeed - m_body->GetLinearVelocity().Length()) / maxSpeed;
		if (speedLimiter < 0)
			speedLimiter = 0;

		//this is to check if desiredForceProjection is not pointing in the opposite direction of the velocity
		//which is something we couldn't distinguish up until this point
		float32 cosine = b2Dot(vel, desiredForceProjection) / (vel.Length() * desiredForceProjection.Length());
		if (cosine >= 0)
		{
			desiredForceProjection = speedLimiter * desiredForceProjection;
		}

		//recombine the forces
		finalForce = desiredForceProjection + desiredForceRejection;
	}

	//update the object body's velocity
	b2Vec2 impulse = m_body->GetMass() * finalForce;
	m_body->ApplyLinearImpulse(impulse, m_body->GetWorldCenter(), true);

	//
	//Shooting
	//
	if (keyPresses["space"] && m_charge >= 120)
	{
		m_charge -= 120;
		m_shoot = 10;
	}

	//
	//Switch Weapons
	//
	if (keyPresses["tab"] && m_shoot == 0 && m_switchFreeze == 0)
	{
		//change current weapon to the next weapon in the enum
		m_currentWeapon = static_cast<WEAPON_UP>(static_cast<int>(m_currentWeapon) + 1);
		if (static_cast<int>(m_currentWeapon) == WEAPON_UP::MAX) {
			m_currentWeapon = static_cast<WEAPON_UP>(0);	//loops back
		}
		m_switchFreeze = 15;
	}
}

void Ship::update()
{
	PhysObject::update();
	if (m_charge < 120)
		m_charge++;

	if (m_switchFreeze > 0)
		m_switchFreeze--;

	if (m_shoot == 0)
		m_laser.active = false;

	if (m_currentWeapon == Ship::W_LASERS && m_shoot > 0)
	{
		shootLaser();
	}
}

void Ship::render(SDL_Renderer * renderer, Camera & camera)
{
	GameObject::render(renderer, camera);
	if (m_currentWeapon == Ship::W_LASERS && m_shoot > 0 && m_shoot % 3 == 0)
	{
		drawScaledLine(m_laser.start, m_laser.end, camera);
	}
}

Projectile* Ship::createProjectile()
{
	m_shoot -= 2;
	float32 projX = cos((m_angle - 90) * DEGTORAD);
	float32 projY = sin((m_angle - 90) * DEGTORAD);
	projX += m_X;
	projY += m_Y;

	projX *= PIXELS_PER_METER;
	projY *= PIXELS_PER_METER;


	b2FixtureDef* projectileFixture = giveCircle(2.5);
	projectileFixture->density = 0.5f;

	Projectile* createdProjectile = new Projectile(m_world, projectileFixture, projX, projY, gTextures["laser"], true);
	createdProjectile->m_owner = this;

	float32 speed = 40;
	b2Vec2 velocity( speed * cos((m_angle - 90) * DEGTORAD), speed * sin((m_angle - 90) * DEGTORAD));

	createdProjectile->m_body->SetLinearVelocity(velocity);

	return createdProjectile;
}

void Ship::shootLaser()
{
	m_shoot -= 1;
	RayCaster cast(m_world);

	float32 radAngle = (m_angle - 90) * DEGTORAD;

	b2Vec2 origin(cos(radAngle) + m_X, sin(radAngle) + m_Y);

	cast.do_raycast(origin, radAngle, 10, this);
	RayCaster::RayCastResult& result = cast.get_result();
	b2Vec2 contact = result.contact_point;

	if (result.closest_object != NULL)
	{
		b2Body* hitBody = result.closest_object->m_body;

		b2Vec2 impulse(cos(radAngle), sin(radAngle));
		hitBody->ApplyLinearImpulse(impulse, contact, true);
	}

	m_laser.start = b2Vec2(result.ray_start.x, result.ray_start.y);
	m_laser.end = b2Vec2(result.ray_end.x, result.ray_end.y);
	m_laser.active = true;
}
