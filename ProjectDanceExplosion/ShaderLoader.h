#pragma once

#include <GL\glew.h>
#include <GL\freeglut.h>

#include <iostream>
#include <fstream>
#include <vector>


class ShaderLoader
{
private:
	std::string ReadShader(char* filename);
	GLuint CreateShader(GLenum shaderType, std::string source, char shaderName);

public:
	ShaderLoader();
	~ShaderLoader();
	GLuint CreateProgram(char* vertexShaderFileName, char* fragmentShaderFileName);
};