#ifndef _ENTITY_RENDERER_H
#define _ENTITY_RENDERER_H

#define GLM_FORCE_RADIANS

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

//#include "Graphics/Camera.h"
#include "Game/Entity/EntityList.h"
#include "Game/Entity/Entity.h"
#include "Game/World.h"
#include "Game/GUI/GUIIcon.h"

//TEST: REMOVE!!!
//#include "Game/TempEntityCreator/TempEntityModelXMLCreator.h"
//#include "Graphics/Model/Model.h"

#include "Renderer/RS/TerrainDecal.h"

#include "Phage/Phage.h"

class EntityRenderer
{
public:
	EntityRenderer();
	~EntityRenderer();

	int Init();

	int RenderEntities(World* world, EntityList* entityList);
	int PhageModelCreate();
private:
	//int RenderHealthIndicator(Camera* camera, Entity* entity);
private:
	GLuint _buildProgressTextureID;
	GLuint _buildProgressBackTextureID;
	bool glow;
	bool bump;
	bool spec;

	//Framebuffer
	GLuint FBO;
	GLuint fbDepth;

	GUIIcon* guiScreen;

	//TEST: REMOVE!
	//TempEntityModelXMLCreator* tempModelCreator;
	//Model* swordModel;

	GLuint _textureSelectionCircle;
	RS::TerrainDecal _rsSelectionCircle;

	GLuint _farmDecalTexture;
	GLuint _farmBumpDecalTexture;

	GLuint _manorDecalTexture;

	GLuint _selectionSquare64x64;

	//Phage::PhageEntity* _phageEntityRenderer;
};

#endif
