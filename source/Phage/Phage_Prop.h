#ifndef _PHAGE_PROP_H
#define _PHAGE_PROP_H

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
	class PhageModel;
	class PhageProp
	{
	public:
		PhageProp();
		~PhageProp();

		int Init();

		int SetPhageModel(Phage::PhageModel* model);
		Phage::PhageModel* GetPhageModel();

		int Draw(Phage::PhageCamera* camera);
	private:
		Phage::PhageModel* _phageModel;
	};
}

#endif
