#include "FactionXMLData.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Parser/EntityListXML.h"
#include "Parser/ActionListXML.h"

FactionXMLData::FactionXMLData()
{
	_entityListXML = NULL;
	_actionListXML = NULL;
}

FactionXMLData::~FactionXMLData()
{
	delete _entityListXML;
	delete _actionListXML;
}

int FactionXMLData::Init()
{
	_entityListXML = new EntityListXML();
	_entityListXML->Init();

	_actionListXML = new ActionListXML();
	_actionListXML->Init();

	return 0;
}

EntityListXML* FactionXMLData::GetEntityListXML()
{
	return _entityListXML;
}

ActionListXML* FactionXMLData::GetActionListXML()
{
	return _actionListXML;
}

ProjectileListXML* FactionXMLData::GetProjectileListXML()
{
	return _projectileListXML;
}
