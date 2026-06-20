#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <iostream>
#include "stb/stb_image.h"
#include<glad/glad.h>



class Texture
{

public:

	GLenum colorFormat;
	GLuint texture;
	int width, height, nrChannels;

	Texture(std::string fileName);


	void Delete() const;
	void Bind2D() const;


};


#endif