#include "ProjectileBase.h"

#include "Parser/ProjectileXML.h"
#include "Phage/Backend/Model.h"

ProjectileBase::ProjectileBase()
{
}

ProjectileBase::~ProjectileBase()
{
}

int ProjectileBase::Init()
{

	return 0;
}

int ProjectileBase::SetProjectileXML(ProjectileXML* projectileXML)
{
	_projectileXML = projectileXML;

	return 0;
}

ProjectileXML* ProjectileBase::GetProjectileXML()
{
	return _projectileXML;
}

int ProjectileBase::SetModel(Model* model)
{
	_model = model;

	return 0;
}

std::string ProjectileBase::GetProjectileName()
{
	return _projectileXML->_projectileName;
}
