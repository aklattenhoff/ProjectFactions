#ifndef _LOAD_GLE
#define _LOAD_GLE

//#define GL3_PROTOTYPES 1
//#include <GL3/gl3.h>
//#include <gl/GL.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int LoadGLExtensions();

//glShaderSource
typedef void (APIENTRY* glShaderSource_Func)(GLuint, GLsizei, const GLchar**, const GLint);
extern glShaderSource_Func glShaderSource;

//glGetShaderiv
typedef void (APIENTRY* glGetShaderiv_Func)(GLuint, GLenum, GLint*);
extern glGetShaderiv_Func glGetShaderiv;

//glGetShaderInfoLog
typedef void (APIENTRY* glGetShaderInfoLog_Func)(GLuint, GLsizei, GLsizei*, GLchar*);
extern glGetShaderInfoLog_Func glGetShaderInfoLog;

//glCreateShader
typedef GLuint (APIENTRY* glCreateShader_Func)(GLenum);
extern glCreateShader_Func glCreateShader;

//glCompileShader
typedef void (APIENTRY* glCompileShader_Func)(GLuint);
extern glCompileShader_Func glCompileShader;

//glCreateProgram
typedef GLuint (APIENTRY* glCreateProgram_Func)(void);
extern glCreateProgram_Func glCreateProgram;

//glAttachShader
typedef void (APIENTRY* glAttachShader_Func)(GLuint, GLuint);
extern glAttachShader_Func glAttachShader;

//glLinkProgram
typedef void (APIENTRY* glLinkProgram_Func)(GLuint);
extern glLinkProgram_Func glLinkProgram;

//glGetProgramiv
typedef void (APIENTRY* glGetProgramiv_Func)(GLuint, GLenum, GLint*);
extern glGetProgramiv_Func glGetProgramiv;

//glGetProgramInfoLog
typedef void (APIENTRY* glGetProgramInfoLog_Func)(GLuint, GLsizei, GLsizei*, GLchar*);
extern glGetProgramInfoLog_Func glGetProgramInfoLog;

//glGenBuffers
typedef void (APIENTRY* glGenBuffers_Func)(GLsizei, GLuint*);
extern glGenBuffers_Func glGenBuffers;

//glBindBuffer
typedef void (APIENTRY* glBindBuffer_Func)(GLenum, GLuint);
extern glBindBuffer_Func glBindBuffer;

//glBufferData
typedef void (APIENTRY* glBufferData_Func)(GLenum, GLsizeiptr, const GLvoid*, GLenum);
extern glBufferData_Func glBufferData;

//glGenVertexArrays
typedef void (APIENTRY* glGenVertexArrays_Func)(GLsizei, GLuint*);
extern glGenVertexArrays_Func glGenVertexArrays;

//glBindVertexArray
typedef void (APIENTRY* glBindVertexArray_Func)(GLuint);
extern glBindVertexArray_Func glBindVertexArray;

//glUseProgram
typedef void (APIENTRY* glUseProgram_Func)(GLuint);
extern glUseProgram_Func glUseProgram;

//glGetAttribLocation
typedef GLint (APIENTRY* glGetAttribLocation_Func)(GLuint, const GLchar*);
extern glGetAttribLocation_Func glGetAttribLocation;

//glEnableVertexAttribArray
typedef void (APIENTRY* glEnableVertexAttribArray_Func)(GLuint);
extern glEnableVertexAttribArray_Func glEnableVertexAttribArray;

//glVertexAttribPointer
typedef void (APIENTRY* glVertexAttribPointer_Func)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
extern glVertexAttribPointer_Func glVertexAttribPointer;

//glVertexAttribIPointer
typedef void (APIENTRY* glVertexAttribIPointer_Func)(GLuint, GLint, GLenum, GLsizei, const GLvoid*);
extern glVertexAttribIPointer_Func glVertexAttribIPointer;

//glDisableVertexAttribArray
typedef void (APIENTRY* glDisableVertexAttribArray_Func)(GLuint);
extern glDisableVertexAttribArray_Func glDisableVertexAttribArray;

//glGetUniformLocation
typedef GLint (APIENTRY* glGetUniformLocation_Func)(GLuint, const GLchar*);
extern glGetUniformLocation_Func glGetUniformLocation;

//glUniformMatrix4fv
typedef void (APIENTRY* glUniformMatrix4fv_Func)(GLint, GLsizei, GLboolean, const GLfloat*);
extern glUniformMatrix4fv_Func glUniformMatrix4fv;

//glUniform2fv
typedef void (APIENTRY* glUniform2fv_Func)(GLint, GLsizei, const GLfloat*);
extern glUniform2fv_Func glUniform2fv;

//glUniform3fv
typedef void (APIENTRY* glUniform3fv_Func)(GLint, GLsizei, const GLfloat*);
extern glUniform3fv_Func glUniform3fv;

//glUniform4fv
typedef void (APIENTRY* glUniform4fv_Func)(GLint, GLsizei, const GLfloat*);
extern glUniform4fv_Func glUniform4fv;

//glUniform1i
typedef void (APIENTRY* glUniform1i_Func)(GLint, GLint);
extern glUniform1i_Func glUniform1i;

//glUniform1f
typedef void (APIENTRY* glUniform1f_Func)(GLint, GLfloat);
extern glUniform1f_Func glUniform1f;

//glUniform1iv
typedef void (APIENTRY* glUniform1iv_Func)(GLint, GLsizei, const GLint*);
extern glUniform1iv_Func glUniform1iv;

//glActiveTexture
typedef void (APIENTRY* glActiveTexture_Func)(GLenum);
extern glActiveTexture_Func glActiveTexture;

//glGetBufferSubData
typedef void (APIENTRY* glGetBufferSubData_Func)(GLenum, GLintptr, GLsizeiptr, GLvoid*);
extern glGetBufferSubData_Func glGetBufferSubData;

//glGenFramebuffers
typedef void (APIENTRY* glGenFramebuffers_Func)(GLsizei, GLuint*);
extern glGenFramebuffers_Func glGenFramebuffers;

//glDeleteFramebuffers
typedef void (APIENTRY* glDeleteFramebuffers_Func)(GLsizei, GLuint*);
extern glDeleteFramebuffers_Func glDeleteFramebuffers;

//glBindFramebuffer
typedef void (APIENTRY* glBindFramebuffer_Func)(GLenum, GLuint);
extern glBindFramebuffer_Func glBindFramebuffer;

//glFramebufferTexture2D
typedef void (APIENTRY* glFramebufferTexture2D_Func)(GLenum, GLenum, GLenum, GLuint, GLint);
extern glFramebufferTexture2D_Func glFramebufferTexture2D;

//glFramebufferTexture
typedef void (APIENTRY* glFramebufferTexture_Func)(GLenum, GLenum, GLuint, GLint);
extern glFramebufferTexture_Func glFramebufferTexture;

//glDrawBuffers
typedef void (APIENTRY* glDrawBuffers_Func)(GLsizei, const GLenum*);
extern glDrawBuffers_Func glDrawBuffers;

//glCheckFramebufferStatus
typedef GLenum (APIENTRY* glCheckFramebufferStatus_Func)(GLenum);
extern glCheckFramebufferStatus_Func glCheckFramebufferStatus;

//glGenRenderbuffers
typedef void (APIENTRY* glGenRenderbuffers_Func)(GLsizei, GLuint*);
extern glGenRenderbuffers_Func glGenRenderbuffers;

//glBindRenderbuffer
typedef void (APIENTRY* glBindRenderbuffer_Func)(GLenum, GLuint);
extern glBindRenderbuffer_Func glBindRenderbuffer;

//glRenderbufferStorage
typedef void (APIENTRY* glRenderbufferStorage_Func)(GLenum, GLenum, GLsizei, GLsizei);
extern glRenderbufferStorage_Func glRenderbufferStorage;

//glFramebufferRenderbuffer
typedef void (APIENTRY* glFramebufferRenderbuffer_Func)(GLenum, GLenum, GLenum, GLuint);
extern glFramebufferRenderbuffer_Func glFramebufferRenderbuffer;

//glTexStorage2D
typedef void (APIENTRY* glTexStorage2D_Func)(GLenum, GLsizei, GLenum, GLsizei, GLsizei);
extern glTexStorage2D_Func glTexStorage2D;

//glGenerateMipmap
typedef void (APIENTRY* glGenerateMipmap_Func)(GLenum);
extern glGenerateMipmap_Func glGenerateMipmap;

//glBlitFramebuffer
typedef void (APIENTRY* glBlitFramebuffer_Func)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
extern glBlitFramebuffer_Func glBlitFramebuffer;

#ifdef __cplusplus
}
#endif

#endif /* _LOAD_GLE */
