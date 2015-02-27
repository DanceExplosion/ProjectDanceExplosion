#include "ShaderLoader.h"

ShaderLoader::ShaderLoader(){};
ShaderLoader::~ShaderLoader(){};

std::string ShaderLoader::ReadShader(char* filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);
	
	if(!file.good())
	{
		std::cout<<"can't read file "<<filename<<" \n"<<std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int) file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();

	return shaderCode;
}

GLuint ShaderLoader::CreateShader(GLenum shaderType, std::string source, char shaderName)
{
	int compileStatus = 0;

	GLuint shader = glCreateShader(shaderType);

	const char* ptr_shaderCode = source.c_str();
	const int size = source.size();

	glShaderSource(shader, 1, &ptr_shaderCode, &size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	// if shader doesn't compile
	if(compileStatus == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> shaderLog(infoLogLength);
		// 
        glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderLog[0]);
		std::cout << "ERROR compiling shader: " << shaderName << std::endl;
		std::cout<< &shaderLog[0] << std::endl;
		// & return dummy data
        return 0;
	}

	return shader;
}

GLuint ShaderLoader::CreateProgram(char* vertexShaderFileName, char* fragmentShaderFileName)
{

	std::string vertexShaderCode = ReadShader(vertexShaderFileName);
	std::string fragmentShaderCode = ReadShader(fragmentShaderFileName);

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, 'v');
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, 'f');

	int linkResult = 0;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);

	if(linkResult == GL_FALSE)
	{
		std::cout << "Link Error" << std::endl;
		int infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> programLog(infoLogLength);
		glGetProgramInfoLog(program, infoLogLength, NULL, &programLog[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl;
		std::cout << &programLog[0] << std::endl;
		return 0;
	}

	return program;
}
