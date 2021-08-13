#include "ActionPanel.h"

#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityBase.h"
#include "Parser/EntityXMLTag.h"

#include "Game/ActionManager/ActionManager.h"
#include "Game/ActionManager/ActionArray.h"
#include "Game/ActionManager/Action.h"
#include "Game/ActionManager/ActionList.h"

#include "Game/EntityManager/EntitySelectionList.h"

#include "Renderer/Renderer.h"
#include "Renderer/RS/GUITooltip.h"

#include "Game/World.h"
#include "Game/Entity/EntityBaseList.h"

ActionPanel::ActionPanel()
{
	vertices = NULL;
	indices = NULL;
	texCoords = NULL;

	guiIconAction = NULL;
}

ActionPanel::~ActionPanel()
{
	delete[] vertices;
	delete[] indices;
	delete[] texCoords;

	delete guiIconAction;
}

int ActionPanel::Init()
{
	vertices = new float[12];

	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = OrthoDepth(ORTHO_ACTION_PANEL);

	vertices[3] = 1.0f;
	vertices[4] = 0.0f;
	vertices[5] = OrthoDepth(ORTHO_ACTION_PANEL);

	vertices[6] = 1.0f;
	vertices[7] = 1.0f;
	vertices[8] = OrthoDepth(ORTHO_ACTION_PANEL);

	vertices[9] = 0.0f;
	vertices[10] = 1.0f;
	vertices[11] = OrthoDepth(ORTHO_ACTION_PANEL);

	indices = new GLuint[6];

	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;

	texCoords = new float[8];

	texCoords[0] = 0.0f;
	texCoords[1] = 0.0f;

	texCoords[2] = 1.0f;
	texCoords[3] = 0.0f;

	texCoords[4] = 1.0f;
	texCoords[5] = 1.0f;

	texCoords[6] = 0.0f;
	texCoords[7] = 1.0f;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8*4, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, 8*4, texCoords, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Texture
	IMG_Init(IMG_INIT_PNG);
	glGenTextures(1, &texture);
	SDL_Surface *surface;
	surface = IMG_Load("textures/ui/action_panel.png");
	int numberOfColors = surface->format->BytesPerPixel;
	GLenum textureFormat;
	if (numberOfColors == 4)
	{
		if (surface->format->Rmask == 0x000000ff)
		{textureFormat = GL_RGBA;}
		else
		{textureFormat = GL_BGRA;}
	}
	else if (numberOfColors == 3)
	{
		if (surface->format->Rmask == 0x000000ff)
		{textureFormat = GL_RGB;}
		else
		{textureFormat = GL_BGR;}
	}
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (surface)
		SDL_FreeSurface(surface);

	guiIconAction = new GUIIcon();
	guiIconAction->Init();

	return 0;
}

int ActionPanel::Generate(glm::vec2 screenRes, Phage::PhageUIEntity* phageUIEntity)
{
	float sixteenth = (4.0f * screenRes.y / 3.0f) / 16.0f;
	topLeft = glm::vec2((screenRes.x / 2.0f) - 8.0f * sixteenth, screenRes.y - screenRes.y / 5.0f);
	botRight = glm::vec2((screenRes.x / 2.0f) - 2.0f * sixteenth, screenRes.y);

	//glm::vec2 topLeft = glm::vec2(0.0f, screenRes.y - screenRes.y / 5.5f);
	//glm::vec2 botRight = glm::vec2(screenRes.x / 2.0f - 1.0f * (4.0f * screenRes.y / 3.0f) / 8.0f, screenRes.y);

	//glm::vec2 topLeft = glm::vec2(0.0f, screenRes.y - screenRes.y / 5.5f);
	//glm::vec2 botRight = glm::vec2(3.0f * (4.0f * screenRes.y / 3.0f) / 8.0f, screenRes.y);

	vertices[0] = topLeft.x;
	vertices[1] = topLeft.y;

	vertices[3] = botRight.x;
	vertices[4] = topLeft.y;

	vertices[6] = botRight.x;
	vertices[7] = botRight.y;

	vertices[9] = topLeft.x;
	vertices[10] = botRight.y;

	phageUIEntity->SetTranslate(topLeft.x, topLeft.y);
	phageUIEntity->SetScale(botRight.x - topLeft.x, botRight.y - topLeft.y);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	guiIconAction->Generate(glm::vec2((botRight.y-topLeft.y) / 3.375f));

	_rsGUITexture.SetPoints((int)topLeft.x, (int)topLeft.y, (int)botRight.x, (int)botRight.y, 0.01f);

	return 0;
}

int ActionPanel::Draw(GLuint tex)
{
	//glBindVertexArray(VAO);
	
	//glBindTexture(GL_TEXTURE_2D, tex);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glBindVertexArray(0);

	_rsGUITexture.SetTexture(tex);
	RendererSingleton->AddGUITexture(_rsGUITexture);

	return 0;
}

int ActionPanel::DrawEntityActions(World* world, Entity* entity)
{
	glm::vec2 statsPoint1 = topLeft + glm::vec2((botRight.x-topLeft.x)/(19.28571*2.0f),(botRight.y-topLeft.y)/18.0f);
	glm::vec2 statsPoint2 = botRight - glm::vec2((botRight.x-topLeft.x)/(19.28571*2.0f),(botRight.y-topLeft.y)/18.0f);

	std::vector<EntityXMLTag::Action> actionVector = entity->GetEntityBase()->ActionVector();

	for (unsigned int i = 0;i < actionVector.size();i ++)
	{
		int index = actionVector[i].index;
		glm::vec2 position;
		position.x = (float)(index % 8) * guiIconAction->GetSize().x;
		position.y = (float)(index / 8) * guiIconAction->GetSize().y;

		position += statsPoint1;

		//guiIconAction->DrawAtLocation(camera, position.x, position.y, world->GetActionList()->At(actionVector[i].actionName)->getActionIconTexture());
		glm::vec2 actSize = glm::vec2((botRight.y-topLeft.y) / 3.375f);
		RS::GUITexture rsTexture;
		rsTexture.SetPoints((int)position.x, (int)position.y, (int)(position.x+actSize.x), (int)(position.y+actSize.y), 0.02f);
		rsTexture.SetTexture(world->GetActionList()->At(actionVector[i].actionName)->getActionIconTexture());
		RendererSingleton->AddGUITexture(rsTexture);
	}

	return 0;
}

int ActionPanel::ProcessMouse(World* world, Input* input, ActionManagerInterface* actionManagerInterface)
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
					else if (action->getActionType() == ACTION_TYPE_TRAIN)
					{
						EntityBase* entBase = world->GetEntityBaseList()->At(action->gettrainObjectName());
						firstString = "Train ";
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

					rsToolTip.SetX((int)input->GetMousePosition().x);
					rsToolTip.SetY((int)input->GetMousePosition().y - (2+2/*num line plus 2*/)*rsToolTip.GetHeight());

					RendererSingleton->AddGUIToolTip(rsToolTip);
				}
			}
		}
	}

	return returnCode;
}
