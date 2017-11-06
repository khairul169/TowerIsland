#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>

// GLEW
#include "glew/glew.h"

// GLFW3
#include "glfw/glfw3.h"

class Window
{
private:
	GLFWwindow* window;

public:
	int width, height;

	float getAspectRatio() { return (float)width / (float)height; }

public:
	void init();
	void loop();
	void swapBuffers();

	bool canDestroyed();
	bool getKey(int key);
};

#endif