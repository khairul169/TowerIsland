#include "main.h"

void Mesh::create(vector<float> buffers, vector<unsigned int> indices)
{
	// Generate VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate buffers
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffers.size(), &buffers[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// Store indices size
	numIndices = indices.size();
}

void Mesh::draw()
{
	// Bind VAO
	glBindVertexArray(vao);

	// Shaders binding
	mainObject->materialShaders->bind();
	mainObject->materialShaders->updateCamera(mainObject->getCamera());

	// Set model transform
	mat4 transform = mat4(1.0f);
	transform = translate(transform, position);
	transform = transform * toMat4(quaternion);
	transform = scale(transform, scaling);

	mainObject->materialShaders->setModelMatrix(transform);
	mainObject->materialShaders->updateProjection();
	
	mainObject->materialShaders->setColor(material->color);
	mainObject->materialShaders->setTexture(material->diffuse);

	// Bind vertex buffer
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); // 3 + 2 + 3 = 8
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 3)); // 3
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 5)); // 3 + 2

	// Indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

	// Draw the triangle !
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

// QuadMesh

QuadMesh::QuadMesh()
{
	// Generate mesh
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const GLfloat buffer[] = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f,  1.0f, 0.0f, -1.0f,  1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 0.0f};

	glGenBuffers(1, &vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void QuadMesh::draw()
{
	// Draw quad
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
