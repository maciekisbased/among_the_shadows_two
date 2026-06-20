#include "textureLoader/textureClass.h"


Texture::Texture(std::string fileName) 
{
	glGenTextures(1, &texture);
	
	// load and generate the texture
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);

	switch (nrChannels)
	{ // switch statement to verify the number of color channels the image uses so the correct channel
		// number is used in texture generation
	case 1:
		colorFormat = GL_RED;
		break;

	case 3: 
		colorFormat = GL_RGB;
		break;

	case 4:
		colorFormat = GL_RGBA;
		break;

	default:
		{
			std::cout << nrChannels << std::endl;
			throw std::runtime_error("UNSUPPORTED::NUMBER::OF::COLOR::CHANNELS" + nrChannels);
		}

	}

	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		throw std::runtime_error("FAILED::TO::LOAD::IMAGE");

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	// clears the image data afer the texture has been loaded
	stbi_image_free(data);

}

void Texture::Bind2D() const
{ // binds texture
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::Delete() const
{
	glDeleteTextures(1, &texture);
}
