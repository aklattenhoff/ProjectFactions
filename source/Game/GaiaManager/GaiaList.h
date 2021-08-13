#ifndef _GAIA_MANAGER_H
#define _GAIA_MANAGER_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class World;

class GaiaManager
{
public:
	GaiaManager();
	~GaiaManager();

	//Initializes the entity manager
	int Init();

	int ProcessGaia(World* world);
private:
};

#endif
