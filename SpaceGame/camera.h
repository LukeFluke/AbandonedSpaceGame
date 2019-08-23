#pragma once
#include "Box2D/Box2D.h"

class Camera
{
public:
	Camera(int screenWidth, int screenHeight);

	void setPosition(float32 x, float32 y, float32 ang);

	void update();

	//center of the view
	b2Vec2 center;

	//what the camera is focusing on
	b2Vec2 anchor;

	//screen res
	int m_screenWidth;
	int m_screenHeight;

	//camera view box half extents
	float32 halfWidth;
	float32 halfHeight;

	//rotation of view
	float32 angle;

	//Zoom
	float32 zoom;
};