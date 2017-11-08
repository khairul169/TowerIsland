#include "main.h"

void Camera::setPerspective(float fov, float ratio, float near, float far)
{
	this->fov = fov;
	this->ratio = ratio;
	this->near = near;
	this->far = far;

	projection = perspective(fov, ratio, near, far);
}

void Camera::setOrtho(float left, float right, float bottom, float top, float near, float far)
{
	projection = ortho(left, right, bottom, top, near, far);
}

void Camera::setRatio(float ratio)
{
	this->ratio = ratio;

	projection = perspective(this->fov, this->ratio, this->near, this->far);
}

void Camera::setLookAt(vec3 origin, vec3 look_pos, vec3 up)
{
	view = lookAt(origin, look_pos, up);
}
