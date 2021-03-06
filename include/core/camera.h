#ifndef CAMERA_H
#define CAMERA_H

#include "main.h"

class Camera
{
public:
	// Variables
	mat4 projection, view;
	float fov, ratio, near, far;
	
	// Camera transform
	vec3 mOrigin, mLookAt;

public:
	Camera()
	{
		fov = 45.0f;
		ratio = 4.0f / 3.0f;
		near = 0.1f;
		far = 100.0f;
	}

	void SetPerspective(float fov, float ratio, float near, float far);
	void SetOrtho(float left, float right, float bottom, float top, float near, float far);
	void SetLookAt(vec3 origin, vec3 look_pos, vec3 up);
	void SetRatio(float ratio);
};

#endif // !CAMERA_H
