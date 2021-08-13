#include "Phage_MVP.h"

Phage::MVP::MVP()
{
}

Phage::MVP::~MVP()
{
}

int Phage::MVP::Init()
{
	_projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
	_ortho = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	_view = glm::mat4(1.0f);
	_model = glm::mat4(1.0f);

	return 0;
}

int Identity(const int matrix)
{
	return 0;
}

int Translate(const int matrix, float x, float y, float z)
{
	return 0;
}

int Rotate(const int matrix, float a, float x, float y, float z)
{
	return 0;
}

int Scale(const int matrix, float x, float y, float z)
{
	return 0;
}
