#include "main.h"
#include "window.h"

void Window::init()
{
	// Setup variables
	width = 1024;
	height = 600;

	// Init glfw
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW\n");
		return;
	}

	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, PROJECT_NAME, NULL, NULL);

	if (window == NULL) {
		printf("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 50, 50);

	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void Window::loop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(187.0f/255.0f, 210.0f / 255.0f, 247.0f / 255.0f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull face
	glEnable(GL_CULL_FACE);
}

void Window::swapBuffers()
{
	// Swap buffers
	glfwSwapBuffers(window);
	glfwSwapInterval(1);
	glfwPollEvents();
}

bool Window::canDestroyed()
{
	return (glfwWindowShouldClose(window) != 0);
}

bool Window::getKey(int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}
