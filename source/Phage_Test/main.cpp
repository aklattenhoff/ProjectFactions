#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "freetype27.lib")
#pragma comment(lib, "lib3ds-2_0.lib")
#pragma comment(lib, "assimp.lib")

GLuint CompileShader(std::string vertexFileName, std::string fragmentFileName)
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
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	//Error check program
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	return program;
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	SDL_Window*_window = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	SDL_GLContext _glContext = SDL_GL_CreateContext(_window);

	LoadGLExtensions();

	GLuint mainShader = CompileShader("test_shaders/mainVert.fsl", "test_shaders/mainFrag.fsl");
	GLuint postShader = CompileShader("test_shaders/postVert.fsl", "test_shaders/postFrag.fsl");

	glm::mat4 ortho = glm::ortho(0.0f, 1200.f, 800.0f, 0.0f, -1.0f, 1.0f);

	int matrixLoc = glGetUniformLocation(mainShader, "matrix");
	glUseProgram(mainShader);
	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(ortho));

	matrixLoc = glGetUniformLocation(postShader, "matrix");
	glUseProgram(postShader);
	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(ortho));

	int samplerLoc = glGetUniformLocation(postShader, "sampler");
	glUseProgram(postShader);
	glUniform1i(samplerLoc, 3);

	samplerLoc = glGetUniformLocation(postShader, "samplerDiffuse");
	glUseProgram(postShader);
	glUniform1i(samplerLoc, 4);

	samplerLoc = glGetUniformLocation(mainShader, "sampler");
	glUseProgram(mainShader);
	glUniform1i(samplerLoc, 3);

	samplerLoc = glGetUniformLocation(mainShader, "samplerDiffuse");
	glUseProgram(mainShader);
	glUniform1i(samplerLoc, 4);

	int postProcLoc = glGetUniformLocation(mainShader, "postProc");
	glUseProgram(mainShader);
	glUniform1i(postProcLoc, 0);

	float vertices[] = {
		0.0f, 0.0f, 0.0f,
		1200.0f, 0.0f, 0.0f,
		1200.0f, 800.0f, 0.0f,
		0.0f, 800.0f, 0.0f
	};

	GLuint indices[] = {
		0, 2, 1, 0, 3, 2
	};

	float textCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	GLuint _vao, _vbo, _ibo, _tbo;
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);

	glBindVertexArray(_vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * 4, vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * 4, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _tbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 4, textCoords, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	GLuint _FBO, _fbColorTexture, _fbDepthTexture, _fbDiffuseTexture;
	glGenFramebuffers(1, &_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	glGenTextures(1, &_fbColorTexture);
	glBindTexture(GL_TEXTURE_2D, _fbColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1200, 800, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fbColorTexture, 0);

	glGenTextures(1, &_fbDepthTexture);
	glBindTexture(GL_TEXTURE_2D, _fbDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 1200, 800, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _fbDepthTexture, 0);

	glGenTextures(1, &_fbDiffuseTexture);
	glBindTexture(GL_TEXTURE_2D, _fbDiffuseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1200, 800, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _fbDiffuseTexture, 0);

	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	bool done = false;
	SDL_Event sdlEvent;
	bool aDown = false;
	bool dDown = false;
	bool buttonDown[256] = { false };
	float fps;
	float frames = 0.0f;
	unsigned int time = SDL_GetTicks();
	while (!done)
	{
		frames += 1.0f;
		if (SDL_GetTicks() - time >= 1000)
		{
			fps = frames / ((float)(SDL_GetTicks() - time) / 1000.0f);
			frames = 0.0f;
			time = SDL_GetTicks();

			printf("FPS: %f\n", fps);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(mainShader);

		glUniform1i(postProcLoc, 0);

		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, _fbColorTexture);
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, _fbDiffuseTexture);
		glActiveTexture(GL_TEXTURE0);

		//glUseProgram(postShader);
		glUniform1i(postProcLoc, 1);

		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		SDL_GL_SwapWindow(_window);

		while (SDL_PollEvent(&sdlEvent))
		{
			switch (sdlEvent.type)
			{
				case SDL_KEYDOWN:
					if (sdlEvent.key.keysym.sym < 256)
						buttonDown[sdlEvent.key.keysym.sym] = true;
					break;
				case SDL_KEYUP:
					if (sdlEvent.key.keysym.sym < 256)
						buttonDown[sdlEvent.key.keysym.sym] = false;
					break;
				case SDL_QUIT:
					done = 1;
					break;
			}
		}
	}

	return 0;
}