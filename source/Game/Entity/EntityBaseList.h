#ifndef _ENTITY_BASE_LIST_H
#define _ENTITY_BASE_LIST_H

#include <string>
#include <vector>
#include <map>

#include "Game/Entity/EntityBase.h"

class Model;
class ProjectileBase;

namespace Phage
{
	class PhageModel;
	class PhageProp;
}

class EntityBaseList
{
public:
	EntityBaseList();
	~EntityBaseList();

	int Add(EntityBase* entityBase);

	EntityBase* At(std::string str);

	int AddProp(Model* prop, std::string name);
	Model* PropAt(std::string str);

	int AddPhageProp(Phage::PhageProp* prop, std::string name);
	Phage::PhageProp* PhagePropAt(std::string str);

	int AddProjectileBase(ProjectileBase* projectileBase);
	ProjectileBase* ProjectileBaseAt(std::string str);
private:
	std::vector<EntityBase*> _entityBaseVector;
	std::map<std::string, EntityBase*> _entityBaseMap;

	std::vector<Model*> _propVector;
	std::map<std::string, Model*> _propMap;

	std::vector<Phage::PhageProp*> _phagePropVector;
	std::map<std::string, Phage::PhageProp*> _phagePropMap;

	std::vector<ProjectileBase*> _projectileBaseVector;
	std::map<std::string, ProjectileBase*> _projectileBaseMap;
};

#endif
