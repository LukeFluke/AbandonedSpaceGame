#pragma once
#include "defs_n_types.h"
#include "Box2D/Box2D.h"

#include <vector>

//
//Fixture Defs for physics bodies
//

//radius is in pixels
b2FixtureDef* giveCircle(float32 radius)
{
	b2CircleShape *circle = new b2CircleShape;
	circle->m_p.Set(0, 0);
	circle->m_radius = radius / PIXELS_PER_METER;

	b2FixtureDef*  fixtureDef = new b2FixtureDef;
	fixtureDef->shape = circle;

	return fixtureDef;
}

//half length and half width in pixels
b2FixtureDef giveBox(float32 halfWidth, float32 halfHeight)
{
	b2PolygonShape box;
	box.SetAsBox(halfWidth / PIXELS_PER_METER, halfHeight / PIXELS_PER_METER);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;

	return fixtureDef;
}

b2FixtureDef givePolygon(b2Vec2 verticies[], int number)
{
	b2PolygonShape polygon;
	polygon.Set(verticies, number);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygon;

	return fixtureDef;
}