#include "main.h"

void StaticMesh::loadMesh(const char* path)
{
	// Debug info
	printf("Loading mesh %s...\n", path);

	// Load mesh object
	filename = "assets/models/" + string(path);
	basepath = filename.substr(0, filename.find_last_of("/") + 1);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Cannot load mesh %s!\n", filename.c_str());
		return;
	}

	// Parse object
	parse(scene);

	printf("Mesh %s loaded.\n\n", filename.c_str());
}

void StaticMesh::parse(const aiScene* scene)
{
	// Allocate materials
	vector<Material*> materials;
	materials.resize(scene->mNumMaterials);

	// Retrieve the materials
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		// New material
		materials[i] = new Material();

		// Mesh material
		aiMaterial* m_Material = scene->mMaterials[i];

		// Variables
		aiColor4D color;
		aiString texturePath;

		if (aiGetMaterialColor(m_Material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
		{
			materials[i]->color = vec3(color.r, color.g, color.b);
		}

		if (aiGetMaterialTexture(m_Material, aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
		{
			Texture* diffuseTex = new Texture();
			string texPath = basepath + string(texturePath.C_Str());

			// Load texture
			diffuseTex->loadTexture(texPath.c_str());

			// Set material diffuse texture
			materials[i]->diffuse = diffuseTex;
		}
	}

	// Loop the meshes
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		// Mesh
		aiMesh* m_Mesh = scene->mMeshes[i];
		Mesh* mesh = new Mesh();

		// Set material
		mesh->material = materials[m_Mesh->mMaterialIndex];

		vector<float> buffers;
		vector<unsigned int> indices;

		for (unsigned int i = 0; i < m_Mesh->mNumVertices; i++)
		{
			// Store vertices
			buffers.push_back(m_Mesh->mVertices[i].x);
			buffers.push_back(m_Mesh->mVertices[i].y);
			buffers.push_back(m_Mesh->mVertices[i].z);

			// Vars
			vec3 normals = vec3(0.0f);
			vec2 uvs = vec2(0.0f);

			// UVs
			if (m_Mesh->HasTextureCoords(0))
				uvs = vec2(m_Mesh->mTextureCoords[0][i].x, m_Mesh->mTextureCoords[0][i].y);

			buffers.push_back(uvs.x);
			buffers.push_back(uvs.y);

			// Normals
			if (m_Mesh->HasNormals())
				normals = vec3(m_Mesh->mNormals[i].x, m_Mesh->mNormals[i].y, m_Mesh->mNormals[i].z);

			buffers.push_back(normals.x);
			buffers.push_back(normals.y);
			buffers.push_back(normals.z);
		}

		for (unsigned int i = 0; i < m_Mesh->mNumFaces; i++)
		{
			// Mesh's face
			aiFace& m_Face = m_Mesh->mFaces[i];

			if (m_Face.mNumIndices != 3)
				continue;

			// Append indices
			for (int i = 0; i < 3; i++)
			{
				indices.push_back(m_Face.mIndices[i]);
			}
		}

		// Load the mesh
		mesh->create(buffers, indices);

		// Append mesh
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
