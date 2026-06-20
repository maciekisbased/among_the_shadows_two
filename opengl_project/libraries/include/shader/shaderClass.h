#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

std::string get_file_contents(const char* filename);


class Shader
{

	public:
		// different shader program has different ID
		GLuint ID;
		Shader(const char* vertexFile, const char* fragmentFile);


		GLint getUniformLocation(const std::string& name) const;

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec4(const std::string& name,
			float x, float y, float z, float w) const;
		void setVec3(const std::string& name,
			float x, float y, float z) const;
		void setVec3(const std::string& name,
			glm::vec3 value) const;
		void setMat4(const std::string& name, glm::mat4 matrix) const;


		void Activate() const; 
		void Delete() const;


};


#endif