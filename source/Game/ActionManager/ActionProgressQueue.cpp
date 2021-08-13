#include "ActionProgressQueue.h"

#include "Game/Entity/EntityProcessEvent.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

ActionProgressQueue::ActionProgressQueue()
{
}

ActionProgressQueue::~ActionProgressQueue()
{
}

int ActionProgressQueue::Init()
{
	return 0;
}

int ActionProgressQueue::AddActionProgress(EntityProcessEvent* entProcEvnt, unsigned int textureID)
{
	for (int i = _actionProgressVector.size()-1;i >= 0;i --)
	{
		if (_actionProgressVector[i].entityProcessEventVector.front()->owner == entProcEvnt->owner)
		{
			if (_actionProgressVector[i].entityProcessEventVector.front()->entityBase == entProcEvnt->entityBase)
			{
				_actionProgressVector[i].entityProcessEventVector.push_back(entProcEvnt);
				return 1;
			}
			break;
		}
	}

	_actionProgressVector.push_back(ActionProgress());
	_actionProgressVector.back().entityProcessEventVector.push_back(entProcEvnt);
	_actionProgressVector.back().textureID = textureID;

	return 0;
}

int ActionProgressQueue::Size()
{
	return _actionProgressVector.size();
}

int ActionProgressQueue::GetActionProgress(int index, EntityProcessEvent** entProcEvnt, unsigned int* textureID, int* count)
{
	if (index >= (int)_actionProgressVector.size())
		return -1;

	(*entProcEvnt) = _actionProgressVector[index].entityProcessEventVector.front();
	(*textureID) = _actionProgressVector[index].textureID;
	(*count) = _actionProgressVector[index].entityProcessEventVector.size();

	return 0;
}

int ActionProgressQueue::RemoveActionProgress(EntityProcessEvent* entProcEvnt)
{
	for (int i = 0;i < (int)_actionProgressVector.size();i ++)
	{
		for (int j = 0;j < (int)_actionProgressVector[i].entityProcessEventVector.size();j ++)
		{
			if (_actionProgressVector[i].entityProcessEventVector[j] == entProcEvnt)
			{
				_actionProgressVector[i].entityProcessEventVector.erase(_actionProgressVector[i].entityProcessEventVector.begin());
				if (_actionProgressVector[i].entityProcessEventVector.size() < 1)
					_actionProgressVector.erase(_actionProgressVector.begin()+i);
				return 1;
			}
		}
	}

	return 0;
}
