#ifndef _ACTION_LIST_CREATOR_H
#define _ACTION_LIST_CREATOR_H

#include <vector>

class Parser;
class ActionList;

class ActionListCreator
{
public:
	ActionListCreator();
	~ActionListCreator();

	int Init();

	ActionList* CreateActionList(Parser* parser);
private:
};

#endif
