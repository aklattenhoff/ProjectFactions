#include "Phage_FrameBuffer.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Phage::PhageFrameBuffer::PhageFrameBuffer()
{
}

Phage::PhageFrameBuffer::~PhageFrameBuffer()
{
	DeleteFrameBuffer();
}

int Phage::PhageFrameBuffer::Init()
{

	return 0;
}

int Phage::PhageFrameBuffer::CreateFrameBuffer(int w, int h, int bufferType, bool linearColor, bool linearDepth)
{
	_width = w;
	_height = h;
	_bufferType = bufferType;

	glGenFramebuffers(1, &_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (_bufferType == COLOR_BUFFER || _bufferType == COLOR_DEPTH_BUFFER || COLOR_DEPTH_G_BUFFER)
	{
		glGenTextures(1, &_fbColorTexture);
		glBindTexture(GL_TEXTURE_2D, _fbColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		if (linearColor)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fbColorTexture, 0);
	}
	else if (_bufferType == COLOR_SINGLE_CHANNEL)
	{
		glGenTextures(1, &_fbColorTexture);
		glBindTexture(GL_TEXTURE_2D, _fbColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _width, _height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
		if (linearColor)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fbColorTexture, 0);
	}

	if (_bufferType == DEPTH_BUFFER || _bufferType == COLOR_DEPTH_BUFFER || _bufferType == COLOR_DEPTH_G_BUFFER || _bufferType == COLOR_SINGLE_CHANNEL)
	{
		glGenTextures(1, &_fbDepthTexture);
		glBindTexture(GL_TEXTURE_2D, _fbDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		if (linearDepth)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _fbDepthTexture, 0);
	}

	if (_bufferType == COLOR_DEPTH_G_BUFFER)
	{
		glGenTextures(1, &_fbGPosTexture);
		glBindTexture(GL_TEXTURE_2D, _fbGPosTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _fbGPosTexture, 0);

		glGenTextures(1, &_fbGNormTexture);
		glBindTexture(GL_TEXTURE_2D, _fbGNormTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _fbGNormTexture, 0);

		glGenTextures(1, &_fbGMiscTexture);
		glBindTexture(GL_TEXTURE_2D, _fbGMiscTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, _fbGMiscTexture, 0);
	}

	if (_bufferType == COLOR_BUFFER || _bufferType == COLOR_DEPTH_BUFFER || _bufferType == COLOR_SINGLE_CHANNEL)
	{
		GLenum attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);
	}
	else if (_bufferType == COLOR_DEPTH_G_BUFFER)
	{
		GLenum attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("ERROR: %i\n", (int)glCheckFramebufferStatus(_FBO));
		//exit(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}

int Phage::PhageFrameBuffer::DeleteFrameBuffer()
{
	glDeleteTextures(1, &_fbColorTexture);
	glDeleteTextures(1, &_fbDepthTexture);
	glDeleteTextures(1, &_fbGPosTexture);
	glDeleteTextures(1, &_fbGNormTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glDeleteFramebuffers(1, &_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}

int Phage::PhageFrameBuffer::ClearFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (_bufferType == COLOR_BUFFER || _bufferType == COLOR_DEPTH_BUFFER || _bufferType == COLOR_SINGLE_CHANNEL)
	{
		GLenum attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);
	}
	else if (_bufferType == COLOR_DEPTH_G_BUFFER)
	{
		GLenum attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}

int Phage::PhageFrameBuffer::ClearFrameBuffer(float r, float g, float b)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (_bufferType == COLOR_BUFFER || _bufferType == COLOR_DEPTH_BUFFER || _bufferType == COLOR_SINGLE_CHANNEL)
	{
		GLenum attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);
	}
	else if (_bufferType == COLOR_DEPTH_G_BUFFER)
	{
		GLenum attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);
	}

	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}

int Phage::PhageFrameBuffer::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (_bufferType == COLOR_DEPTH_BUFFER)
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
	}
	else if(_bufferType == COLOR_BUFFER)
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_FALSE);
	}
	else if (_bufferType == DEPTH_BUFFER)
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_TRUE);
	}
	else if (_bufferType == COLOR_DEPTH_G_BUFFER)
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
	}

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	//glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (_bufferType == COLOR_BUFFER || _bufferType == COLOR_DEPTH_BUFFER || _bufferType == COLOR_SINGLE_CHANNEL)
	{
		GLenum attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);
	}
	else if (_bufferType == COLOR_DEPTH_G_BUFFER)
	{
		GLenum attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);
	}

	return 0;
}

int Phage::PhageFrameBuffer::UnbindFrameBuffer()
{
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}

GLuint Phage::PhageFrameBuffer::GetFrameBufferID()
{
	return _FBO;
}

GLuint Phage::PhageFrameBuffer::GetColorTextureID()
{
	return _fbColorTexture;
}

GLuint Phage::PhageFrameBuffer::GetDepthTextureID()
{
	return _fbDepthTexture;
}

GLuint Phage::PhageFrameBuffer::GetGPosTextureID()
{
	return _fbGPosTexture;
}

GLuint Phage::PhageFrameBuffer::GetGNormTextureID()
{
	return _fbGNormTexture;
}

GLuint Phage::PhageFrameBuffer::GetGMiscTextureID()
{
	return _fbGMiscTexture;
}
