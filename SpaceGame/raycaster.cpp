#include "raycaster.h"
#include "game_object.h"

#include <iostream>
using std::cout;
using std::endl;

RayCaster::RayCaster()
{
	m_world = NULL;
	m_ignore_collidable = false;
}

RayCaster::RayCaster(const b2World * world)
{
	m_world = world;
	m_ignore_collidable = false;
}

RayCaster::RayCastResult & RayCaster::get_result()
{
	return m_raycast;
}

void RayCaster::set_world(const b2World * world)
{
	m_world = world;
}

//main raycasting function
float RayCaster::do_raycast(const b2Vec2& start_point, float angle, float distance, 
	const PhysObject* starting_object, bool ignore_collidable)
{
	m_ignore_collidable = ignore_collidable;
	if (distance == -1) {
		distance = 20000;
	}
	float32 endX = start_point.x + cos(angle) * distance;
	float32 endY = start_point.y + sin(angle) * distance;
	
	m_raycast.ray_start = start_point;
	m_raycast.ray_end = b2Vec2(endX, endY);
	m_raycast.ray_angle = angle;
	m_raycast.start_object = starting_object;
	m_raycast.closest_object = NULL;
	m_raycast.shortest_dist = -1;
	m_raycast.contact_point = b2Vec2(-1, -1);

	if (m_world == NULL) {
		return -1;
	}

   	m_world->RayCast(this, start_point, m_raycast.ray_end);

	return m_raycast.shortest_dist;
}

float32 RayCaster::ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction)
{
	b2Body* body = fixture->GetBody();

	if (body->GetUserData() == NULL)
	{
		cout << "Body has no user data!" << endl;
		return -1;
	}

	PhysObject* hitobj = static_cast<PhysObject*>(body->GetUserData());
	b2Vec2 end = point;
	float32 dist = (end - b2Vec2(m_raycast.ray_start.x, m_raycast.ray_start.y)).Length();

	if (fixture->IsSensor()) {
		return 1;
	}

	if (m_raycast.shortest_dist != -1 && dist > m_raycast.shortest_dist) {
		return 1;
	}

	m_raycast.shortest_dist = dist;
	m_raycast.closest_object = hitobj;
	m_raycast.contact_point = b2Vec2(end.x, end.y);

	return 1;
}
