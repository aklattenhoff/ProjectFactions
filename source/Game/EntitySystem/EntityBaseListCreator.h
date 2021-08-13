#ifndef _ENTITY_BASE_LIST_CREATOR_H
#define _ENTITY_BASE_LIST_CREATOR_H

#include <vector>

class EntityBaseCreator;
class Parser;
class EntityBaseList;

class EntityBaseListCreator
{
public:
	EntityBaseListCreator();
	~EntityBaseListCreator();

	int Init();

	EntityBaseList* CreateEntityBaseList(Parser* parser);
private:
	EntityBaseCreator* _entityBaseCreator;
};

#endif
