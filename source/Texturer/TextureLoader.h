#ifndef _TEXTURE_LOADER
#define _TEXTURE_LOADER

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#define TL_NEAREST 0
#define TL_LINEAR 1
#define TL_MIPMAP 2

#ifdef __cplusplus
extern "C" {
#endif

	extern GLuint TextureLoader_LoadFromFile(const char* fileName, int interp = 1);
	extern GLenum TextureLoader_GetTextureFormat(SDL_Surface* surface);

	//Rotation is done in 90 degree increments
	//0: simply returns the passed surface (this is used for simplifying "for" statements that use modulus. i.e. rotation = i%4)
	//1: 90 degrees CW
	//2: 180 degrees CW
	//3: 270 degrees CW
	extern SDL_Surface* TextureLoader_PreprocessRotation(SDL_Surface* surface, int rotation);

#ifdef __cplusplus
}
#endif

#endif /* _TEXTURE_LOADER */
