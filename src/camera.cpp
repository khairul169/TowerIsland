#include "main.h"

void Camera::setPerspective(float fov, float ratio, float near, float far)
{
	projection = perspective(fov, ratio, near, far);
}

void Camera::setLookAt(vec3 origin, vec3 look_pos, vec3 up)
{
	view = lookAt(origin, look_pos, up);
}
