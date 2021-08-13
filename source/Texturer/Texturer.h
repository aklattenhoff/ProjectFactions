#ifndef _TEXTURER_H
#define _TEXTURER_H

#include <queue>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define TexturerSingleton Texturer::GetSingleton()

class Texturer
{
public:
	static const int VECTOR_MAIN = 0;
	static const int VECTOR_UI = 1;
public:
	struct Texture
	{
		std::string file;
		bool yflip;
		bool mipmap;
		GLuint id;
		int w, h;
		int lod;
		Texture()
		{
			yflip = false;
			mipmap = false;
		}
	};
public:
	Texturer();
	~Texturer();

	static int CreateSingleton();
	static Texturer* GetSingleton();
	static int DeleteSingleton();

	int InitTexturer();
	int LoadTexture(Texturer::Texture* texture, int vectorID = 0);
	int RunTextureWork(int vectorID = 0);
private:
	struct TextureWork
	{
		Texturer::Texture* texture;
		int tryNumber;
	};
private:
	static Texturer* _instance;

	bool _isInit;
	GLuint _baseTexture;
	GLuint _badTexture;

	std::queue<Texturer::TextureWork> _textureWorkVector;
	std::queue<Texturer::TextureWork> _uiTextureWorkVector;
};

#endif
