#ifndef _PHAGE_PROP_DESC_H
#define _PHAGE_PROP_DESC_H

#include <stdlib.h>
#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Phage_Math.h"
#include "Phage_Matrix.h"

namespace Phage
{
	class PhageCamera;
	class PhageProp;
	class PhagePropDesc
	{
	public:
		PhagePropDesc();
		~PhagePropDesc();

		int Init();

		int SetPhageProp(Phage::PhageProp* prop);
		Phage::PhageProp* GetPhageProp();

		int Identity();
		int Translate(float x, float y, float z);
		int Rotate(float a, float x, float y, float z);
		int Scale(float x, float y, float z);

		glm::mat4 GetMatrix();
	private:
		Phage::PhageProp* _phageProp;

		glm::mat4 _propMat;
	};
}

#endif
