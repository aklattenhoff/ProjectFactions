#include "Phage_PropDesc.h"

Phage::PhagePropDesc::PhagePropDesc()
{
}

Phage::PhagePropDesc::~PhagePropDesc()
{
}

int Phage::PhagePropDesc::Init()
{
	return 0;
}

int Phage::PhagePropDesc::SetPhageProp(Phage::PhageProp* prop)
{
	_phageProp = prop;

	return 0;
}

Phage::PhageProp* Phage::PhagePropDesc::GetPhageProp()
{
	return _phageProp;
}

int Phage::PhagePropDesc::Identity()
{
	_propMat = glm::mat4(1.0f);

	return 0;
}

int Phage::PhagePropDesc::Translate(float x, float y, float z)
{
	_propMat = glm::translate(_propMat, glm::vec3(x, y, z));

	return 0;
}
int Phage::PhagePropDesc::Rotate(float a, float x, float y, float z)
{
	_propMat = glm::rotate(_propMat, glm::radians(a), glm::vec3(x, y, z));

	return 0;
}

int Phage::PhagePropDesc::Scale(float x, float y, float z)
{
	_propMat = glm::scale(_propMat, glm::vec3(x, y, z));

	return 0;
}

glm::mat4 Phage::PhagePropDesc::GetMatrix()
{
	return _propMat;
}
