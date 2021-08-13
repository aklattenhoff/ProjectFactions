#include "ProjectileListXMLCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Parser/ProjectileXMLCreator.h"
#include "Parser/ProjectileListXML.h"

#include "Parser/EntityListXML.h"

ProjectileListXMLCreator::ProjectileListXMLCreator()
{
	_projectileXMLCreator = NULL;
}

ProjectileListXMLCreator::~ProjectileListXMLCreator()
{
	delete _projectileXMLCreator;
}

int ProjectileListXMLCreator::Init()
{
	_projectileXMLCreator = new ProjectileXMLCreator();
	_projectileXMLCreator->Init();

	return 0;
}

ProjectileListXML* ProjectileListXMLCreator::CreateFromEntityList(EntityListXML* entityListXML)
{
	ProjectileListXML* projectileListXML = NULL;

	if (entityListXML->GetProjectileFileVector().size() > 0)
	{
		projectileListXML = new ProjectileListXML();
		projectileListXML->Init();
	}


	for (unsigned int i = 0;i < entityListXML->GetProjectileFileVector().size();i ++)
	{
		ProjectileXML* projectileXML = _projectileXMLCreator->CreateFromFile(entityListXML->GetProjectileFileVector()[i]);
		projectileListXML->_projectileXMLVector.push_back(projectileXML);
	}

	return projectileListXML;
}
