#ifndef MESH_CLASS_H
#define MESH_CLASS_H


#include<string>
#include<vector>

#include"shader/shaderClass.h"
#include"Cameras/cameraClass.h"
#include"textureLoader/textureClass.h"


struct Vertex {

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

};

class Mesh {

public:

	// mesh data
	std::vector <Vertex>  verticies;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	GLuint VAO;

	Mesh(std::vector <Vertex>& verticies, std::vector <GLuint> indicies, std::vector <Texture>);

	void Draw(Shader& shader);


private:

	GLuint VBO, EBO;

	void setupMesh();

};


#endif