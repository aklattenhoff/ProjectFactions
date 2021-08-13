#ifndef _FACTION_BASE_LIST_H
#define _FACTION_BASE_LIST_H

#include <string>
#include <vector>
#include <map>

#include "Game/Entity/EntityBaseCreator.h"

#include "Game/Faction/FactionBase.h"

class FactionBaseList
{
public:
	FactionBaseList();
	~FactionBaseList();
	
	int Init();
	int CreateAllFactions(const char* fileName);
	FactionBase* At(std::string str);

private:
	EntityBaseCreator* _entityBaseCreator;

	std::vector<FactionBase*> _factionBaseVector;
	std::map<std::string, FactionBase*> _factionBaseMap;
};

#endif
