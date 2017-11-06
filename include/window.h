#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>

// GLEW
#include "GL/glew.h"

// GLFW3
#include "GLFW/glfw3.h"

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