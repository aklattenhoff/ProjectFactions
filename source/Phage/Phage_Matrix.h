#ifndef _PHAGE_MATRIX_H
#define _PHAGE_MATRIX_H

#include <stdlib.h>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Phage
{
	class Matrix
	{
	public:
		Matrix();
		~Matrix();

		int Init();

		int Identity();
		int Translate(float x, float y, float z);
		int SetTranslate(float x, float y, float z);
		int Rotate(float a, float x, float y, float z);
		int SetRotate(float a, float x, float y, float z);
		int SetBaseRotate(float a, float x, float y, float z);
		int Scale(float x, float y, float z);
		int SetScale(float x, float y, float z);
		int Perspective(float fovy, float aspect, float near, float far);
		int Ortho(float left, float right, float bottom, float top, float near, float far);
		glm::mat4 GetMatrix(int type);
	private:
		glm::mat4 _translation;
		glm::mat4 _baseRotation;
		glm::mat4 _rotation;
		glm::mat4 _scale;
		glm::mat4 _matrix;
		bool _recalcMatrix;
		bool _perspectiveMatrix;
	};
}

#endif
