#include"shaderClass.h"


std::string get_file_contents(const char* filename)
{
	// code that reads a file and returns its conternts as a string
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{

		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);

	}
	throw std::runtime_error("Could not open file");

}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// from reading file contents gets a std::string
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// turns code into character array to be use in glShaderSource
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// replaces the source code in the shader object as vertexShaderSource
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// shader source needs to be compiled so it can be understood by the gpu
	glCompileShader(vertexShader);

	int success;
	char infolog[512] = {};
	//checks if shader compiled correctly if not throws an error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{

		throw std::runtime_error(
			std::string("ERROR::SHADER::VERTEX::COMPILATION::FAILED\n") + infolog
		);

	}

	// we need to do the same thing for the fragment shader as we did with the vertex shader

	//creates a fragment shader object stored as an unssigned intiger
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// replaces the sourcec code in the shader object as fragmentShaderSource
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// shader source needs to be compiled so it can be understood by the gpu
	glCompileShader(fragmentShader);
	//checks if fragment shader compiled correctly 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		throw std::runtime_error(
			std::string("ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n") + infolog
		);

	}


	// to use the compiled shaders we have to store them in a 'shader program'
	// shader program is ID in the shader class 

	ID = glCreateProgram();

	//attach the fragment and vertex shaders to the program 
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	// links the shaders so they can be run as an executable in the gpu ? i think?
	glLinkProgram(ID);
	//check if linking shaders was successful and prints logs 
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infolog);

		throw std::runtime_error(
			std::string("ERROR::SHADER::PROGRAM::LINK::FAILED\n") + infolog
			);

	}


	// delete the shaders that are already used in the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


}

void Shader::Activate() const
{
	glUseProgram(ID); // selects shader program to be used
}

void Shader::Delete() const
{
	glDeleteProgram(ID); // deletes shader program
}

void Shader::setVec4(const std::string& name,  
	float x, float y, float z, float w) const
{ 
	// changes uniform of type vec4
	glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::setInt(const std::string& name, int value) const
{
	// changes unifrom of type int
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setBool(const std::string& name, bool value) const
{
	// changes uniform of type bool
	glUniform1i(getUniformLocation(name), (int)value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	// changes uniform of type float
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 matrix) const
{
	// changes uniform of type mat4 
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}


GLint Shader::getUniformLocation(const std::string& name) const
{
	// finds the uniform location of uniform with name
	int uniformLocation = glGetUniformLocation(ID, name.c_str());
	// if the uniform location cannot be found then throws error
	if (uniformLocation == -1)
	{
		throw std::runtime_error(std::string("FAILED::TO::LOCATE::UNIFORM"));
	}

	return uniformLocation;

}

