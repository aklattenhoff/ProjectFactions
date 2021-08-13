#include "GUICursor.h"

#include "Renderer/Renderer.h"

GUICursor::GUICursor()
{
	cursorVertices = new float[12];

	cursorVertices[0] = 0.0f;
	cursorVertices[1] = 0.0f;
	cursorVertices[2] = OrthoDepth(ORTHO_CURSOR);

	cursorVertices[3] = 32.0f;
	cursorVertices[4] = 0.0f;
	cursorVertices[5] = OrthoDepth(ORTHO_CURSOR);

	cursorVertices[6] = 32.0f;
	cursorVertices[7] = 32.0f;
	cursorVertices[8] = OrthoDepth(ORTHO_CURSOR);

	cursorVertices[9] = 0.0f;
	cursorVertices[10] = 32.0f;
	cursorVertices[11] = OrthoDepth(ORTHO_CURSOR);

	cursorIndices = new GLuint[6];

	cursorIndices[0] = 0;
	cursorIndices[1] = 2;
	cursorIndices[2] = 1;
	cursorIndices[3] = 0;
	cursorIndices[4] = 3;
	cursorIndices[5] = 2;

	cursorTexCoords = new float[8];

	cursorTexCoords[0] = 0.0f;
	cursorTexCoords[1] = 0.0f;

	cursorTexCoords[2] = 1.0f;
	cursorTexCoords[3] = 0.0f;

	cursorTexCoords[4] = 1.0f;
	cursorTexCoords[5] = 1.0f;

	cursorTexCoords[6] = 0.0f;
	cursorTexCoords[7] = 1.0f;

	glGenVertexArrays(1, &cursorVAO);
	glGenBuffers(1, &cursorVBO);
	glGenBuffers(1, &cursorIBO);

	glBindVertexArray(cursorVAO);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, cursorVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, cursorVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cursorIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*4, cursorIndices, GL_STATIC_DRAW);

	glGenBuffers(1, &cursorTBO);
    glBindBuffer(GL_ARRAY_BUFFER, cursorTBO);
    glBufferData(GL_ARRAY_BUFFER, 8*4, cursorTexCoords, GL_DYNAMIC_DRAW);
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
	surface = IMG_Load("art/cursor.png");
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

	_rsGUITexture.SetTexture(texture);
}

GUICursor::~GUICursor()
{
}

int GUICursor::Init()
{
	SDL_ShowCursor(0);

	return 0;
}

int GUICursor::Draw()
{
	//glBindVertexArray(cursorVAO);

	//glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int mX, mY;
	SDL_GetMouseState(&mX, &mY);

	//camera->GetShader()->UseProgram(SHADER_DEFAULT);
	//camera->GetMVPMatrix()->Push(MODEL);
	//camera->GetMVPMatrix()->translate(MODEL, (float)mX, (float)mY, 0.0f);
	//camera->UpdateMatrix(SHADER_DEFAULT);

	/*cursorVertices[0] = (float)mX;
	cursorVertices[1] = (float)mY;
	cursorVertices[3] = (float)mX+24;
	cursorVertices[4] = (float)mY;
	cursorVertices[6] = (float)mX+24;
	cursorVertices[7] = (float)mY+24;
	cursorVertices[9] = (float)mX;
	cursorVertices[10] = (float)mY+24;

	glBindBuffer(GL_ARRAY_BUFFER, cursorVBO);
	glBufferData(GL_ARRAY_BUFFER, 12*4, cursorVertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//camera->GetMVPMatrix()->Pop(MODEL);
	//camera->UpdateMatrix(SHADER_DEFAULT);

	//glDisable(GL_BLEND);

	//glBindVertexArray(0);

	_rsGUITexture.SetPoints(mX, mY, mX+32, mY+32, 0.999f);
	RendererSingleton->AddGUITexture(_rsGUITexture);

	return 0;
}
