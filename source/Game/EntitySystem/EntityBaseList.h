#ifndef _ENTITY_BASE_LIST_H
#define _ENTITY_BASE_LIST_H

#include <string>
#include <vector>
#include <map>

class EntityBase;

class EntityBaseList
{
public:
	EntityBaseList();
	~EntityBaseList();

	int Add(EntityBase* entityBase);

	EntityBase* At(std::string str);
private:
	std::vector<EntityBase*> _entityBaseVector;
	std::map<std::string, EntityBase*> _entityBaseMap;
};

#endif
