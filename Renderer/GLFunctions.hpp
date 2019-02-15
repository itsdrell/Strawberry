#pragma once
#include "Engine/Renderer/external/gl/glcorearb.h"
#include "Engine/Renderer/external/gl/glext.h"

#ifndef EMSCRIPTEN_PORT
#include "Engine/Renderer/external/gl/wglext.h"
extern HMODULE gGLLibrary; // needed for the template to work
#endif

//====================================================================================
// Forward Declare
//====================================================================================
//typedef int HMODULE; // clears up an error
//void GetProcAddress(HMODULE, char const*) {}

//====================================================================================
// Type Defs + Defines
//====================================================================================
#define GL_CHECK_ERROR()	GLCheckError( __FILE__, __LINE__ )
#define GL_CHECK_ERROR_AND_DIE() GLCheckErrorAndDie( __FILE__, __FUNCTION__, __LINE__ )
#define GL_BIND_FUNCTION(f)      wglGetTypedProcAddress( &f, #f )


//====================================================================================
// Standalone C Functions
//====================================================================================
bool GLCheckError( char const *file, int line );
bool GLFailed();
bool GLSucceeded();
void GLCheckErrorAndDie( char const *file, char const* function, int line );

void BindNewGLFunctions();
void BindGLFunctions();


//===============================================================================================
// Templates
//===============================================================================================
template <typename T>
bool wglGetTypedProcAddress( T *out, char const *name ) 
{
#ifndef EMSCRIPTEN_PORT
	// Grab the function from the currently bound render context
	// most opengl 2.0+ features will be found here
	*out = (T) wglGetProcAddress(name); 


	if ((*out) == nullptr) {
		// if it is not part of wgl (the device), then attempt to get it from the GLL library
		// (most OpenGL functions come from here)
		*out = (T) GetProcAddress( gGLLibrary, name); 
		//*out = (T) GetProcAddress( (HMODULE) Renderer::GetInstance()->GetGLLibrary(), name); 

	}

	return (*out != nullptr); 
#endif

	return true;
}


//====================================================================================
// Externs
//====================================================================================
extern PFNGLCLEARPROC glClear;
extern PFNGLCLEARCOLORPROC glClearColor;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLDRAWARRAYSPROC glDrawArrays;
extern PFNGLLINEWIDTHPROC glLineWidth;
extern PFNGLENABLEPROC glEnable;
extern PFNGLBLENDFUNCPROC glBlendFunc;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLGENSAMPLERSPROC glGenSamplers;
extern PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
extern PFNGLDELETESAMPLERSPROC glDeleteSamplers;
extern PFNGLBINDSAMPLERPROC glBindSampler;
extern PFNGLACTIVETEXTUREARBPROC glActiveTexture;
extern PFNGLBINDTEXTUREPROC glBindTexture;
extern PFNGLPIXELSTOREIPROC glPixelStorei;
extern PFNGLGENTEXTURESPROC glGenTextures;
extern PFNGLTEXPARAMETERIPROC glTexParameteri;
extern PFNGLTEXIMAGE2DPROC glTexImage2D;
extern PFNGLCLEARDEPTHPROC glClearDepth;
extern PFNGLDEPTHFUNCPROC glDepthFunc;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
extern PFNGLDRAWBUFFERSPROC glDrawBuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus; 
extern PFNGLGETERRORPROC glGetError;
extern PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
extern PFNGLDEPTHMASKPROC glDepthMask;
extern PFNGLGETTEXIMAGEPROC glGetTexImage;
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;
extern PFNGLDRAWELEMENTSPROC glDrawElements;
extern PFNGLPOLYGONMODEPROC glPolygonMode;
extern PFNGLDISABLEPROC glDisable;
extern PFNGLCULLFACEPROC glCullFace;
extern PFNGLFRONTFACEPROC glFrontFace;
extern PFNGLBLENDFUNCIPROC glBlendFunci;
extern PFNGLUNIFORM1FVPROC glUniform1fv;
extern PFNGLUNIFORM1FVPROC glUniform2fv;
extern PFNGLUNIFORM1FVPROC glUniform3fv;
extern PFNGLUNIFORM1FVPROC glUniform4fv;
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
extern PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
extern PFNGLTEXSTORAGE2DPROC glTexStorage2D;
extern PFNGLDELETETEXTURESPROC glDeleteTextures;
extern PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf;


#ifdef EMSCRIPTEN_PORT
#else
// wgl
extern PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#endif

//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================