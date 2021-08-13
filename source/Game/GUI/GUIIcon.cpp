#include "GUIIcon.h"

GUIIcon::GUIIcon()
{
	vertices = NULL;
	indices = NULL;
	texCoords = NULL;
}

GUIIcon::~GUIIcon()
{
	delete[] vertices;
	delete[] indices;
	delete[] texCoords;
}

int GUIIcon::Init()
{
	vertices = new float[12];

	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = 0.0f;

	vertices[3] = 1.0f;
	vertices[4] = 0.0f;
	vertices[5] = 0.0f;

	vertices[6] = 1.0f;
	vertices[7] = 1.0f;
	vertices[8] = 0.0f;

	vertices[9] = 0.0f;
	vertices[10] = 1.0f;
	vertices[11] = 0.0f;

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

	return 0;
}

int GUIIcon::Init(std::string fileName)
{
	vertices = new float[12];

	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = 0.0f;

	vertices[3] = 1.0f;
	vertices[4] = 0.0f;
	vertices[5] = 0.0f;

	vertices[6] = 1.0f;
	vertices[7] = 1.0f;
	vertices[8] = 0.0f;

	vertices[9] = 0.0f;
	vertices[10] = 1.0f;
	vertices[11] = 0.0f;

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
	surface = IMG_Load(fileName.c_str());
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

int GUIIcon::Generate(glm::vec2 size)
{
	//float eighth = (4.0f * screenRes.y / 3.0f) / 8.0f;
	//glm::vec2 topLeft = glm::vec2(screenRes.x / 2.0f - eighth, screenRes.y - screenRes.y / 5.5f + (screenRes.y / 5.5f) / 5.0f);
	//glm::vec2 botRight = glm::vec2(screenRes.x / 2.0f - eighth + (2.0f * eighth) / 4.0f, screenRes.y);

	glm::vec2 topLeft = glm::vec2(0.0f, 0.0f);
	glm::vec2 botRight = size;

	vertices[0] = topLeft.x;
	vertices[1] = topLeft.y;

	vertices[3] = botRight.x;
	vertices[4] = topLeft.y;

	vertices[6] = botRight.x;
	vertices[7] = botRight.y;

	vertices[9] = topLeft.x;
	vertices[10] = botRight.y;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

int GUIIcon::Draw()
{
	glBindVertexArray(VAO);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	return 0;
}

int GUIIcon::Draw(GLuint tex)
{
	glBindVertexArray(VAO);
	
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	return 0;
}

/*int GUIIcon::DrawAtLocation(Camera* camera, float x, float y, GLuint tex, float depth)
{
	camera->GetShader()->UseProgram(SHADER_DEFAULT);
	camera->GetMVPMatrix()->Push(MODEL);
	camera->GetMVPMatrix()->translate(MODEL, x, y, 0.0f);
	camera->UpdateMatrix(SHADER_DEFAULT);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDisable(GL_BLEND);

	camera->GetMVPMatrix()->Pop(MODEL);
	camera->UpdateMatrix(SHADER_DEFAULT);

	return 0;
}*/

int GUIIcon::DrawAtLocation()
{
	return 0;
}

glm::vec2 GUIIcon::GetSize()
{
	return glm::vec2(vertices[6], vertices[7]);
}
