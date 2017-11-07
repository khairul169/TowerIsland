#include "main.h"

// Obj loader
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

void Mesh::draw()
{
	// Bind VAO
	glBindVertexArray(vao);

	// Shaders binding
	gMain->materialShaders->bind();
	gMain->materialShaders->updateCamera(gMain->getCamera());

	// Set model transform
	mat4 transform = mat4(1.0f);
	transform = translate(transform, position);
	transform = transform * toMat4(quaternion);
	transform = scale(transform, scaling);

	gMain->materialShaders->setModelMatrix(transform);
	gMain->materialShaders->updateProjection();
	gMain->materialShaders->setColor(material->color);
	gMain->materialShaders->setTexture(material->diffuse);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vtb);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvb);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 3rd attribute buffer : Normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, nmb);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, numTris * 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void StaticMesh::loadMesh(const char* path)
{
	// Load mesh object
	filename = "assets/models/" + string(path);
	basepath = filename.substr(0, filename.find_last_of("/") + 1);

	string err;
	if (!LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), basepath.c_str(), true))
	{
		printf("Cannot load obj %s! err: %s\n", filename.c_str(), err.c_str());
		return;
	}

	// Parse object
	parse();
}

void StaticMesh::parse()
{
	vector<Material*> m_materials;
	m_materials.resize(materials.size() + 1);

	// Setup materials
	for (size_t i = 0; i < materials.size(); i++)
	{
		m_materials[i] = new Material();
		m_materials[i]->color = vec3(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);

		if (!materials[i].diffuse_texname.empty())
		{
			Texture *diffuse = new Texture();
			string diffusePath = basepath + materials[i].diffuse_texname;
			diffuse->loadTexture(diffusePath.c_str());

			m_materials[i]->diffuse = diffuse;
		}
	}

	// Add default materials
	Material *defaultMat = new Material();
	defaultMat->color = vec3(0.8f);
	m_materials[materials.size()] = defaultMat;

	// Loop the shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		Mesh *mesh = new Mesh();
		GLuint vertexID, uvID, normalsID, vertexArrayID;

		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		// buffers
		vector<vec3> vertices;
		vector<vec2> uvs;
		vector<vec3> normals;

		int material_id = -1;

		for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
			index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
			index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
			index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

			if (material_id == -1)
				material_id = shapes[s].mesh.material_ids[f];

			// Vertices
			float v[3][3];
			for (int k = 0; k < 3; k++) {
				int f0 = idx0.vertex_index;
				int f1 = idx1.vertex_index;
				int f2 = idx2.vertex_index;

				v[0][k] = attrib.vertices[3 * f0 + k];
				v[1][k] = attrib.vertices[3 * f1 + k];
				v[2][k] = attrib.vertices[3 * f2 + k];
			}

			// UVs
			float tc[3][2];
			if (attrib.texcoords.size() > 0) {
				// Flip Y coord.
				tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
				tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
				tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
				tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
				tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
				tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
			}
			else {
				tc[0][0] = 0.0f;
				tc[0][1] = 0.0f;
				tc[1][0] = 0.0f;
				tc[1][1] = 0.0f;
				tc[2][0] = 0.0f;
				tc[2][1] = 0.0f;
			}

			// Normals
			float n[3][3];
			if (attrib.normals.size() > 0) {
				int f0 = idx0.normal_index;
				int f1 = idx1.normal_index;
				int f2 = idx2.normal_index;
				for (int k = 0; k < 3; k++) {
					n[0][k] = attrib.normals[3 * f0 + k];
					n[1][k] = attrib.normals[3 * f1 + k];
					n[2][k] = attrib.normals[3 * f2 + k];
				}
			}

			for (int k = 0; k < 3; k++) {
				vertices.push_back(vec3(v[k][0], v[k][1], v[k][2]));
				uvs.push_back(vec2(tc[k][0], tc[k][1]));
				normals.push_back(vec3(n[k][0], n[k][1], n[k][2]));
			}
		}

		if (vertices.size() > 0)
		{
			glGenBuffers(1, &vertexID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexID);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

			mesh->numTris = vertices.size() / 3;
		}

		if (uvs.size() > 0)
		{
			glGenBuffers(1, &uvID);
			glBindBuffer(GL_ARRAY_BUFFER, uvID);
			glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
		}

		if (normals.size() > 0)
		{
			glGenBuffers(1, &normalsID);
			glBindBuffer(GL_ARRAY_BUFFER, normalsID);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
		}

		if (material_id >= 0)
		{
			mesh->material = m_materials[material_id];
		}
		else
		{
			mesh->material = m_materials[m_materials.size() - 1];
		}

		mesh->vtb = vertexID;
		mesh->uvb = uvID;
		mesh->nmb = normalsID;
		mesh->vao = vertexArrayID;
		meshes.push_back(mesh);
	}
}

void StaticMesh::draw()
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i]->position = position;
		meshes[i]->quaternion = quaternion;
		meshes[i]->scaling = scaling;
		meshes[i]->draw();
	}
}
