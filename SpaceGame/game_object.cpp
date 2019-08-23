#include <stdio.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "game_object.h"

#include "defs_n_types.h"
#include "resource_managment.h"


/*void drawPolygon(b2PolygonShape *polygon, b2Body *body, Camera camera) {
	std::vector<b2Vec2> worldPoints;
	worldPoints.resize(polygon->m_count);

	for (int i = 0; i<polygon->m_count; i++) {
		worldPoints[i] = body->GetWorldPoint(polygon->m_vertices[i]);
	}
	for (int i = 0; i<polygon->m_count - 1; i++) {
		drawScaledLine(worldPoints[i], worldPoints[i + 1], camera);
	}
	drawScaledLine(worldPoints[polygon->m_count - 1], worldPoints[0], camera);
}*/

///////////////
//Game Object//
///////////////

GameObject::GameObject(float32 x, float32 y, WTexture* & texture)
{
	m_X = x / PIXELS_PER_METER;
	m_Y = y / PIXELS_PER_METER;
	m_main_sprite = texture;
	m_half_width = texture->getWidth() / 2.0f;
	m_half_height = texture->getHeight() / 2.0f;
}

void GameObject::render(SDL_Renderer * renderer, Camera &camera)
{
	glm::vec4 vector(m_X, m_Y, 1.0, 1.0);
	
	glm::mat4 transformation;
	transformation = glm::translate(transformation, glm::vec3(-m_half_width, -m_half_height, 1.0));
	transformation = glm::scale(transformation, glm::vec3(PIXELS_PER_METER, PIXELS_PER_METER, 1.0));
	transformation = glm::translate(transformation, glm::vec3(camera.halfWidth, camera.halfHeight, 1.0));
	transformation = glm::rotate(transformation, glm::radians(-camera.angle), glm::vec3(0.0, 0.0, 1.0));
	transformation = glm::translate(transformation, glm::vec3(-camera.center.x, -camera.center.y, 1.0));

	vector = transformation * vector;

//	if ( renderX < (0 - m_main_sprite->getWidth() * 2) || renderX > (camera.m_screenWidth + m_main_sprite->getWidth()) ||
	//	 renderY < (0 - m_main_sprite->getHeight() * 2) || renderY > (camera.m_screenHeight + m_main_sprite->getHeight()) )
		//return;

	m_main_sprite->render(renderer, static_cast<int>(vector.x), static_cast<int>(vector.y), NULL, m_angle - camera.angle);
}

void GameObject::SetPosition(float32 x, float32 y)
{
	m_X = x;
	m_Y = y;
}

void GameObject::changeSprite(WTexture * texture)
{
	m_main_sprite = texture;
	m_half_height = m_main_sprite->getHeight() / 2.0f;
	m_half_width = m_main_sprite->getWidth() / 2.0f;
}

//////////////
//Background//
//////////////

Background::Background(float32 x, float32 y, WTexture *& texture) : GameObject(x, y, texture)
{}

void Background::renderTiled()
{
}

//////////////////
//Physics Object//
//////////////////

PhysObject::PhysObject(b2World * world, float32 x, float32 y, WTexture *& texture, bool fixedRotation, b2BodyType b2type)
	: GameObject(x, y, texture)
{
	m_world = world;

	b2BodyDef objectDef;
	objectDef.type = b2type;
	objectDef.position.Set(m_X, m_Y);
	m_body = m_world->CreateBody(&objectDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_half_width / PIXELS_PER_METER, m_half_height / PIXELS_PER_METER);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	m_body->CreateFixture(&fixtureDef);
	m_body->SetFixedRotation(fixedRotation);
	m_body->SetUserData(this);
}

PhysObject::PhysObject(b2World *world, b2FixtureDef *fixture, float32 x, float32 y, WTexture *& texture, bool fixedRotation, b2BodyType b2type)
	: GameObject(x, y, texture)
{
	m_world = world;

	b2BodyDef objectDef;
	objectDef.type = b2type;
	objectDef.position.Set(m_X, m_Y);
	m_body = m_world->CreateBody(&objectDef);

	if (fixture->density == 0) {
		fixture->density = 1.0f;
	}

	m_body->CreateFixture(fixture);
	m_body->SetFixedRotation(fixedRotation);
	m_numContacts = 0;

	delete fixture->shape;
	delete fixture;
	m_body->SetUserData(this);
}

PhysObject::PhysObject(b2World * world, std::vector<b2FixtureDef> fixtures, float32 x, float32 y, WTexture *& texture, bool fixedRotation, b2BodyType b2type)
	: GameObject(x, y, texture)
{
	m_world = world;

	b2BodyDef objectDef;
	objectDef.type = b2type;
	objectDef.position.Set(m_X, m_Y);
	m_body = m_world->CreateBody(&objectDef);

	for (int i = 0; i < fixtures.size(); i++)
	{
		m_body->CreateFixture(&fixtures[i]);
	}

	m_body->SetFixedRotation(fixedRotation);
	m_numContacts = 0;
	m_body->SetUserData(this);
}

void PhysObject::update()
{
	b2Vec2 pos = m_body->GetPosition();
	float32 ang = m_body->GetAngle();
	SetPosition(pos.x, pos.y);
	m_angle = ang * RADTODEG;
}

void PhysObject::startContact(){
	m_numContacts++;
}

void PhysObject::endContact(){
	m_numContacts--;
}

