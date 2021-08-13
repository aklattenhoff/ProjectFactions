#include "ActionXML.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

ActionXML::ActionXML()
{
}

ActionXML::~ActionXML()
{
}

std::string ActionXML::GetActionName()
{
	return _actionName;
}

int ActionXML::GetActionType()
{
	return _actionType;
}

std::string ActionXML::GetIconFileName()
{
	return _iconFileName;
}

std::string ActionXML::GetBuildObjectName()
{
	return _buildObjectName;
}

std::string ActionXML::GetTrainObjectName()
{
	return _trainObjectName;
}

std::string ActionXML::GetBuildWallConnectorObjectName()
{
	return _buildWallConnectorObjectName;
}

std::string ActionXML::GetBuildWall2x1ObjectName()
{
	return _buildWall2x1ObjectName;
}

std::string ActionXML::GetBuildWall4x1ObjectName()
{
	return _buildWall4x1ObjectName;
}

std::string ActionXML::GetBuildWall6x1ObjectName()
{
	return _buildWall6x1ObjectName;
}

std::string ActionXML::GetUpgradeObjectName()
{
	return _upgradeObjectName;
}

float ActionXML::GetUpgradeTime()
{
	return _upgradeTime;
}
