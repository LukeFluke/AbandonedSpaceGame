#pragma once
#include "Box2D/Box2D.h"


class SContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};