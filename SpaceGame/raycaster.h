#pragma once
#include "Box2D/Box2D.h"



class PhysObject;

class RayCaster : public b2RayCastCallback
{
public:

	//represents the result of a raycast call
	struct RayCastResult
	{
		const PhysObject*	start_object;	//The object where the ray started started (if any)
		b2Vec2		ray_start;		//The starting point of the ray cast
		float		ray_angle;	//The angle at which the ray was cast (in radians)
		b2Vec2		ray_end;		//The maximum point on the ray  
		PhysObject*	closest_object; //The closest object hit by the ray
		float		shortest_dist;	//The closest point of contact on that object
		b2Vec2		contact_point;	//The point where the ray hit
	};

private:
	RayCastResult m_raycast;
	const b2World* m_world;
	bool m_ignore_collidable;

public:
	RayCaster();
	RayCaster(const b2World* world);

	RayCastResult& get_result();

	void set_world(const b2World* world);

	float do_raycast(const b2Vec2& start_point, float angle, float distance, const PhysObject* starting_object = NULL, bool ignore_collidable = false);

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
};