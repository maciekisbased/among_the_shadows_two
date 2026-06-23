#include"mesh/meshClass.h"

Mesh::Mesh(std::vector <Vertex>& verticies, std::vector <GLuint> indices, std::vector <Texture> textures)
{

	Mesh::verticies = verticies;
	Mesh::indices = indices;
	Mesh::textures = textures;

	setupMesh();

}


void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// setup for rectangle
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// array is the size of number of verticies used (one vertex size * number of verticies)
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	// Attribute pointer for position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// Attribute pointer for normal of faces	 offset of Normal gets position fo Normal vec in Vertex struct
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(1);

	// Attribute pointer for the texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));
	glEnableVertexAttribArray(2);
	
	// sets up EBO array with indices 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	// for shader with no texture if texture vector is empty then no texture is assigned
	for (unsigned int i = 0; i < textures.size(); i++)
	{	// format for texures is textureType then number e.g diffuse0, specular3 ect
		std::string textureType = textures[i].type;
		std::string number;

		if (textureType == "diffuse") // sets number to the current number of diffuse texures and incriments
			number = std::to_string(diffuseNr++);
		else if (textureType == "specular") // sets number to the current number of specular textures and incriments
			number = std::to_string(specularNr++);


		shader.setInt(("material." + textureType + number).c_str(), i);
		textures[i].Bind();
	}
	
	// draw mesh using indices
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
