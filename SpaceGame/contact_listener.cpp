#include "contact_listener.h"
#include "game_object.h"

void SContactListener::BeginContact(b2Contact * contact)
{
	//check if fixture A was a PhysObject
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	if (bodyUserData)
		static_cast<PhysObject*>(bodyUserData)->startContact();

	//check if fixture B was a PhysObject
	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserData)
		static_cast<PhysObject*>(bodyUserData)->startContact();
}

void SContactListener::EndContact(b2Contact * contact)
{
	//check if fixture A was a PhysObject
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	if (bodyUserData)
		static_cast<PhysObject*>(bodyUserData)->endContact();

	//check if fixture B was a PhysObject
	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserData)
		static_cast<PhysObject*>(bodyUserData)->endContact();
}
