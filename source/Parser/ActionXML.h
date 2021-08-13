#ifndef _ACTION_XML_H
#define _ACTION_XML_H

#include <string>

class ActionXML
{
public:
	static const int TYPE_BUILD = 0;
	static const int TYPE_TRAIN = 1;
	static const int TYPE_BUILD_WALL = 2;
	static const int TYPE_UPGRADE = 3;
public:
	ActionXML();
	~ActionXML();

	std::string GetActionName();
	int GetActionType();
	std::string GetIconFileName();
	std::string GetBuildObjectName();
	std::string GetTrainObjectName();
	std::string GetBuildWallConnectorObjectName();
	std::string GetBuildWall2x1ObjectName();
	std::string GetBuildWall4x1ObjectName();
	std::string GetBuildWall6x1ObjectName();

	std::string GetUpgradeObjectName();
	float GetUpgradeTime();
private:
	friend class ActionXMLCreator;

	std::string _actionName;
	int _actionType;

	std::string _iconFileName;

	std::string _buildObjectName;
	std::string _trainObjectName;

	std::string _buildWallConnectorObjectName;
	std::string _buildWall2x1ObjectName;
	std::string _buildWall4x1ObjectName;
	std::string _buildWall6x1ObjectName;

	std::string _upgradeObjectName;
	float _upgradeTime;
};
#endif
