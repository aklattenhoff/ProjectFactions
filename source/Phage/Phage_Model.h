#ifndef _PHAGE_MODEL_H
#define _PHAGE_MODEL_H

#include <stdlib.h>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Phage_Math.h"
#include "Phage_Matrix.h"

#include "Backend/Model.h"
#include "Backend/Animation.h"
#include "Backend/Skeleton.h"

namespace Phage
{
	class PhageSound;
	class PhageModel
	{
	public:
		static const int ORIENTATION_Y = 0;
		static const int ORIENTATION_Z = 1;
	public:
		PhageModel();
		~PhageModel();

		int Init();

		int LoadCollada(std::string fileName);
		int LoadMainTexture(std::string fileName);
		int LoadBumpTexture(std::string fileName);
		int LoadSBCTexture(std::string fileName);

		int SetModel(Model* model);

		int SetOrientation(int orientation);
		int GetOrientation();
		int SetScale(float scale);
		float GetScale();
		int SetName(std::string name);
		std::string GetName();

		int AddAnimation(std::string fileName, int animFlag, float animSpeed);
		Animation* GetAnimation(int animIndex);

		std::vector<aiMatrix4x4>  GetBoneMatrices(int animIndex, float animSecs);

		int SetSfxFileMap(std::map<std::string, Phage::PhageSound*> sfxMap);
		Phage::PhageSound* GetPhageSound(std::string name);

		int Draw();
	private:
		Model* _model;
		int _orientation;
		float _scale;

		std::string _name;

		std::map<std::string, Phage::PhageSound*> _sfxFileMap;
	};
}

#endif
