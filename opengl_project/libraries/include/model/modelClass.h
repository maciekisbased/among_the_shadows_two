#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<string>
#include<vector>

#include "shader/shaderClass.h"
#include "mesh/meshClass.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{


public:

	Model(const std::string& path);

	// model data including vector of all meshes in the model aswell as the directory
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> loadedTextures; // stores the textures that have already been loaded

	void Draw(Shader& shader);

private:

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);

};


#endif