#include "Phage_Prop.h"

#include "Phage_Model.h"

Phage::PhageProp::PhageProp()
{
}

Phage::PhageProp::~PhageProp()
{
}

int Phage::PhageProp::Init()
{
	return 0;
}

int Phage::PhageProp::SetPhageModel(Phage::PhageModel* model)
{
	_phageModel = model;

	return 0;
}

Phage::PhageModel* Phage::PhageProp::GetPhageModel()
{
	return _phageModel;
}

int Phage::PhageProp::Draw(Phage::PhageCamera* camera)
{
	_phageModel->Draw();

	return 0;
}
