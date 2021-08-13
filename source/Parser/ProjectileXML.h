#ifndef _PROJECTILE_XML_H
#define _PROJECTILE_XML_H

#include <string>
#include <vector>

class ProjectileXML
{
public:
	struct ArcMotion
	{
		float dist1, angle1, height1, dist2, angle2, height2;
	};
public:
	ProjectileXML();
	~ProjectileXML();
public:
	friend class ProjectileXMLCreator;

	std::string _projectileName;
	std::string _name;
	std::vector<std::string> _flagVector;
	std::string _model;
	std::string _ballisticsFlag;
	std::string _damageFlag;
	ArcMotion _arcMotion;
	float _velocity;
};
#endif
