#ifndef _TEMP_ENTITY_BASE_LIST_CREATOR_H
#define _TEMP_ENTITY_BASE_LIST_CREATOR_H

#include <vector>

class TempEntityBaseCreator;
class Parser;
class EntityBaseList;

class TempEntityBaseListCreator
{
public:
	TempEntityBaseListCreator();
	~TempEntityBaseListCreator();

	int Init();

	EntityBaseList* CreateEntityBaseList(Parser* parser);
private:
	TempEntityBaseCreator* _entityBaseCreator;
};

#endif
