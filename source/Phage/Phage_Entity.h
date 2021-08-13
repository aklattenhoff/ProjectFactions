#ifndef _PHAGE_ENTITY_H
#define _PHAGE_ENTITY_H

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
	class PhageProp;
	class PhagePropDesc;
	class PhageEntity
	{
	public:
		PhageEntity();
		~PhageEntity();

		int Init();

		int SetPhageModel(Phage::PhageModel* model);

		int Identity();
		int SetBaseRotate(float a, float x, float y, float z);
		int SetTranslate(float x, float y, float z);
		int SetRotate(float a, float x, float y, float z);
		int SetScale(float x, float y, float z);
		int AddTranslate(float x, float y, float z);
		int AddRotate(float a, float x, float y, float z);
		int AddScale(float x, float y, float z);

		glm::mat4 GetMatrix();

		int SetAnimationIndex(int animIndex);
		int GetAnimationIndex();
		bool HasAnimation(int animIndex);
		int SetAnimationSeconds(float animSecs);
		float GetAnimationSeconds();

		int AddPhagePropDesc(Phage::PhagePropDesc propDesc);
		int PropDescSize();
		Phage::PhagePropDesc PropDescAt(int index);
		int ClearPhagePropDesc();

		int SetIsSelected(bool f);
		bool GetIsSelected();

		int SetBBCenter(glm::vec3 center);
		glm::vec3 GetBBCenter();

		std::vector<aiMatrix4x4> GetBoneMatrices(int animIndex, float animSecs);

		int Draw(Phage::PhageCamera* camera);
	private:
		Phage::PhageModel* _phageModel;

		Phage::Matrix _matrix;

		int _animationIndex;
		float _animationSeconds;

		std::vector<Phage::PhagePropDesc> _phagePropVector;

		bool _isSelected;

		glm::vec3 _bbCenter;
	};
}

#endif
