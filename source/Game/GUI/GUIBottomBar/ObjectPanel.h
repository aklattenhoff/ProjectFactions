#ifndef _OBJECT_PANEL_H
#define _OBJECT_PANEL_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Input/Input.h"

#include "Texturer/TextureLoader.h"

#include "Game/GUI/OrthoDepth.h"

//#include "Graphics/Camera.h"

#include "Game/GUI/GUIText.h"
#include "Game/GUI/GUIIcon.h"

#include "Renderer/RS/GUITexture.h"
#include "Renderer/RS/GUIText.h"

#include "Game/Entity/EntityBase.h"
#include "Game/Entity/Entity.h"

#include "Phage/Phage.h"

#include "Game/World.h"

class ObjectPanel
{
public:
	ObjectPanel();
	~ObjectPanel();

	int Init();
	int Generate(glm::vec2 screenRes, Phage::PhageUIEntity* phageUIEntity);
	int Draw(GLuint tex);
	int DrawEntityStats(World* world, Entity* entity);
	int ProcessMouse(Input* input);
private:
	float* vertices;
	GLuint* indices;
	float* texCoords;
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	GLuint TBO;

	glm::vec2 screenResolution;

	GUIText* guiText;
	GUIText* guiTextName;
	GUIText* guiTextSmall;

	GUIIcon* guiIconObject;
	GUIIcon* guiIconStats;
	GUIIcon* guiIconCollection;

	GLuint hackArmorTexture;
	GLuint pierceArmorTexture;
	GLuint magicArmorTexture;

	glm::vec2 topLeft;
	glm::vec2 botRight;
	glm::vec2 size;
	int textHeight;

	glm::vec2 collectionPoint1;
	glm::vec2 collectionPoint2;

	glm::vec2 statsPoint1;
	glm::vec2 statsPoint2;

	RS::GUITexture _rsGUITexture;
};
#endif
