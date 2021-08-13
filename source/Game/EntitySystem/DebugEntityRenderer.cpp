#include "DebugEntityRenderer.h"

#include <string>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/EntitySystem/Entity.h"
#include "Game/EntitySystem/EntityBase.h"

DebugEntityRenderer::DebugEntityRenderer()
{
	_guiText = NULL;
}

DebugEntityRenderer::~DebugEntityRenderer()
{
	delete _guiText;
}

int DebugEntityRenderer::Init()
{
	_guiText = new GUIText();
	_guiText->Init("arialbd.ttf", 14);

	return 0;
}

int DebugEntityRenderer::RenderEntityStatuses(Camera* camera, EntityList* entityList)
{
	Entity* entity;

	camera->GetShader()->SetLighting(0);
	camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f);

	for (int i = 0;i < entityList->Size();i ++)
	{
		entity = entityList->Get(i);

		camera->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
		camera->GetShader()->UseProgram(SHADER_DEFAULT);
		camera->UpdateMatrix(SHADER_DEFAULT);

		//Render text
		glm::vec4 point2D = camera->Calculate2DPoint(entity->PosX(), entity->PosY(), entity->PosZ());
		std::string str = GetEntityStatusString(entity->GetEntityStatus());
		glm::vec2 strSize = _guiText->CalcStringSize(str);
		_guiText->Draw((int)(point2D.x-strSize.x/2.0f), (int)(point2D.y-20.0f), str, camera);
	}

	camera->GetShader()->SetLighting(1);
	camera->GetMVPMatrix()->SetProjectionMatrixType(PROJECTION);

	return 0;
}

const char* DebugEntityRenderer::GetEntityStatusString(int status)
{
	switch (status)
	{
		case Entity::STATUS_IDLING: return "IDLING"; break;
		case Entity::STATUS_WALKING: return "WALKING"; break;
		case Entity::STATUS_GATHERING: return "GATHERING"; break;
		case Entity::STATUS_ATTACKING: return "ATTACKING"; break;
		case Entity::STATUS_BUILDING: return "BUILDING"; break;
	}

	return "UNKOWN";
}
