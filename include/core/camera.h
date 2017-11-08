#ifndef CAMERA_H
#define CAMERA_H

#include "main.h"

class Camera
{
private:
	mat4 projection, view;

public:
	float fov, ratio, near, far;

public:
	Camera()
	{
		fov = 45.0f;
		ratio = 4.0f / 3.0f;
		near = 0.1f;
		far = 100.0f;
	}

	void setPerspective(float fov, float ratio, float near, float far);
	void setOrtho(float left, float right, float bottom, float top, float near, float far);
	void setLookAt(vec3 origin, vec3 look_pos, vec3 up);
	void setRatio(float ratio);

	mat4 getProjection() { return projection; }
	mat4 getView() { return view; }
};

#endif // !CAMERA_H
