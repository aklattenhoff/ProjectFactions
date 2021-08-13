#include "Phage_Model.h"

#include "Backend/Model.h"

Phage::PhageModel::PhageModel()
{
	_model = NULL;
}

Phage::PhageModel::~PhageModel()
{
	delete _model;
}

int Phage::PhageModel::Init()
{
	_model = new Model();
	_model->Init();

	_orientation = Phage::PhageModel::ORIENTATION_Y;
	_scale = 1.0f;

	return 0;
}

int Phage::PhageModel::LoadCollada(std::string fileName)
{
	return _model->LoadScene(fileName);
}

int Phage::PhageModel::LoadMainTexture(std::string fileName)
{
	return _model->LoadMainTexture(fileName);
}

int Phage::PhageModel::LoadBumpTexture(std::string fileName)
{
	return _model->LoadBumpTexture(fileName);
}

int Phage::PhageModel::LoadSBCTexture(std::string fileName)
{
	return _model->LoadSBCTexture(fileName);
}

int Phage::PhageModel::SetModel(Model* model)
{
	_model = model;

	return 0;
}

int Phage::PhageModel::SetOrientation(int orientation)
{
	_orientation = orientation;

	return 0;
}

int Phage::PhageModel::GetOrientation()
{
	return _orientation;
}

int Phage::PhageModel::SetScale(float scale)
{
	_scale = scale;

	return 0;
}

float Phage::PhageModel::GetScale()
{
	return _scale;
}

int Phage::PhageModel::SetName(std::string name)
{
	_name = name;

	return 0;
}

std::string Phage::PhageModel::GetName()
{
	return _name;
}

int Phage::PhageModel::AddAnimation(std::string fileName, int animFlag, float animSpeed)
{
	_model->AddAnimation(fileName, animFlag, animSpeed);
	return 0;
}

Animation* Phage::PhageModel::GetAnimation(int animIndex)
{
	return _model->GetAnimation(animIndex);
}

std::vector<aiMatrix4x4>  Phage::PhageModel::GetBoneMatrices(int animIndex, float animSecs)
{
	return _model->GetBoneMatrices(animIndex, animSecs);
}

int Phage::PhageModel::SetSfxFileMap(std::map<std::string, Phage::PhageSound*> sfxMap)
{
	_sfxFileMap = sfxMap;

	return 0;
}

Phage::PhageSound* Phage::PhageModel::GetPhageSound(std::string name)
{
	return _sfxFileMap[name];
}

int Phage::PhageModel::Draw()
{
	_model->Draw();
	
	return 0;
}
