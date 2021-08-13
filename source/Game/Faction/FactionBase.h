#ifndef _FACTION_BASE_H
#define _FACTION_BASE_H

#include "Game/Entity/EntityBaseCreator.h"

#include "Game/Entity/EntityBaseList.h"

class FactionBase
{
public:
	FactionBase();
	~FactionBase();

	int Init();
	int CreateEntityBaseList(const char* fileName, EntityBaseCreator* entityBaseCreator);

	EntityBaseList* GetEntityBaseList();

private:
	EntityBaseList* _entityBaseList;
};

#endif
