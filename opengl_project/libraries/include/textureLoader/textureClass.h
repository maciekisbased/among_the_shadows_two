#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <iostream>
#include "stb/stb_image.h"
#include<glad/glad.h>



class Texture
{

public:

	GLenum colorFormat;
	GLuint ID;
	//GLuint unit;  now not using uints instead bind must assign a slot
	const char* path;
	std::string type;
	int width, height, nrChannels;

	Texture(const char* path, std::string texType, const std::string& directory, GLuint slot);

	void Delete() const;
	void Bind(GLuint slot) const;
	void Unbind() const;


};


#endif