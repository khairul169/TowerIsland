#include "main.h"

Main *gMain;

// main procedure entry
int main(int argc, char **argv)
{
	gMain = new Main();
	gMain->init();
	return 0;
}

void Main::init()
{
	isActive = true;
	flTime = 0.0f;

	// Init window
	window = new Window();
	window->init();

	// Setup camera
	camera = new Camera();
	camera->setPerspective(radians(45.0f), 1024.0f / 600.0f, 0.1f, 100.0f);
	camera->setLookAt(vec3(4, 3, 3), vec3(0, 0, 0), vec3(0, 1, 0));

	// Our game is ready
	ready();

	// Mainloop
	while (!window->canDestroyed())
	{
		window->loop();
		loop();
		window->swapBuffers();
	}
}

void Main::ready()
{
	// Load shaders
	materialShaders = new MaterialShaders();
	materialShaders->load("material");

	// Init vao
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	// This will identify our vertex buffer
	//GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

static float rotation = 0.0f;

void Main::loop()
{
	// Get time
	flTime = (float)glfwGetTime();
	flDelta = flTime-flLastTime;

	// Reset time
	if (flTime > 3600.0f)
	{
		flTime = flTime - 3600.0f;
	}

	// Update camera
	rotation = fmod(rotation + 30.0f * flDelta, 360.0f);
	vec3 origin = vec3(5 * sin(radians(rotation)), 2, 5 * cos(radians(rotation)));
	camera->setLookAt(origin, vec3(0, 0, 0), vec3(0, 1, 0));

	// Shaders binding
	materialShaders->bind();
	materialShaders->updateCamera(camera);
	
	// Set model transform
	mat4 transform = mat4(1.0f);
	transform = translate(transform, vec3(0.0f, 0.0f, 0.0f));
	transform = scale(transform, vec3(1.0f));

	materialShaders->setModelMatrix(transform);
	materialShaders->updateProjection();
	materialShaders->setColor(vec3(1.0f, 1.0f, 0.2f));

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12*3);
	glDisableVertexAttribArray(0);

	flLastTime = flTime;
}
