#ifndef _PHAGE_FRAME_BUFFER_H
#define _PHAGE_FRAME_BUFFER_H

#include <stdlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

namespace Phage
{
	class PhageFrameBuffer
	{
	public:
		const static int COLOR_BUFFER = 1;
		const static int DEPTH_BUFFER = 2;
		const static int COLOR_DEPTH_BUFFER = 3;
		const static int COLOR_DEPTH_G_BUFFER = 4;
		const static int COLOR_SINGLE_CHANNEL = 5;
	public:
		PhageFrameBuffer();
		~PhageFrameBuffer();

		int Init();
		int CreateFrameBuffer(int w, int h, int bufferType, bool linearColor, bool linearDepth);
		int DeleteFrameBuffer();
		int ClearFrameBuffer();
		int ClearFrameBuffer(float r, float g, float b);
		int BindFrameBuffer();
		int UnbindFrameBuffer();

		GLuint GetFrameBufferID();
		GLuint GetColorTextureID();
		GLuint GetDepthTextureID();
		GLuint GetGPosTextureID();
		GLuint GetGNormTextureID();
		GLuint GetGMiscTextureID();
	private:
		GLuint _FBO;
		GLuint _fbColorTexture;
		GLuint _fbDepthTexture;
		GLuint _fbGPosTexture;
		GLuint _fbGNormTexture;
		GLuint _fbGMiscTexture;

		int _width;
		int _height;

		int _bufferType;
	};
}

#endif
