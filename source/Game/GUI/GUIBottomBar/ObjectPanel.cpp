#include "ObjectPanel.h"

#include "Renderer/Renderer.h"

ObjectPanel::ObjectPanel()
{
	vertices = NULL;
	indices = NULL;
	texCoords = NULL;

	guiText = NULL;
	guiTextName = NULL;
	guiTextSmall = NULL;

	guiIconObject = NULL;
	guiIconStats = NULL;
	guiIconCollection = NULL;
}

ObjectPanel::~ObjectPanel()
{
	delete[] vertices;
	delete[] indices;
	delete[] texCoords;

	delete guiText;
	delete guiTextName;
	delete guiTextSmall;

	delete guiIconObject;
	delete guiIconStats;
	delete guiIconCollection;
}

int ObjectPanel::Init()
{
	vertices = new float[12];

	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = OrthoDepth(ORTHO_OBJECT_PANEL);

	vertices[3] = 1.0f;
	vertices[4] = 0.0f;
	vertices[5] = OrthoDepth(ORTHO_OBJECT_PANEL);

	vertices[6] = 1.0f;
	vertices[7] = 1.0f;
	vertices[8] = OrthoDepth(ORTHO_OBJECT_PANEL);

	vertices[9] = 0.0f;
	vertices[10] = 1.0f;
	vertices[11] = OrthoDepth(ORTHO_OBJECT_PANEL);

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

	guiText = new GUIText();
	guiText->Init("arialbd.ttf", 12);
	guiTextName = new GUIText();
	guiTextName->Init("arialbd.ttf", 14);
	guiTextSmall = new GUIText();
	guiTextSmall->Init("arialbd.ttf", 10);

	guiIconObject = new GUIIcon();
	guiIconObject->Init();

	guiIconStats = new GUIIcon();
	guiIconStats->Init();

	guiIconCollection = new GUIIcon();
	guiIconCollection->Init();

	hackArmorTexture = TextureLoader_LoadFromFile("textures/ui/hack_armor_icon.png");
	pierceArmorTexture = TextureLoader_LoadFromFile("textures/ui/pierce_armor_icon.png");
	magicArmorTexture = TextureLoader_LoadFromFile("textures/ui/magic_armor_icon.png");

	return 0;
}

int ObjectPanel::Generate(glm::vec2 screenRes, Phage::PhageUIEntity* phageUIEntity)
{
	screenResolution = screenRes;

	float sixteenth = (4.0f * screenRes.y / 3.0f) / 16.0f;
	topLeft = glm::vec2(screenRes.x / 2.0f - 2.0f * sixteenth, screenRes.y - screenRes.y / 5.0f);
	botRight = glm::vec2(screenRes.x / 2.0f + 3.0f * sixteenth, screenRes.y);
	size = botRight - topLeft;
	textHeight = (int)((3.0f / 4.0f) * ((24.0f / 216.0f) * size.y));
	if (textHeight < 8) textHeight = 8;

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

	guiText->Destroy();
	guiText->Init("arialbd.ttf", (int)(screenRes.y / 64.0f));

	guiTextName->Destroy();
	guiTextName->Init("arialbd.ttf", (int)(screenRes.y / 48.0f));

	guiTextSmall->Destroy();
	guiTextSmall->Init("arialbd.ttf", (int)(screenRes.y / 90.0f));

	guiIconObject->Generate(glm::vec2((botRight.y-topLeft.y) / 2.25f));

	guiIconStats->Generate(glm::vec2(screenRes.y / 33.75f, screenRes.y / 33.75f));

	guiIconCollection->Generate(glm::vec2((botRight.y-topLeft.y) / 6.0f));

	statsPoint1 = topLeft + glm::vec2((botRight.x-topLeft.x)/(16.07143f*2.0f),(botRight.y-topLeft.y)/18.0f);
	statsPoint2 = statsPoint1 + glm::vec2((botRight.x-topLeft.x)/1.71756f,(botRight.y-topLeft.y)/1.12500f);

	collectionPoint1 = topLeft + glm::vec2((botRight.x-topLeft.x)/(1.54110f),(botRight.y-topLeft.y)/18.0f);
	collectionPoint2 = collectionPoint1 + glm::vec2((botRight.x-topLeft.x)/3.12500f,(botRight.y-topLeft.y)/1.12500f);

	_rsGUITexture.SetPoints((int)topLeft.x, (int)topLeft.y, (int)botRight.x, (int)botRight.y);

	return 0;
}

int ObjectPanel::Draw(GLuint tex)
{
	//glBindVertexArray(VAO);
	
	//glBindTexture(GL_TEXTURE_2D, tex);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glBindVertexArray(0);

	_rsGUITexture.SetTexture(tex);
	RendererSingleton->AddGUITexture(_rsGUITexture);

	return 0;
}

int ObjectPanel::DrawEntityStats(World* world, Entity* entity)
{
	//TODO: Change all of these hard coded values. In every UI of the bottom bar!
	//NOTE: They are all based on the 1080p version of the ui resources

	//Draw name
	int leftPoint = (int)(topLeft.x + 15.0f*((botRight.x-topLeft.x)/450.0f) + 4.0f);
	int topPoint = (int)(topLeft.y + 13.0f*((botRight.y-topLeft.y)/216.0f) + 1.0f);
	RS::GUIText rsName;
	rsName.SetText(leftPoint, topPoint, entity->GetEntityBase()->GetEntityXML()->name, textHeight);
	RendererSingleton->AddGUIText(rsName);

	return 0;
}

int ObjectPanel::ProcessMouse(Input* input)
{
	int returnCode = 0;

	if (input->GetMousePosition().x > topLeft.x && input->GetMousePosition().x < botRight.x &&
		input->GetMousePosition().y > topLeft.y && input->GetMousePosition().y < botRight.y)
	{
		returnCode = 1;

		//Check if clicking in collection
		if (input->GetMousePosition().x > collectionPoint1.x && input->GetMousePosition().x < collectionPoint2.x &&
			input->GetMousePosition().y > collectionPoint1.y && input->GetMousePosition().y < collectionPoint2.y)
		{
			if (input->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON))
			{
				int index = (int)(4.0f * (input->GetMousePosition().x - collectionPoint1.x) / (collectionPoint2.x - collectionPoint1.x)) +
							4 * (int)(5.0f * (input->GetMousePosition().y - collectionPoint1.y) / (collectionPoint2.y - collectionPoint1.y));
			}
		}
	}

	return returnCode;
}
