#ifndef _DEBUG_ENTITY_RENDERER_H
#define _DEBUG_ENTITY_RENDERER_H

#include "Game/Graphics/Camera.h"
#include "Game/EntitySystem/EntityList.h"

#include "Game/GUI/GUIText.h"

class DebugEntityRenderer
{
public:
	DebugEntityRenderer();
	~DebugEntityRenderer();

	int Init();

	int RenderEntityStatuses(Camera* camera, EntityList* entityList);
private:
	const char* GetEntityStatusString(int status);
private:
	GUIText* _guiText;
};

#endif
