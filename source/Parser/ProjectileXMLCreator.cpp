#include "ProjectileXMLCreator.h"

#include "Parser/ProjectileXML.h"

ProjectileXMLCreator::ProjectileXMLCreator()
{
}

ProjectileXMLCreator::~ProjectileXMLCreator()
{
}

int ProjectileXMLCreator::Init()
{
	return 0;
}

ProjectileXML* ProjectileXMLCreator::CreateFromFile(std::string fileName)
{
	//TODO: Add lots of error checking here!
	tinyxml2::XMLDocument xmlProjectile;
	xmlProjectile.LoadFile(fileName.c_str());

	tinyxml2::XMLElement* projectileElement = xmlProjectile.FirstChildElement("Projectile");

	ProjectileXML* projectileXML = new ProjectileXML();

	projectileXML->_projectileName = projectileElement->Attribute("name");

	projectileXML->_name = projectileElement->FirstChildElement("Name")->GetText();

	tinyxml2::XMLElement* flagElement = projectileElement->FirstChildElement("Flag");
	while (flagElement)
	{
		projectileXML->_flagVector.push_back(flagElement->GetText());
		flagElement = flagElement->NextSiblingElement("Flag");
	}

	projectileXML->_model = projectileElement->FirstChildElement("Model")->GetText();
	projectileXML->_ballisticsFlag = projectileElement->FirstChildElement("BallisticsFlag")->GetText();
	projectileXML->_damageFlag = projectileElement->FirstChildElement("DamageFlag")->GetText();

	tinyxml2::XMLElement* arcMotionElement = projectileElement->FirstChildElement("ArcMotion");
	if (arcMotionElement)
	{
		projectileXML->_arcMotion.dist1 = (float)atof(arcMotionElement->Attribute("dist1"));
		projectileXML->_arcMotion.angle1 = (float)atof(arcMotionElement->Attribute("angle1"));
		projectileXML->_arcMotion.height1 = (float)atof(arcMotionElement->Attribute("height1"));
		projectileXML->_arcMotion.dist2 = (float)atof(arcMotionElement->Attribute("dist2"));
		projectileXML->_arcMotion.angle2 = (float)atof(arcMotionElement->Attribute("angle2"));
		projectileXML->_arcMotion.height2 = (float)atof(arcMotionElement->Attribute("height2"));
	}

	projectileXML->_velocity = (float)atof(projectileElement->FirstChildElement("Velocity")->GetText());


	return projectileXML;
}
