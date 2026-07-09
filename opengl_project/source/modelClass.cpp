#include "model/modelClass.h"


Model::Model(const std::string& path)
{
	loadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader); // draws all the meshes in meshes array
}

void Model::loadModel(std::string path)
{	// importer object initialized
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); 
	//aiProcess_FlipsUVs changes the y axis orientation so model loads properly and is not upsidedown

	// checks if the scene object is emty, there is no root node and if the scene flags are incomplete
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << importer.GetErrorString() << std::endl;
		throw std::runtime_error(std::string("ERROR::ASSIMP::") + importer.GetErrorString());
	}
	// path.substr finds point in string 
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene); // process scene rootnode
}


void Model::processNode(aiNode* node, const aiScene* scene)
{
	//nodes contains mesh indicies and references to child nodes, so we process them individually

	// indexes the mMeshs array and retrieves mesh
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene)); // appends mesh to end of meshes array
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}


Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{ // processes mesh data from node mesh indicies and creates mesh object to be used in meshes array


	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// retrieves xyz position of vertices ans sets them as vertex position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// retrieves and sets the normals for vertex
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		
		if (mesh->mTextureCoords[0])  //checks if textureCoords exists before assigning them
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}

		else // set as 0.0 0.0 if no texture coords exist for mesh
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex); // verticies added to vertex list 

	}
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) // faces contain indicies 
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex > 0) // checks for materials
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		// takes everything from diffuseMaps vector and puts it into texture
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// does the same thing but for specular maps 
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	
	}

	return Mesh(vertices, indices, textures);

}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
	std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		for (unsigned int j = 0; j < loadedTextures.size(); j++)
		{
			if (std::strcmp(loadedTextures[j].path, str.C_Str()) == 0) // checks if textures loaded same as current texure being processed
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}

		}

		if (!skip)
		{
			Texture texture(str.C_Str(), typeName.c_str(), this->directory, 0); // texture constructor assigns
			// type as const char*, typeName as const char* and path as const char*
			textures.push_back(texture);
			loadedTextures.push_back(texture); // adds to loaded texture
		}
		
	}
	return textures;
}
