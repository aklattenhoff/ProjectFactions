#include "EntityBaseList.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/ProjectileSystem/ProjectileBase.h"

EntityBaseList::EntityBaseList()
{
}

EntityBaseList::~EntityBaseList()
{
}

int EntityBaseList::Add(EntityBase* entityBase)
{
	_entityBaseVector.push_back(entityBase);
	_entityBaseMap.emplace(entityBase->EntityName(), entityBase);

	return 0;
}

EntityBase* EntityBaseList::At(std::string str)
{
	EntityBase* entityBase = NULL;
	try
	{
		entityBase = _entityBaseMap.at(str);
	}
	catch (const std::out_of_range&)
	{
		return NULL;
	}
	return entityBase;
}

int EntityBaseList::AddProp(Model* prop, std::string name)
{
	_propVector.push_back(prop);
	_propMap.emplace(name, prop);

	return 0;
}

Model* EntityBaseList::PropAt(std::string str)
{
	Model* prop = NULL;
	try
	{
		prop = _propMap.at(str);
	}
	catch (const std::out_of_range&)
	{
		return NULL;
	}
	return prop;
}

int EntityBaseList::AddPhageProp(Phage::PhageProp* prop, std::string name)
{
	_phagePropVector.push_back(prop);
	_phagePropMap.emplace(name, prop);

	return 0;
}

Phage::PhageProp* EntityBaseList::PhagePropAt(std::string str)
{
	Phage::PhageProp* prop = NULL;
	try
	{
		prop = _phagePropMap.at(str);
	}
	catch (const std::out_of_range&)
	{
		return NULL;
	}
	return prop;
}

int EntityBaseList::AddProjectileBase(ProjectileBase* projectileBase)
{
	_projectileBaseVector.push_back(projectileBase);
	_projectileBaseMap.emplace(projectileBase->GetProjectileName(), projectileBase);

	return 0;
}

ProjectileBase* EntityBaseList::ProjectileBaseAt(std::string str)
{
	ProjectileBase* projectileBase = NULL;
	try
	{
		projectileBase = _projectileBaseMap.at(str);
	}
	catch (const std::out_of_range&)
	{
		return NULL;
	}
	return projectileBase;
}
