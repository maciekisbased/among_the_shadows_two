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
	GLuint unit;
	const char* path;
	const char* type;
	int width, height, nrChannels;

	Texture(const char* path, const char* texType, const std::string& directory, GLuint slot);

	void Delete() const;
	void Bind() const;
	void Unbind() const;


};


#endif