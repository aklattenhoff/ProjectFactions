#ifndef _ACTION_SYSTEM_H
#define _ACTION_SYSTEM_H

class ActionManager;

class ActionSystem
{
public:
	ActionSystem();
	~ActionSystem();

	int Init();

	ActionManager* GetActionManager();
private:
	ActionManager* _actionManager;
};
#endif
