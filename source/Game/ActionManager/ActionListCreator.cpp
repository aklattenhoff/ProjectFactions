#include "ActionListCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/ActionManager/ActionList.h"
#include "Game/ActionManager/Action.h"

//Parser
#include "Parser/Parser.h"
#include "Parser/FactionListXML.h"
#include "Parser/FactionXMLData.h"
#include "Parser/ActionListXML.h"
#include "Parser/ActionXML.h"

#include "Texturer/TextureLoader.h"

ActionListCreator::ActionListCreator()
{
}

ActionListCreator::~ActionListCreator()
{
}

int ActionListCreator::Init()
{
	return 0;
}

ActionList* ActionListCreator::CreateActionList(Parser* parser)
{
	ActionList* actionList = new ActionList();
	
	std::vector<FactionXMLData*> factionXMLDataVector = parser->GetFactionListXML()->GetFactionXMLDataVector();
	for (unsigned int i = 0;i < factionXMLDataVector.size();i ++)
	{
		std::vector<ActionXML*> actionXMLVector = factionXMLDataVector[i]->GetActionListXML()->GetActionXMLVector();
		for (unsigned int j = 0;j < actionXMLVector.size();j ++)
		{
			if (!actionList->At(actionXMLVector[j]->GetActionName()))
			{
				Action* action = new Action();
				action->actionName = actionXMLVector[j]->GetActionName();
				action->actionType = actionXMLVector[j]->GetActionType();
				action->buildObjectName = actionXMLVector[j]->GetBuildObjectName();
				action->iconFileName = actionXMLVector[j]->GetIconFileName();
				action->trainObjectName = actionXMLVector[j]->GetTrainObjectName();
				action->buildWallConnectorObjectName = actionXMLVector[j]->GetBuildWallConnectorObjectName();
				action->buildWall2x1ObjectName = actionXMLVector[j]->GetBuildWall2x1ObjectName();
				action->buildWall4x1ObjectName = actionXMLVector[j]->GetBuildWall4x1ObjectName();
				action->buildWall6x1ObjectName = actionXMLVector[j]->GetBuildWall6x1ObjectName();
				action->upgradeObjectName = actionXMLVector[j]->GetUpgradeObjectName();
				action->upgradeTime = actionXMLVector[j]->GetUpgradeTime();
				//Load texture from file
				action->iconTexture = TextureLoader_LoadFromFile(action->iconFileName.c_str());
				actionList->Add(action);
				DebugOutput("Created Action: %s\n", actionXMLVector[j]->GetActionName().c_str());
			}
		}
	}

	return actionList;
}
