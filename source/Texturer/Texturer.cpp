#include "Texturer.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Texturer* Texturer::_instance = NULL;

Texturer::Texturer()
{
	_isInit = false;
}

Texturer::~Texturer()
{
}

int Texturer::CreateSingleton()
{
	_instance = new Texturer();

	return 0;
}

Texturer* Texturer::GetSingleton()
{
	return _instance;
}

int Texturer::DeleteSingleton()
{
	delete _instance;
	_instance = NULL;

	return 0;
}

int Texturer::InitTexturer()
{
	_isInit = true;

	IMG_Init(IMG_INIT_PNG);

	unsigned char* blank = new unsigned char[1*1*4];
	for (int i = 0;i < 1*1*4; i ++)
		blank[i] = 192;
	for (int i = 0;i < 1*1; i ++)
		blank[i*4+3] = 255;

	glGenTextures(1, &_baseTexture);
	glBindTexture(GL_TEXTURE_2D, _baseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, blank);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0;i < 1*1; i ++)
		blank[i*4+0] = 255;
	glGenTextures(1, &_badTexture);
	glBindTexture(GL_TEXTURE_2D, _badTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, blank);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] blank;

	return 0;
}

int Texturer::LoadTexture(Texturer::Texture* texture, int vectorID)
{
	if (!_isInit)
	{
		InitTexturer();
	}

	texture->id = _baseTexture;
	texture->w = 1;
	texture->h = 1;
	texture->lod = 1;

	if (vectorID == VECTOR_MAIN)
	{
		_textureWorkVector.push(Texturer::TextureWork());
		_textureWorkVector.back().texture = texture;
		_textureWorkVector.back().tryNumber = 0;
	}
	else if (vectorID == VECTOR_UI)
	{
		_uiTextureWorkVector.push(Texturer::TextureWork());
		_uiTextureWorkVector.back().texture = texture;
		_uiTextureWorkVector.back().tryNumber = 0;
	}

	return 0;
}

int Texturer::RunTextureWork(int vectorID)
{
	std::queue<Texturer::TextureWork>* textureWorkPointer = NULL;

	if (vectorID == VECTOR_MAIN) textureWorkPointer = &_textureWorkVector;
	else if (vectorID == VECTOR_UI) textureWorkPointer = &_uiTextureWorkVector;

	if (!textureWorkPointer)
		return -1;

	if (textureWorkPointer->size() == 0)
		return 1;

	textureWorkPointer->front().tryNumber ++;
	Texturer::Texture* texture = textureWorkPointer->front().texture;

	SDL_Surface* surface = NULL;
	surface = IMG_Load(texture->file.c_str());

	if (!surface)
	{
		if (textureWorkPointer->front().tryNumber > 4)
			texture->id = _badTexture;
		else
			textureWorkPointer->push(textureWorkPointer->front());
		textureWorkPointer->pop();
		return -1;
	}

	if (texture->yflip)
	{
		void* tempRow = (void*)malloc(surface->pitch);
		for (int i = 0;i < surface->h/2;i ++)
		{
			memcpy((Uint8*)tempRow, (Uint8*)(surface->pixels)+surface->pitch*i, surface->pitch);
			memcpy((Uint8*)(surface->pixels)+surface->pitch*i, (Uint8*)(surface->pixels)+surface->pitch*(surface->h - i-1), surface->pitch);
			memcpy((Uint8*)(surface->pixels)+surface->pitch*(surface->h - i-1), (Uint8*)tempRow, surface->pitch);
		}
	}

	GLenum textureFormat;
	if (surface->format->BytesPerPixel == 4)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
	else if (surface->format->BytesPerPixel == 3)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	if (texture->mipmap)
	{
		glTexStorage2D(GL_TEXTURE_2D, /*num_mipmaps*/5, GL_RGBA8, surface->w, surface->h);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, surface->w, surface->h, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	textureWorkPointer->pop();

	return 0;
}
