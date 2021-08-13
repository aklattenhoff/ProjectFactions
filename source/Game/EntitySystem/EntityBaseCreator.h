#ifndef _ENTITY_BASE_CREATOR_H
#define _ENTITY_BASE_CREATOR_H

class EntityModelXMLCreator;

class EntityXML;
class EntityBase;

class EntityBaseCreator
{
public:
	EntityBaseCreator();
	~EntityBaseCreator();

	int Init();

	EntityBase* CreateEntityBase(EntityXML* entityXML);
private:
	EntityModelXMLCreator* _entityModelXMLCreator;
};

#endif
