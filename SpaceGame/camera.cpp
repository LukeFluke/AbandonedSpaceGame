#include "camera.h"
#include "defs_n_types.h"

Camera::Camera(int screenWidth, int screenHeight)
{
	halfWidth = (float32)screenWidth / (2.0 * PIXELS_PER_METER);
	halfHeight = (float32)screenHeight / (2.0 * PIXELS_PER_METER);
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

void Camera::setPosition(float32 x, float32 y, float32 ang)
{
	center.x = x;
	center.y = y;
	angle = ang;
}

void Camera::update()
{
}
