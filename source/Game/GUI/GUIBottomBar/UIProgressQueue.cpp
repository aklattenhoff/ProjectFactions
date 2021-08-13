#include "UIProgressQueue.h"

#include <sstream>

#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityBase.h"
#include "Parser/EntityXMLTag.h"

#include "Game/ActionManager/ActionManager.h"
#include "Game/ActionManager/ActionArray.h"
#include "Game/ActionManager/Action.h"
#include "Game/ActionManager/ActionList.h"
#include "Game/ActionManager/ActionProgressQueue.h"

#include "Game/EntityManager/EntitySelectionList.h"
#include "Game/Entity/EntityProcessEvent.h"

#include "Renderer/Renderer.h"
#include "Renderer/RS/GUITooltip.h"
#include "Renderer/RS/GUIText.h"

#include "Game/World.h"
#include "Game/Entity/EntityBaseList.h"

UIProgressQueue::UIProgressQueue()
{
	guiIconAction = NULL;
}

UIProgressQueue::~UIProgressQueue()
{
	delete guiIconAction;
}

int UIProgressQueue::Init()
{
	guiIconAction = new GUIIcon();
	guiIconAction->Init();

	SDL_Surface *surface = NULL;
	surface = IMG_Load("textures/ui/action_progress_queue_timer.png");
	if (!surface) return -1;

	GLenum textureFormat;
	if (surface->format->BytesPerPixel == 4)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
	else if (surface->format->BytesPerPixel == 3)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (surface)
		SDL_FreeSurface(surface);

	return 0;
}

int UIProgressQueue::Generate(glm::vec2 screenRes, Phage::PhageUIEntity* phageUIEntity)
{
	float sixteenth = (4.0f * screenRes.y / 3.0f) / 16.0f;
	topLeft = glm::vec2((screenRes.x / 2.0f) - 2.0f * sixteenth, screenRes.y - screenRes.y / 5.0f - screenRes.y / 15.0f);
	botRight = glm::vec2((screenRes.x / 2.0f) + 5.0f * sixteenth, screenRes.y - screenRes.y / 5.0f - screenRes.y / 100.0f);

	guiIconAction->Generate(glm::vec2((botRight.y-topLeft.y) / 3.375f));

	_rsGUITexture.SetPoints((int)topLeft.x, (int)topLeft.y, (int)botRight.x, (int)botRight.y, 0.01f);

	return 0;
}

int UIProgressQueue::DrawUIProgressQueue(World* world)
{
	glm::vec2 statsPoint1 = topLeft;
	glm::vec2 statsPoint2 = botRight - glm::vec2((botRight.x-topLeft.x)/(19.28571*2.0f),(botRight.y-topLeft.y)/18.0f);

	ActionProgressQueue* actionQueue = world->GetActionProgressQueue();
	EntityProcessEvent* entProcEvnt = NULL;
	unsigned int texID;
	int count;

	for (int i = 0;i < actionQueue->Size();i ++)
	{
		actionQueue->GetActionProgress(i, &entProcEvnt, &texID, &count);

		float progress = 0.0f;
		if (entProcEvnt)
			progress = entProcEvnt->trainPointsRemaining / entProcEvnt->trainPoints;
		if (progress <= 0.0f) progress = 1.0f;

		int index = i;
		glm::vec2 position;

		position = statsPoint1;
		position.x += (botRight.y - topLeft.y)*((float)index);

		//guiIconAction->DrawAtLocation(camera, position.x, position.y, world->GetActionList()->At(actionVector[i].actionName)->getActionIconTexture());
		glm::vec2 actSize = glm::vec2((botRight.y - topLeft.y));
		RS::GUITexture rsTexture;
		rsTexture.SetPoints((int)position.x, (int)position.y, (int)(position.x+actSize.x), (int)(position.y+actSize.y), 0.02f);
		rsTexture.SetTexture(texID);
		RendererSingleton->AddGUITexture(rsTexture);

		rsTexture.SetPoints((int)position.x, (int)(position.y+(1.0f - progress)*actSize.y), (int)(position.x+actSize.x), (int)(position.y+actSize.y), 0.03f);
		rsTexture.SetTexture(_textureID);
		RendererSingleton->AddGUITexture(rsTexture);

		if (count > 1)
		{
			RS::GUIText rsText;
			std::stringstream ss (std::stringstream::in | std::stringstream::out);
			ss << "" << count;
			rsText.SetText((int)(position.x+(actSize.y/10.0f)), (int)(position.y+(2.0f*actSize.x/3.0f)), ss.str(), 14);
			RendererSingleton->AddGUIText(rsText);
		}
	}

	return 0;
}

int UIProgressQueue::ProcessMouse(World* world, Input* input, ActionManagerInterface* actionManagerInterface)
{
	int returnCode = 0;

	if (input->GetMousePosition().x > topLeft.x && input->GetMousePosition().x < botRight.x &&
		input->GetMousePosition().y > topLeft.y && input->GetMousePosition().y < botRight.y)
	{
		returnCode = 1;

		if (input->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON))
		{
			glm::vec2 statsPoint1 = topLeft + glm::vec2((botRight.x-topLeft.x)/(19.28571*2.0f),(botRight.y-topLeft.y)/18.0f);
			glm::vec2 statsPoint2 = botRight - glm::vec2((botRight.x-topLeft.x)/(19.28571*2.0f),(botRight.y-topLeft.y)/18.0f);

			if (input->GetMousePosition().x > statsPoint1.x && input->GetMousePosition().x < statsPoint2.x &&
				input->GetMousePosition().y > statsPoint1.y && input->GetMousePosition().y < statsPoint2.y)
			{
				int index = (int)(8.0f * (input->GetMousePosition().x - statsPoint1.x) / (statsPoint2.x - statsPoint1.x)) +
							8 * (int)(3.0f * (input->GetMousePosition().y - statsPoint1.y) / (statsPoint2.y - statsPoint1.y));

				DebugOutput("Clicked on action panel action index: %i\n", index);

				int actionIndex = -1;
				/*if (objectSelectionArray->Size() > 0)
					for (unsigned int i = 0;i < objectSelectionArray->GetCurrentCollection().objectInstance->GetActionTagVectorPointer()->size();i ++)
					{
						if (objectSelectionArray->GetCurrentCollection().objectInstance->GetActionTagVectorPointer()->at(i).index == index)
							actionIndex = i;
					}

				if (actionIndex != -1)
				{
					actionManagerInterface->SetActionFromActionPanel(objectSelectionArray->GetCurrentCollection().objectInstance->GetActionTagVectorPointer()->at(actionIndex).action);
					DebugOutput("Added action to ActionManager: %s\n", objectSelectionArray->GetCurrentCollection().objectInstance->GetActionTagVectorPointer()->at(actionIndex).actionName.c_str());
				}*/

				if (world->GetEntitySelectionList()->Size() > 0)
					for (unsigned int i = 0;i < world->GetEntitySelectionList()->At(0)->GetEntityBase()->ActionVector().size();i ++)
					{
						if (world->GetEntitySelectionList()->At(0)->GetEntityBase()->ActionVector().at(i).index == index)
							actionIndex = i;
					}

				if (actionIndex != -1)
				{
					Action* action = world->GetActionList()->At(world->GetEntitySelectionList()->At(0)->GetEntityBase()->ActionVector().at(actionIndex).actionName);
					//Action* action = world->GetFactionList()->GetFaction(0)->actionArray->Get(world->GetEntitySelectionList()->At(0)->GetEntityBase()->ActionVector().at(actionIndex).actionName.c_str());
					actionManagerInterface->SetActionFromActionPanel(action);
					DebugOutput("Added action to ActionManager: %s\n", world->GetEntitySelectionList()->At(0)->GetEntityBase()->ActionVector().at(actionIndex).actionName.c_str());
				}
			}
		}
		else
		{
			//Draw the tooltip for the given action
			glm::vec2 statsPoint1 = topLeft + glm::vec2((botRight.x-topLeft.x)/(19.28571*2.0f),(botRight.y-topLeft.y)/18.0f);
			glm::vec2 statsPoint2 = botRight - glm::vec2((botRight.x-topLeft.x)/(19.28571*2.0f),(botRight.y-topLeft.y)/18.0f);

			if (input->GetMousePosition().x > statsPoint1.x && input->GetMousePosition().x < statsPoint2.x &&
				input->GetMousePosition().y > statsPoint1.y && input->GetMousePosition().y < statsPoint2.y)
			{
				int index = (int)(8.0f * (input->GetMousePosition().x - statsPoint1.x) / (statsPoint2.x - statsPoint1.x)) +
							8 * (int)(3.0f * (input->GetMousePosition().y - statsPoint1.y) / (statsPoint2.y - statsPoint1.y));

				//DebugOutput("Hovered on action panel action index: %i\n", index);

				int actionIndex = -1;

				if (world->GetEntitySelectionList()->Size() > 0)
					for (unsigned int i = 0;i < world->GetEntitySelectionList()->At(0)->GetEntityBase()->ActionVector().size();i ++)
					{
						if (world->GetEntitySelectionList()->At(0)->GetEntityBase()->ActionVector().at(i).index == index)
							actionIndex = i;
					}

				if (actionIndex != -1)
				{
					Action* action = world->GetActionList()->At(world->GetEntitySelectionList()->At(0)->GetEntityBase()->ActionVector().at(actionIndex).actionName);

					RS::GUIToolTip rsToolTip;
					rsToolTip.SetToolTip((int)input->GetMousePosition().x+32, (int)input->GetMousePosition().y, world->GetEntitySelectionList()->At(0)->GetEntityBase()->ActionVector().at(actionIndex).actionName);

					std::string firstString;

					if (action->getActionType() == ACTION_TYPE_BUILD)
					{
						EntityBase* entBase = world->GetEntityBaseList()->At(action->getbuildObjectName());
						firstString = "Build ";
						firstString.append(entBase->GetEntityXML()->name);
						rsToolTip.SetString(0, firstString);
						firstString = "Cost: ";
						for (unsigned int i = 0;i < entBase->GetEntityXML()->costVector.size();i ++)
						{
							if (i > 0) firstString.append(", ");
							firstString.append(std::to_string((int)entBase->GetEntityXML()->costVector[i].cost));
							if (entBase->GetEntityXML()->costVector[i].type.compare("food") == 0) firstString.append("^f");
							else if (entBase->GetEntityXML()->costVector[i].type.compare("wood") == 0) firstString.append("^w");
						}
						rsToolTip.SetString(1, firstString);
					}

					RendererSingleton->AddGUIToolTip(rsToolTip);
				}
			}
		}
	}

	return returnCode;
}
