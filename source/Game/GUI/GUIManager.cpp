#include "GUIManager.h"

#include "Game/EntityManager/EntitySelectionList.h"

#include "Renderer/Renderer.h"

GUIManager::GUIManager()
{
	guiUnitSelectionBox = NULL;
	guiText = NULL;
	guiCursor = NULL;
	guiBottomBar = NULL;
	guiBottomBarParser = NULL;

	mouseOverGUI = 0;
}

GUIManager::~GUIManager()
{
	delete guiUnitSelectionBox;
	delete guiText;
	delete guiCursor;
	delete guiBottomBar;
	delete guiBottomBarParser;
}

int GUIManager::Init()
{
	guiUnitSelectionBox = new GUIUnitSelectionBox();
	guiText = new GUIText();
	guiText->Init("arialbd.ttf", 14);
	guiCursor = new GUICursor();
	guiCursor->Init();

	fps = 0.0f;

	guiBottomBar = new GUIBottomBar();
	guiBottomBar->Init();

	guiBottomBarParser = new GUIBottomBarParser();

	return 0;
}

int GUIManager::PreRenderGUI(glm::vec2 screenRes)
{
	guiBottomBar->GenerateBottomBar(screenRes);

	return 0;
}

int GUIManager::Draw(World* world, FactionList* factionList, Camera* camera, bool selecting)
{
	//camera->GetShader()->UseProgram(SHADER_NO_TEXTURE);

	//camera->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
	//camera->UpdateMatrix(SHADER_NO_TEXTURE);

	//camera->GetShader()->SetLighting(0);

	if (selecting)
	{
		//camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f);
		//guiUnitSelectionBox->Draw();
		RendererSingleton->AddGUILineRect(_rsLineRect);
	}

	//Draw bottom bar
	//camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f);
	//guiBottomBar->Draw(camera);

	//if (world->GetEntitySelectionList()->Size() > 0)
		//guiBottomBar->DrawEntityAction(world, camera, world->GetEntitySelectionList()->At(0));

	if (world->GetEntitySelectionList()->Size() > 0)
	{
		//camera->GetShader()->UseProgram(SHADER_DEFAULT);
		//camera->UpdateMatrix(SHADER_DEFAULT);
		//guiBottomBar->DrawEntityStats(camera, world->GetEntitySelectionList()->At(0));
		//guiBottomBar->DrawObjectAction(camera, objectSelectionArray->GetCurrentCollection().objectInstance);
		//guiBottomBar->DrawObjectCollection(camera, objectSelectionArray);
		//camera->GetShader()->UseProgram(SHADER_NO_TEXTURE);
		//camera->UpdateMatrix(SHADER_NO_TEXTURE);
	}

	//camera->GetShader()->UseProgram(SHADER_DEFAULT);
	//camera->UpdateMatrix(SHADER_DEFAULT);

	//TODO:This should be changed so that it doesn't create the string every time
	std::stringstream ss (std::stringstream::in | std::stringstream::out);
	ss << "FPS: " << round(fps);
	//guiText->Draw(10, 10, ss.str().c_str(), camera);
	_rsText.SetText(10, 10, ss.str().c_str());
	RendererSingleton->AddGUIText(_rsText);

	guiCursor->Draw();

	//camera->GetShader()->SetLighting(1);

	//camera->GetMVPMatrix()->SetProjectionMatrixType(PROJECTION);

	return 0;
}

int GUIManager::Draw(World* world, bool selecting)
{
	if (selecting)
	{
		RendererSingleton->AddGUILineRect(_rsLineRect);
	}

	guiBottomBar->Draw();
	guiBottomBar->DrawResources(world);

	if (world->GetEntitySelectionList()->Size() > 0)
		guiBottomBar->DrawEntityAction(world, world->GetEntitySelectionList()->At(0));

	if (world->GetEntitySelectionList()->Size() > 0)
	{
		guiBottomBar->DrawEntityStats(world, world->GetEntitySelectionList()->At(0));
	}

	guiBottomBar->DrawUIProgressQueue(world);

	std::stringstream ss (std::stringstream::in | std::stringstream::out);
	ss << "FPS: " << round(fps);
	_rsText.SetText(10, 10, ss.str().c_str(), 16);
	RendererSingleton->AddGUIText(_rsText);

	return 0;
}

int GUIManager::ProcessMouse(World* world)
{
	if (guiBottomBar->ProcessMouse(world, world->GetInput(), world->GetActionManagerInterface()))
		mouseOverGUI = 1;
	else
		mouseOverGUI = 0;

	return mouseOverGUI;
}

int GUIManager::SetBottomBarFile(const char* bottomBarFile)
{
	guiBottomBarParser->Parse(bottomBarFile, guiBottomBar);

	return 0;
}

int GUIManager::SetUnitSelectionBoxPoints(glm::vec4 points)
{
	guiUnitSelectionBox->SetPoints(points);
	_rsLineRect.SetPoints(points);

	return 0;
}

int GUIManager::SetFPS(float framesPerSec)
{
	fps = framesPerSec;

	return 0;
}

bool GUIManager::IsMouseOverGUI()
{
	return (mouseOverGUI == 1);
}
