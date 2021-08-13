#ifndef _PHAGE_MVP_H
#define _PHAGE_MVP_H

#include <stdlib.h>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Phage
{
	class MVP
	{
	public:
		MVP();
		~MVP();

		int Init();

		int Identity(const int matrix);
		int Translate(const int matrix, float x, float y, float z);
		int Rotate(const int matrix, float a, float x, float y, float z);
		int Scale(const int matrix, float x, float y, float z);
	private:
		glm::mat4 _projection;
		glm::mat4 _ortho;
		glm::mat4 _view;
		glm::mat4 _model;
		glm::mat4 _mvp;
	};
}

#endif
