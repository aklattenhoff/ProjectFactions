#include "Phage_Matrix.h"

Phage::Matrix::Matrix()
{
}

Phage::Matrix::~Matrix()
{
}

int Phage::Matrix::Init()
{
	_translation = glm::mat4(1.0f);
	_rotation = glm::mat4(1.0f);
	_scale = glm::mat4(1.0f);
	_matrix = glm::mat4(1.0f);

	_recalcMatrix = true;
	_perspectiveMatrix = false;

	return 0;
}

int Phage::Matrix::Identity()
{
	_translation = glm::mat4(1.0f);
	_rotation = glm::mat4(1.0f);
	_scale = glm::mat4(1.0f);
	_matrix = glm::mat4(1.0f);

	_recalcMatrix = true;

	return 0;
}

int Phage::Matrix::Translate(float x, float y, float z)
{
	_translation = glm::translate(_translation, glm::vec3(x, y, z));

	_recalcMatrix = true;

	return 0;
}

int Phage::Matrix::SetTranslate(float x, float y, float z)
{
	_translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

	_recalcMatrix = true;

	return 0;
}

int Phage::Matrix::Rotate(float a, float x, float y, float z)
{
	_rotation = glm::rotate(_rotation, glm::radians(a), glm::vec3(x, y, z));

	_recalcMatrix = true;

	return 0;
}

int Phage::Matrix::SetRotate(float a, float x, float y, float z)
{
	_rotation = glm::rotate(_baseRotation, glm::radians(a), glm::vec3(x, y, z));

	_recalcMatrix = true;

	return 0;
}

int Phage::Matrix::SetBaseRotate(float a, float x, float y, float z)
{
	_baseRotation = glm::rotate(glm::mat4(1.0f), glm::radians(a), glm::vec3(x, y, z));

	_rotation = glm::rotate(_rotation, glm::radians(a), glm::vec3(x, y, z));

	_recalcMatrix = true;

	return 0;
}

int Phage::Matrix::Scale(float x, float y, float z)
{
	_scale = glm::scale(_scale, glm::vec3(x, y, z));

	_recalcMatrix = true;

	return 0;
}

int Phage::Matrix::SetScale(float x, float y, float z)
{
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));

	_recalcMatrix = true;

	return 0;
}

int Phage::Matrix::Perspective(float fovy, float aspect, float near, float far)
{
	_matrix = glm::perspective(glm::radians(fovy), aspect, near, far);

	_perspectiveMatrix = true;

	return 0;
}

int Phage::Matrix::Ortho(float left, float right, float bottom, float top, float near, float far)
{
	_matrix = glm::ortho(left, right, bottom, top, near, far);

	_perspectiveMatrix = true;

	return 0;
}

glm::mat4 Phage::Matrix::GetMatrix(int type)
{
	if (_perspectiveMatrix)
		return _matrix;

	if (_recalcMatrix)
	{
		if (type == 0)
			_matrix = _scale * _rotation * _translation;
		if (type == 1)
			_matrix = _translation * _rotation * _scale;

		_recalcMatrix = false;
	}

	return _matrix;
}
