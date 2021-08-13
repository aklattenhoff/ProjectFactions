#ifndef _ACTION_LIST_H
#define _ACTION_LIST_H

#include <vector>
#include <map>

class Action;

class ActionList
{
public:
	ActionList();
	~ActionList();

	int Add(Action* action);
	Action* At(std::string str);
	int Size();
private:
	std::vector<Action*> _actionVector;
	std::map<std::string, Action*> _actionMap;
};

#endif
