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
	void init();
	void loop();
	void swapBuffers();

	bool canDestroyed();
	bool getKey(int key);
};

#endif