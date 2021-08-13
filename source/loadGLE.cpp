#include "loadGLE.h"

glShaderSource_Func glShaderSource;
glGetShaderiv_Func glGetShaderiv;
glGetShaderInfoLog_Func glGetShaderInfoLog;
glCreateShader_Func glCreateShader;
glCompileShader_Func glCompileShader;
glCreateProgram_Func glCreateProgram;
glAttachShader_Func glAttachShader;
glLinkProgram_Func glLinkProgram;
glGetProgramiv_Func glGetProgramiv;
glGetProgramInfoLog_Func glGetProgramInfoLog;
glGenBuffers_Func glGenBuffers;
glBindBuffer_Func glBindBuffer;
glBufferData_Func glBufferData;
glGenVertexArrays_Func glGenVertexArrays;
glBindVertexArray_Func glBindVertexArray;
glUseProgram_Func glUseProgram;
glGetAttribLocation_Func glGetAttribLocation;
glEnableVertexAttribArray_Func glEnableVertexAttribArray;
glVertexAttribPointer_Func glVertexAttribPointer;
glVertexAttribIPointer_Func glVertexAttribIPointer;
glDisableVertexAttribArray_Func glDisableVertexAttribArray;
glGetUniformLocation_Func glGetUniformLocation;
glUniformMatrix4fv_Func glUniformMatrix4fv;
glUniform2fv_Func glUniform2fv;
glUniform3fv_Func glUniform3fv;
glUniform4fv_Func glUniform4fv;
glUniform1i_Func glUniform1i;
glUniform1f_Func glUniform1f;
glUniform1iv_Func glUniform1iv;
glActiveTexture_Func glActiveTexture;
glGetBufferSubData_Func glGetBufferSubData;
glGenFramebuffers_Func glGenFramebuffers;
glDeleteFramebuffers_Func glDeleteFramebuffers;
glBindFramebuffer_Func glBindFramebuffer;
glFramebufferTexture2D_Func glFramebufferTexture2D;
glFramebufferTexture_Func glFramebufferTexture;
glDrawBuffers_Func glDrawBuffers;
glCheckFramebufferStatus_Func glCheckFramebufferStatus;
glGenRenderbuffers_Func glGenRenderbuffers;
glBindRenderbuffer_Func glBindRenderbuffer;
glRenderbufferStorage_Func glRenderbufferStorage;
glFramebufferRenderbuffer_Func glFramebufferRenderbuffer;
glTexStorage2D_Func glTexStorage2D;
glGenerateMipmap_Func glGenerateMipmap;
glBlitFramebuffer_Func glBlitFramebuffer;

int LoadGLExtensions()
{
	//glShaderSource
	glShaderSource = (glShaderSource_Func) SDL_GL_GetProcAddress("glShaderSource");
	//glGetShaderiv
	glGetShaderiv = (glGetShaderiv_Func) SDL_GL_GetProcAddress("glGetShaderiv");
	//glGetShaderInfoLog
	glGetShaderInfoLog = (glGetShaderInfoLog_Func) SDL_GL_GetProcAddress("glGetShaderInfoLog");
	//glCreateShader
	glCreateShader = (glCreateShader_Func) SDL_GL_GetProcAddress("glCreateShader");
	//glCompileShader
	glCompileShader = (glCompileShader_Func) SDL_GL_GetProcAddress("glCompileShader");
	//glCreateProgram
	glCreateProgram = (glCreateProgram_Func) SDL_GL_GetProcAddress("glCreateProgram");
	//glAttachShader
	glAttachShader = (glAttachShader_Func) SDL_GL_GetProcAddress("glAttachShader");
	//glLinkProgram
	glLinkProgram = (glLinkProgram_Func) SDL_GL_GetProcAddress("glLinkProgram");
	//glGetProgramiv
	glGetProgramiv = (glGetProgramiv_Func) SDL_GL_GetProcAddress("glGetProgramiv");
	//glGetProgramInfoLog
	glGetProgramInfoLog = (glGetProgramInfoLog_Func) SDL_GL_GetProcAddress("glGetProgramInfoLog");
	//glGenBuffers
	glGenBuffers = (glGenBuffers_Func) SDL_GL_GetProcAddress("glGenBuffers");
	//glBindBuffer
	glBindBuffer = (glBindBuffer_Func) SDL_GL_GetProcAddress("glBindBuffer");
	//glBufferData
	glBufferData = (glBufferData_Func) SDL_GL_GetProcAddress("glBufferData");
	//glGenVertexArrays
	glGenVertexArrays = (glGenVertexArrays_Func) SDL_GL_GetProcAddress("glGenVertexArrays");
	//glBindVertexArray
	glBindVertexArray = (glBindVertexArray_Func) SDL_GL_GetProcAddress("glBindVertexArray");
	//glUseProgram
	glUseProgram = (glUseProgram_Func) SDL_GL_GetProcAddress("glUseProgram");
	//glGetAttribLocation
	glGetAttribLocation = (glGetAttribLocation_Func) SDL_GL_GetProcAddress("glGetAttribLocation");
	//glEnableVertexAttribArray
	glEnableVertexAttribArray = (glEnableVertexAttribArray_Func) SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	//glVertexAttribPointer
	glVertexAttribPointer = (glVertexAttribPointer_Func) SDL_GL_GetProcAddress("glVertexAttribPointer");
	//glVertexAttribIPointer
	glVertexAttribIPointer = (glVertexAttribIPointer_Func) SDL_GL_GetProcAddress("glVertexAttribIPointer");
	//glDisableVertexAttribArray
	glDisableVertexAttribArray = (glDisableVertexAttribArray_Func) SDL_GL_GetProcAddress("glDisableVertexAttribArray");
	//glGetUniformLocation
	glGetUniformLocation = (glGetUniformLocation_Func) SDL_GL_GetProcAddress("glGetUniformLocation");
	//glUniformMatrix4fv
	glUniformMatrix4fv = (glUniformMatrix4fv_Func) SDL_GL_GetProcAddress("glUniformMatrix4fv");
	//glUniform2fv
	glUniform2fv = (glUniform2fv_Func) SDL_GL_GetProcAddress("glUniform2fv");
	//glUniform3fv
	glUniform3fv = (glUniform3fv_Func) SDL_GL_GetProcAddress("glUniform3fv");
	//glUniform4fv
	glUniform4fv = (glUniform4fv_Func) SDL_GL_GetProcAddress("glUniform4fv");
	//glUniform1i
	glUniform1i = (glUniform1i_Func) SDL_GL_GetProcAddress("glUniform1i");
	//glUniform1f
	glUniform1f = (glUniform1f_Func) SDL_GL_GetProcAddress("glUniform1f");
	//glUniform1fv
	glUniform1iv = (glUniform1iv_Func) SDL_GL_GetProcAddress("glUniform1iv");
	//glActiveTexture
	glActiveTexture = (glActiveTexture_Func) SDL_GL_GetProcAddress("glActiveTexture");
	//glGetBufferSubData
	glGetBufferSubData = (glGetBufferSubData_Func) SDL_GL_GetProcAddress("glGetBufferSubData");
	//glGenFramebuffers
	glGenFramebuffers = (glGenFramebuffers_Func) SDL_GL_GetProcAddress("glGenFramebuffers");
	//glDeleteFramebuffers
	glDeleteFramebuffers = (glDeleteFramebuffers_Func) SDL_GL_GetProcAddress("glDeleteFramebuffers");
	//glBindFramebuffer
	glBindFramebuffer = (glBindFramebuffer_Func) SDL_GL_GetProcAddress("glBindFramebuffer");
	//glFramebufferTexture2D
	glFramebufferTexture2D = (glFramebufferTexture2D_Func) SDL_GL_GetProcAddress("glFramebufferTexture2D");
	//glFramebufferTexture
	glFramebufferTexture = (glFramebufferTexture_Func) SDL_GL_GetProcAddress("glFramebufferTexture");
	//glDrawBuffers
	glDrawBuffers = (glDrawBuffers_Func) SDL_GL_GetProcAddress("glDrawBuffers");
	//glCheckFramebufferStatus
	glCheckFramebufferStatus = (glCheckFramebufferStatus_Func) SDL_GL_GetProcAddress("glCheckFramebufferStatus");
	//glGenRenderbuffers
	glGenRenderbuffers = (glGenRenderbuffers_Func) SDL_GL_GetProcAddress("glGenRenderbuffers");
	//glBindRenderbuffer
	glBindRenderbuffer = (glBindRenderbuffer_Func) SDL_GL_GetProcAddress("glBindRenderbuffer");
	//glRenderbufferStorage
	glRenderbufferStorage = (glRenderbufferStorage_Func) SDL_GL_GetProcAddress("glRenderbufferStorage");
	//glFramebufferRenderbuffer
	glFramebufferRenderbuffer = (glFramebufferRenderbuffer_Func) SDL_GL_GetProcAddress("glFramebufferRenderbuffer");
	//glTexStorage2D;
	glTexStorage2D = (glTexStorage2D_Func) SDL_GL_GetProcAddress("glTexStorage2D");
	//glGenerateMipmap
	glGenerateMipmap = (glGenerateMipmap_Func) SDL_GL_GetProcAddress("glGenerateMipmap");
	//glBlitFramebuffer
	glBlitFramebuffer = (glBlitFramebuffer_Func) SDL_GL_GetProcAddress("glBlitFramebuffer");

	return 0;
}
