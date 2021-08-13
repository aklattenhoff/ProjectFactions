#ifndef _PHAGE_UI_ENTITY_H
#define _PHAGE_UI_ENTITY_H

#include <stdlib.h>
#include <string>
#include <vector>

#include "Phage_Matrix.h"

#include "Phage_UITextureQuad.h"

namespace Phage
{
	class PhageUIEntity
	{
	public:
		PhageUIEntity();
		~PhageUIEntity();

		int Init();
		int CreateAndAddUITextureQuad(float x, float y, float w, float h, std::string fileName, bool upsideDown = false);
		int CreateAndAddUITextureQuad(float x, float y, float w, float h, GLuint textureID, bool upsideDown = false);
		int SetTextureID(GLuint textureID);
		int SetTranslate(float x, float y, float depth = 0.0f);
		int SetScale(float x, float y);
		int SetRotate(float a);
		glm::mat4 GetMatrix();

		int Draw();
	private:
		Phage::Matrix _matrix;
		std::vector<Phage::PhageUITextureQuad*> _phageUITextureQuadVector;
	};
}

#endif
