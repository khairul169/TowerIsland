#include "main.h"

void StaticMesh::LoadMesh(const char* path)
{
	// Debug info
	printf("Loading mesh %s...\n", path);

	// Load mesh object
	filename = "assets/models/" + string(path);
	basepath = filename.substr(0, filename.find_last_of("/") + 1);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_PreTransformVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Cannot load mesh %s!\n", filename.c_str());
		return;
	}

	// Parse object
	Parse(scene);

	printf("Mesh %s loaded.\n\n", filename.c_str());
}

void StaticMesh::Parse(const aiScene* scene)
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
		aiMaterial* mMaterial = scene->mMaterials[i];

		// Variables
		aiColor4D color;
		aiString texturePath;

		if (aiGetMaterialColor(mMaterial, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
		{
			materials[i]->color = vec3(color.r, color.g, color.b);
		}

		if (aiGetMaterialTexture(mMaterial, aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
		{
			Texture* diffuseTex = new Texture();
			string texPath = basepath + string(texturePath.C_Str());

			// Load texture
			diffuseTex->LoadTexture(texPath.c_str());

			// Set material diffuse texture
			materials[i]->diffuse = diffuseTex;
		}
	}

	// Loop the meshes
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		// Mesh
		aiMesh* mMesh = scene->mMeshes[i];

		// Vertex buffers
		vector<float> buffers;
		vector<unsigned int> indices;

		// Collision buffers
		vector<float> colVertices;
		vector<unsigned int> colIndices;

		bool generateCollision = (string(mMesh->mName.C_Str()).rfind("-col") != -1);

		for (unsigned int i = 0; i < mMesh->mNumVertices; i++)
		{
			// Store vertices
			buffers.push_back(mMesh->mVertices[i].x);
			buffers.push_back(mMesh->mVertices[i].y);
			buffers.push_back(mMesh->mVertices[i].z);

			if (generateCollision)
			{
				colVertices.push_back(mMesh->mVertices[i].x);
				colVertices.push_back(mMesh->mVertices[i].y);
				colVertices.push_back(mMesh->mVertices[i].z);
			}

			// Vars
			vec3 normals = vec3(0.0f);
			vec2 uvs = vec2(0.0f);

			// UVs
			if (mMesh->HasTextureCoords(0))
				uvs = vec2(mMesh->mTextureCoords[0][i].x, mMesh->mTextureCoords[0][i].y);

			buffers.push_back(uvs.x);
			buffers.push_back(uvs.y);

			// Normals
			if (mMesh->HasNormals())
				normals = vec3(mMesh->mNormals[i].x, mMesh->mNormals[i].y, mMesh->mNormals[i].z);

			buffers.push_back(normals.x);
			buffers.push_back(normals.y);
			buffers.push_back(normals.z);
		}

		for (unsigned int i = 0; i < mMesh->mNumFaces; i++)
		{
			// Mesh's face
			aiFace& mFace = mMesh->mFaces[i];

			if (mFace.mNumIndices != 3)
				continue;

			// Append indices
			for (int i = 0; i < 3; i++)
			{
				indices.push_back(mFace.mIndices[i]);

				if (generateCollision)
				{
					colIndices.push_back(mFace.mIndices[i]);
				}
			}
		}

		if (!generateCollision)
		{
			// Create the mesh
			Mesh* mesh = new Mesh();
			mesh->Create(buffers, indices);

			// Set material
			mesh->material = materials[mMesh->mMaterialIndex];

			// Append mesh
			meshes.push_back(mesh);
		}
		else if (colVertices.size() > 0 && colIndices.size() > 0)
		{
			mPhysicsMgr->createObject()->createTriMesh(colVertices, colIndices);
		}
	}
}

void StaticMesh::Draw()
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i]->position = position;
		meshes[i]->quaternion = quaternion;
		meshes[i]->scaling = scaling;
		meshes[i]->Draw();
	}
}
