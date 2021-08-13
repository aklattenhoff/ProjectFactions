#include "GUIBottomBar.h"

#include "Game/Entity/Entity.h"
#include "Renderer/Renderer.h"

GUIBottomBar::GUIBottomBar()
{
	objectPanel = NULL;
	actionPanel = NULL;
	resourcePanel = NULL;
	minimapPanel = NULL;
	uiProgressQueue = NULL;

	guiIcon = NULL;
}

GUIBottomBar::~GUIBottomBar()
{
	delete objectPanel;
	delete actionPanel;
	delete resourcePanel;
	delete minimapPanel;
	delete uiProgressQueue;

	delete guiIcon;
}

int GUIBottomBar::Init()
{
	objectPanel = new ObjectPanel();
	objectPanel->Init();

	actionPanel = new ActionPanel();
	actionPanel->Init();

	resourcePanel = new ResourcePanel();
	resourcePanel->Init();

	minimapPanel = new MinimapPanel();
	minimapPanel->Init();

	uiProgressQueue = new UIProgressQueue();
	uiProgressQueue->Init();

	guiIcon = new GUIIcon();
	guiIcon->Init();

	return 0;
}

int GUIBottomBar::GenerateBottomBar(glm::vec2 screenRes)
{
	objectPanel->Generate(screenRes, _phageObjectPanel);
	actionPanel->Generate(screenRes, _phageActionPanel);
	resourcePanel->Generate(screenRes, _phageResourcePanel);
	minimapPanel->Generate(screenRes, _phageMinimapPanel);
	uiProgressQueue->Generate(screenRes, _phageUIProgressQueue);

	guiIcon->Generate(screenRes);

	return 0;
}

int GUIBottomBar::Draw()
{
	//camera->GetShader()->UseProgram(SHADER_DEFAULT);
	//camera->UpdateMatrix(SHADER_DEFAULT);

	//objectPanel->Draw(objectPanelInfo.texture);
	//actionPanel->Draw(actionPanelInfo.texture);
	//resourcePanel->Draw(resourcePanelInfo.texture);
	//minimapPanel->Draw(minimapPanelInfo.texture);

	RendererSingleton->AddPhageUIEntity(_phageObjectPanel);
	RendererSingleton->AddPhageUIEntity(_phageActionPanel);
	RendererSingleton->AddPhageUIEntity(_phageResourcePanel);
	RendererSingleton->AddPhageUIEntity(_phageMinimapPanel);

	//camera->GetShader()->UseProgram(SHADER_NO_TEXTURE);
	//camera->UpdateMatrix(SHADER_NO_TEXTURE);

	return 0;
}

int GUIBottomBar::ProcessMouse(World* world, Input* input, ActionManagerInterface* actionManagerInterface)
{
	int returnCode = 0;

	if (objectPanel->ProcessMouse(input))
		returnCode = 1;
	if (actionPanel->ProcessMouse(world, input, actionManagerInterface))
		returnCode = 1;
	if (resourcePanel->ProcessMouse(input))
		returnCode = 1;
	if (minimapPanel->ProcessMouse(input))
		returnCode = 1;

	return returnCode;
}

int GUIBottomBar::DrawEntityAction(World* world, Entity* entity)
{
	actionPanel->DrawEntityActions(world, entity);

	return 0;
}

int GUIBottomBar::DrawEntityStats(World* world, Entity* entity)
{
	objectPanel->DrawEntityStats(world, entity);

	return 0;
}

int GUIBottomBar::DrawUIProgressQueue(World* world)
{
	uiProgressQueue->DrawUIProgressQueue(world);

	return 0;
}

int GUIBottomBar::DrawResources(World* world)
{
	resourcePanel->DrawResources(world);

	return 0;
}
