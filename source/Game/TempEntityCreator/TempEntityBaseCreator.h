#ifndef _TEMP_ENTITY_BASE_CREATOR_H
#define _TEMP_ENTITY_BASE_CREATOR_H

#include <string>

class TempEntityModelXMLCreator;

class EntityXML;
class EntityBase;
class Model;
class ProjectileXML;
class ProjectileBase;

namespace Phage
{
	class PhageEntity;
	class PhageModel;
}

class TempEntityBaseCreator
{
public:
	TempEntityBaseCreator();
	~TempEntityBaseCreator();

	int Init();

	EntityBase* CreateEntityBase(EntityXML* entityXML);
	Model* CreateProp(std::string fileName);
	ProjectileBase* CreateProjectileBase(ProjectileXML* projectileXML);

	Phage::PhageModel* CreatePropPhage(std::string fileName);
private:
	TempEntityModelXMLCreator* _entityModelXMLCreator;
};

#endif
