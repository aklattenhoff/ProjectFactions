#ifndef _PROJECTILE_BASE_H
#define _PROJECTILE_BASE_H

#include <string>

class ProjectileXML;
class Model;

class ProjectileBase
{
public:
	ProjectileBase();
	~ProjectileBase();

	int Init();

	int SetProjectileXML(ProjectileXML* projectileXML);
	ProjectileXML* GetProjectileXML();
	int SetModel(Model* model);

	std::string GetProjectileName();
private:
	ProjectileXML* _projectileXML;
	Model* _model;
};
#endif
