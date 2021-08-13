#include "MinimapPanel.h"

#include "Renderer/Renderer.h"

MinimapPanel::MinimapPanel()
{
	vertices = NULL;
	indices = NULL;
	texCoords = NULL;
}

MinimapPanel::~MinimapPanel()
{
	delete[] vertices;
	delete[] indices;
	delete[] texCoords;
}

int MinimapPanel::Init()
{
	vertices = new float[12];

	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = OrthoDepth(ORTHO_MINIMAP_PANEL);

	vertices[3] = 1.0f;
	vertices[4] = 0.0f;
	vertices[5] = OrthoDepth(ORTHO_MINIMAP_PANEL);

	vertices[6] = 1.0f;
	vertices[7] = 1.0f;
	vertices[8] = OrthoDepth(ORTHO_MINIMAP_PANEL);

	vertices[9] = 0.0f;
	vertices[10] = 1.0f;
	vertices[11] = OrthoDepth(ORTHO_MINIMAP_PANEL);

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
	surface = IMG_Load("textures/ui/minimap_panel.png");
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

	return 0;
}

int MinimapPanel::Generate(glm::vec2 screenRes, Phage::PhageUIEntity* phageUIEntity)
{
	float sixteenth = (4.0f * screenRes.y / 3.0f) / 16.0f;
	glm::vec2 topLeft = glm::vec2(screenRes.x / 2.0f + 5.0f * sixteenth, screenRes.y - screenRes.y / 4.0f);
	glm::vec2 botRight = glm::vec2(screenRes.x / 2.0f + 8.0f * sixteenth, screenRes.y);

	//glm::vec2 topLeft = glm::vec2(screenRes.x / 2.0f + screenRes.x / 8.0f + 1.0f * (4.0f * screenRes.y / 3.0f) / 8.0f, screenRes.y - screenRes.y / 4.0f);
	//glm::vec2 botRight = glm::vec2(screenRes.x, screenRes.y);

	//glm::vec2 topLeft = glm::vec2(screenRes.x - 2.0f * (4.0f * screenRes.y / 3.0f) / 8.0f, screenRes.y - screenRes.y / 4.0f);
	//glm::vec2 botRight = glm::vec2(screenRes.x, screenRes.y);

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

	_rsGUITexture.SetPoints((int)topLeft.x, (int)topLeft.y, (int)botRight.x, (int)botRight.y);

	return 0;
}

int MinimapPanel::Draw(GLuint tex)
{
	//glBindVertexArray(VAO);
	
	//glBindTexture(GL_TEXTURE_2D, tex);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glBindVertexArray(0);

	_rsGUITexture.SetTexture(tex);
	RendererSingleton->AddGUITexture(_rsGUITexture);

	return 0;
}

int MinimapPanel::ProcessMouse(Input* input)
{
	if (input->GetMousePosition().x > topLeft.x && input->GetMousePosition().x < botRight.x &&
		input->GetMousePosition().y > topLeft.y && input->GetMousePosition().y < botRight.y)
		return 1;

	return 0;
}
