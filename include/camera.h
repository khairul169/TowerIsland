#ifndef CAMERA_H
#define CAMERA_H

#include "main.h"

class Camera
{
private:
	mat4 projection, view;

public:
	void setPerspective(float fov, float ratio, float near, float far);
	void setLookAt(vec3 origin, vec3 look_pos, vec3 up);

	mat4 getProjection() { return projection; }
	mat4 getView() { return view; }
};

#endif // !CAMERA_H
