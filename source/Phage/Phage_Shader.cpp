#include "Phage_Shader.h"

#include <fstream>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "loadGLE.h"
#include "Debug/Debug.h"

Phage::PhageShader::PhageShader()
{
}

Phage::PhageShader::~PhageShader()
{
}

int Phage::PhageShader::Init()
{
	return 0;
}

int Phage::PhageShader::LoadShader(std::string vertexFileName, std::string fragmentFileName)
{
	//Open both files
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	vertexFile.open(vertexFileName.c_str(), std::ios::in);
	if (!vertexFile) return -1;
	fragmentFile.open(fragmentFileName.c_str(), std::ios::in);
	if (!fragmentFile) return -1;

	//Load the text from file
	std::string vertexText;
	while (vertexFile.good())
	{
		vertexText.push_back(vertexFile.get());
	}
	vertexText.pop_back();
	vertexFile.close();

	std::string fragmentText;
	while (fragmentFile.good())
	{
		fragmentText.push_back(fragmentFile.get());
	}
	fragmentText.pop_back();
	fragmentFile.close();

	//Compiles and links shaders
	GLuint vertexShader;
	GLuint fragmentShader;
	GLint status;

	//Compile vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderCharString = vertexText.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderCharString, NULL);
	glCompileShader(vertexShader);

	//Error check vertex shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(vertexShader, infoLogLength, NULL, strInfoLog);

		fprintf(stderr, "Compile failure in vertex shader:\n%s\n", strInfoLog);
		delete[] strInfoLog;

		return -1;
	}

	//Compile fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderCharString = fragmentText.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderCharString, NULL);
	glCompileShader(fragmentShader);

	//Error check fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, strInfoLog);

		fprintf(stderr, "Compile failure in vertex shader:\n%s\n", strInfoLog);
		delete[] strInfoLog;

		return -1;
	}

	//Create program
	_vertexShaderVector.push_back(vertexShader);
	_fragmentShaderVector.push_back(fragmentShader);

	_programVector.push_back(glCreateProgram());
	glAttachShader(_programVector.back(), vertexShader);
	glAttachShader(_programVector.back(), fragmentShader);
	glLinkProgram(_programVector.back());

	//Error check program
	glGetProgramiv(_programVector.back(), GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(_programVector.back(), GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(_programVector.back(), infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;

		return -1;
	}

	return 0;
}

int Phage::PhageShader::UseShader(int index)
{
	glUseProgram(_programVector.at(index));

	return 0;
}

int Phage::PhageShader::LoadUniformLocation(int index, std::string variable)
{
	DebugOutput("Loading Uniform: %s...", variable.c_str());
	int returnVal = glGetUniformLocation(_programVector.at(index), variable.c_str());
	DebugOutput("%i\n", returnVal);

	return returnVal;
}

int Phage::PhageShader::SetUniformValue(int index, int location, const float* matrix)
{
	glUseProgram(_programVector.at(index));
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix);

	return 0;
}

int Phage::PhageShader::SetUniformValue(int index, int location, int i)
{
	glUseProgram(_programVector.at(index));
	glUniform1i(location, i);

	return 0;
}

int Phage::PhageShader::SetUniformValue(int index, int location, int numInts, int* i)
{
	glUseProgram(_programVector.at(index));
	glUniform1iv(location, numInts, i);

	return 0;
}

int Phage::PhageShader::SetUniformValue(int index, int location, int numFloats, float* f, int uniformType)
{
	glUseProgram(_programVector.at(index));
	
	if (uniformType == Phage::PhageShader::UNIFORM_FLOAT)
	{
		if (numFloats == 3)
			glUniform3fv(location, 1, f);
		if (numFloats == 2)
			glUniform2fv(location, 1, f);
		if (numFloats == 64)
			glUniform3fv(location, 64, f);
	}
	else if (uniformType == Phage::PhageShader::UNIFORM_MATRIX)
	{
		glUniformMatrix4fv(location, numFloats, GL_TRUE, f);
	}

	return 0;
}
