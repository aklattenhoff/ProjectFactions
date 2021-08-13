#include "Phage_Entity.h"

#include "Phage_Camera.h"

#include "Phage_Model.h"
#include "Phage_PropDesc.h"

Phage::PhageEntity::PhageEntity()
{
	_phageModel = NULL;
}

Phage::PhageEntity::~PhageEntity()
{
	//delete _phageModel;
}

int Phage::PhageEntity::Init()
{
	//_phageModel = new Phage::PhageModel();
	//_phageModel->Init();

	_matrix.Init();
	_matrix.Identity();

	return 0;
}

int Phage::PhageEntity::SetPhageModel(Phage::PhageModel* model)
{
	_phageModel = model;

	return 0;
}

int Phage::PhageEntity::Identity()
{
	_matrix.Identity();

	return 0;
}

int Phage::PhageEntity::SetBaseRotate(float a, float x, float y, float z)
{
	_matrix.SetBaseRotate(a, x, y, z);

	return 0;
}

int Phage::PhageEntity::SetTranslate(float x, float y, float z)
{
	_matrix.SetTranslate(x, y, z);

	return 0;
}

int Phage::PhageEntity::SetRotate(float a, float x, float y, float z)
{
	_matrix.SetRotate(a, x, y, z);

	return 0;
}

int Phage::PhageEntity::SetScale(float x, float y, float z)
{
	_matrix.SetScale(x, y, z);

	return 0;
}

int Phage::PhageEntity::AddTranslate(float x, float y, float z)
{
	_matrix.Translate(x, y, z);

	return 0;
}

int Phage::PhageEntity::AddRotate(float a, float x, float y, float z)
{
	_matrix.Rotate(a, x, y, z);

	return 0;
}

int Phage::PhageEntity::AddScale(float x, float y, float z)
{
	_matrix.Scale(x, y, z);

	return 0;
}

glm::mat4 Phage::PhageEntity::GetMatrix()
{
	return _matrix.GetMatrix(1);
}

int Phage::PhageEntity::SetAnimationIndex(int animIndex)
{
	_animationIndex = animIndex;

	return 0;
}

int Phage::PhageEntity::GetAnimationIndex()
{
	return _animationIndex;
}

bool Phage::PhageEntity::HasAnimation(int animIndex)
{
	return _phageModel->GetAnimation(animIndex);
}

int Phage::PhageEntity::SetAnimationSeconds(float animSecs)
{
	_animationSeconds = animSecs;

	return 0;
}

float Phage::PhageEntity::GetAnimationSeconds()
{
	return _animationSeconds;
}

int Phage::PhageEntity::AddPhagePropDesc(Phage::PhagePropDesc prop)
{
	_phagePropVector.push_back(prop);

	return 0;
}

int Phage::PhageEntity::PropDescSize()
{
	return _phagePropVector.size();
}

Phage::PhagePropDesc Phage::PhageEntity::PropDescAt(int index)
{
	return _phagePropVector[index];
}

int Phage::PhageEntity::ClearPhagePropDesc()
{
	_phagePropVector.clear();

	return 0;
}

int Phage::PhageEntity::SetIsSelected(bool f)
{
	_isSelected = f;

	return 0;
}

bool Phage::PhageEntity::GetIsSelected()
{
	return _isSelected;
}

int Phage::PhageEntity::SetBBCenter(glm::vec3 center)
{
	_bbCenter = center;

	return 0;
}

glm::vec3 Phage::PhageEntity::GetBBCenter()
{
	return _bbCenter;
}

std::vector<aiMatrix4x4>  Phage::PhageEntity::GetBoneMatrices(int animIndex, float animSecs)
{
	return _phageModel->GetBoneMatrices(animIndex, animSecs);
}

int Phage::PhageEntity::Draw(Phage::PhageCamera* camera)
{
	_phageModel->Draw();

	return 0;
}
