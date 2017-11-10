#ifndef WINDOW_H
#define WINDOW_H

#include "main.h"
#include <stdio.h>

// GLEW
#include "glew/glew.h"

// GLFW3
#include "glfw/glfw3.h"

#include "renderer.h"

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
	void free();

	bool canDestroyed();
	bool getKey(int key);

	// Callbacks
	void onResized(int width, int height);
};

#endif