#include "main.h"
#include "window.h"

void window_size_callback(GLFWwindow* window, int width, int height) { mainObject->window->onResized(width, height); }

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

	glfwWindowHint(GLFW_SAMPLES, 4);
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
	glfwSetWindowSizeCallback(window, &window_size_callback);

	// Init visual render
	renderer = new VisualRender();
	renderer->init();
}

void Window::onResized(int width, int height)
{
	// Get real framebuffer size
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	// Set actual width & height
	this->width = fbWidth;
	this->height = fbHeight;

	// Reset ratio
	mainObject->camera->setRatio(getAspectRatio());

	// Call renderer resized method
	renderer->resized();
}

void Window::loop()
{
	// Loop the renderer
	renderer->loop();
}

void Window::swapBuffers()
{
	// Swap buffers
	glfwSwapBuffers(window);
	glfwSwapInterval(1);
	glfwPollEvents();
}

void Window::free()
{
	renderer->free();
}

bool Window::canDestroyed()
{
	return (glfwWindowShouldClose(window) != 0);
}

bool Window::getKey(int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}
