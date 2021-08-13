#ifndef _DEBUG_ENTITY_RENDERER_H
#define _DEBUG_ENTITY_RENDERER_H

#include "Game/Entity/EntityList.h"

#include "Game/GUI/GUIText.h"

class DebugEntityRenderer
{
public:
	DebugEntityRenderer();
	~DebugEntityRenderer();

	int Init();

	int RenderEntityStatuses(EntityList* entityList);
private:
	const char* GetEntityStatusString(int status);
private:
	GUIText* _guiText;
};

#endif
