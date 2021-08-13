#include "TextureLoader.h"

#include "loadGLE.h"

GLuint TextureLoader_LoadFromFile(const char* fileName, int interp)
{
	IMG_Init(IMG_INIT_PNG);
	GLuint texture;
	glGenTextures(1, &texture);
	SDL_Surface *surface;
	surface = IMG_Load(fileName);
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

	if (interp == TL_NEAREST)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (interp == TL_LINEAR)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (interp == TL_MIPMAP)
	{
		glTexStorage2D(GL_TEXTURE_2D, /*num_mipmaps*/5, GL_RGBA8, surface->w, surface->h);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, surface->w, surface->h, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	if (interp != TL_MIPMAP)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	if (surface)
		SDL_FreeSurface(surface);

	return texture;
}

GLenum TextureLoader_GetTextureFormat(SDL_Surface* surface)
{
	if (surface->format->BytesPerPixel == 4)
	{
		if (surface->format->Rmask == 0x000000ff)
			return GL_RGBA;
		else
			return GL_BGRA;
	}
	else if (surface->format->BytesPerPixel == 3)
	{
		if (surface->format->Rmask == 0x000000ff)
			return GL_RGB;
		else
			return GL_BGR;
	}

	return 0;
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;
	case 2:
		return *(Uint16 *)p;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;
	case 4:
		return *(Uint32 *)p;
		break;
	default:
		return 0;
	}
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *)p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}

SDL_Surface* TextureLoader_PreprocessRotation(SDL_Surface* surface, int rotation)
{
	//Short circuit if there is no rotation needed and simply return the passed surface pointer
	if (rotation == 0)
		return surface;

	SDL_Surface* rotatedSurface;
	rotatedSurface = SDL_CreateRGBSurface(0,
		rotation == 2 ? surface->w : surface->h,
		rotation == 2 ? surface->h : surface->w,
		surface->format->BitsPerPixel,
		surface->format->Rmask,
		surface->format->Gmask,
		surface->format->Bmask,
		surface->format->Amask);

	if (rotation == 1)
	{
		for (int i = 0; i < surface->w; i++)
			for (int j = surface->h - 1; j >= 0; j--)
				putpixel(rotatedSurface, (surface->h-1)-j, i, getpixel(surface, i, j));
	}
	else if (rotation == 2)
	{
		for (int i = 0; i < surface->w; i++)
			for (int j = surface->h - 1; j >= 0; j--)
				putpixel(rotatedSurface, (surface->w-1)-i, (surface->h-1)-j, getpixel(surface, i, j));
	}
	else if (rotation == 3)
	{
		for (int i = 0; i < surface->w; i++)
			for (int j = surface->h - 1; j >= 0; j--)
				putpixel(rotatedSurface, j, (surface->w-1)-i, getpixel(surface, i, j));
	}

	return rotatedSurface;
}
