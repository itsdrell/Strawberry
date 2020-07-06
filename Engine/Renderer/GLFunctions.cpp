#include "GLFunctions.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Core/General/EngineCommon.hpp"


//===============================================================================================
#ifndef EMSCRIPTEN_PORT

//PFNGLCLEARPROC glClear = nullptr;
//PFNGLCLEARCOLORPROC glClearColor = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
//PFNGLDRAWARRAYSPROC glDrawArrays = nullptr;
//PFNGLLINEWIDTHPROC glLineWidth = nullptr;
//PFNGLENABLEPROC glEnable = nullptr;
//PFNGLBLENDFUNCPROC glBlendFunc = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLGENSAMPLERSPROC glGenSamplers = nullptr;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = nullptr;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = nullptr;
PFNGLBINDSAMPLERPROC glBindSampler = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexturePls = nullptr;
//PFNGLBINDTEXTUREPROC glBindTexture = nullptr;
//PFNGLPIXELSTOREIPROC glPixelStorei = nullptr;
//PFNGLGENTEXTURESPROC glGenTextures = nullptr;
//PFNGLTEXPARAMETERIPROC glTexParameteri = nullptr;
//PFNGLTEXIMAGE2DPROC glTexImage2D = nullptr;
//PFNGLCLEARDEPTHPROC glClearDepth = nullptr;
//PFNGLDEPTHFUNCPROC glDepthFunc = nullptr;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = nullptr;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture = nullptr;
PFNGLDRAWBUFFERSPROC glDrawBuffers = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = nullptr; 
//PFNGLGETERRORPROC glGetError = nullptr;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = nullptr;
//PFNGLDEPTHMASKPROC glDepthMask = nullptr;
//PFNGLGETTEXIMAGEPROC glGetTexImage = nullptr;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = nullptr;
//PFNGLDRAWELEMENTSPROC glDrawElements = nullptr;
//PFNGLPOLYGONMODEPROC glPolygonMode = nullptr;
//PFNGLDISABLEPROC glDisable = nullptr;
//PFNGLCULLFACEPROC glCullFace = nullptr;
//PFNGLFRONTFACEPROC glFrontFace = nullptr;
PFNGLBLENDFUNCIPROC glBlendFunci = nullptr;
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORM1FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM1FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM1FVPROC glUniform4fv = nullptr;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = nullptr;
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = nullptr;
PFNGLTEXSTORAGE2DPROC glTexStorage2D = nullptr;
//PFNGLDELETETEXTURESPROC glDeleteTextures = nullptr;
//PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D = nullptr;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;

#endif



//-----------------------------------------------------------------------------------------------
void BindGLFunctions()
{
#ifndef EMSCRIPTEN_PORT
	
	//glClear = (PFNGLCLEARPROC) SDL_GL_GetProcAddress("glClear");
	//glClearColor = (PFNGLCLEARCOLORPROC) SDL_GL_GetProcAddress("glClearColor");
	glDeleteShader = (PFNGLDELETESHADERPROC) SDL_GL_GetProcAddress("glDeleteShader");
	glCreateShader = (PFNGLCREATESHADERPROC) SDL_GL_GetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC) SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC) SDL_GL_GetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) SDL_GL_GetProcAddress("glGetShaderInfoLog");
	glAttachShader = (PFNGLATTACHSHADERPROC) SDL_GL_GetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC) SDL_GL_GetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC) SDL_GL_GetProcAddress("glGetProgramiv");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC) SDL_GL_GetProcAddress("glDeleteProgram");
	glDetachShader = (PFNGLDETACHSHADERPROC) SDL_GL_GetProcAddress("glDetachShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC) SDL_GL_GetProcAddress("glCreateProgram");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) SDL_GL_GetProcAddress("glGetProgramInfoLog");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) SDL_GL_GetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) SDL_GL_GetProcAddress("glBindVertexArray");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) SDL_GL_GetProcAddress("glDeleteBuffers");
	glGenBuffers = (PFNGLGENBUFFERSPROC ) SDL_GL_GetProcAddress("glGenBuffers");
	glBindBuffer = ( PFNGLBINDBUFFERPROC) SDL_GL_GetProcAddress("glBindBuffer");
	glBufferData = ( PFNGLBUFFERDATAPROC ) SDL_GL_GetProcAddress("glBufferData");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC ) SDL_GL_GetProcAddress("glGetAttribLocation");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC ) SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC ) SDL_GL_GetProcAddress("glVertexAttribPointer");
	glUseProgram = (PFNGLUSEPROGRAMPROC ) SDL_GL_GetProcAddress("glUseProgram");
	//glDrawArrays = ( PFNGLDRAWARRAYSPROC ) SDL_GL_GetProcAddress("glDrawArrays");
	//glLineWidth = (PFNGLLINEWIDTHPROC ) SDL_GL_GetProcAddress("glLineWidth");
	//glEnable = (PFNGLENABLEPROC ) SDL_GL_GetProcAddress("glEnable");
	//glBlendFunc = (PFNGLBLENDFUNCPROC ) SDL_GL_GetProcAddress("glBlendFunc");
	glGetUniformLocation = ( PFNGLGETUNIFORMLOCATIONPROC ) SDL_GL_GetProcAddress("glGetUniformLocation");
	glUniformMatrix4fv = ( PFNGLUNIFORMMATRIX4FVPROC ) SDL_GL_GetProcAddress("glUniformMatrix4fv");
	glGenSamplers= ( PFNGLGENSAMPLERSPROC) SDL_GL_GetProcAddress("glGenSamplers");
	glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC ) SDL_GL_GetProcAddress("glSamplerParameteri");
	glDeleteSamplers = (PFNGLDELETESAMPLERSPROC ) SDL_GL_GetProcAddress("glDeleteSamplers");
	glBindSampler = (PFNGLBINDSAMPLERPROC ) SDL_GL_GetProcAddress("glBindSampler");
	glActiveTexturePls = (PFNGLACTIVETEXTUREPROC ) SDL_GL_GetProcAddress("glActiveTexture");
	//glBindTexture = (PFNGLBINDTEXTUREPROC ) SDL_GL_GetProcAddress("glBindTexture");
	//glPixelStorei = (PFNGLPIXELSTOREIPROC ) SDL_GL_GetProcAddress("glPixelStorei");
	//glGenTextures = (PFNGLGENTEXTURESPROC ) SDL_GL_GetProcAddress("glGenTextures");
	//glTexParameteri = (PFNGLTEXPARAMETERIPROC ) SDL_GL_GetProcAddress("glTexParameteri");
	//glTexImage2D = (PFNGLTEXIMAGE2DPROC ) SDL_GL_GetProcAddress("glTexImage2D");
	//glClearDepth = (PFNGLCLEARDEPTHPROC ) SDL_GL_GetProcAddress("glClearDepth");
	//glDepthFunc = (PFNGLDEPTHFUNCPROC ) SDL_GL_GetProcAddress("glDepthFunc");
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC ) SDL_GL_GetProcAddress("glGenFramebuffers");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC ) SDL_GL_GetProcAddress("glDeleteFramebuffers");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC ) SDL_GL_GetProcAddress("glBindFramebuffer");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) SDL_GL_GetProcAddress("glFramebufferTexture2D");
	glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC ) SDL_GL_GetProcAddress("glFramebufferTexture");
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC ) SDL_GL_GetProcAddress("glDrawBuffers");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC ) SDL_GL_GetProcAddress("glCheckFramebufferStatus");
	//glGetError = (PFNGLGETERRORPROC ) SDL_GL_GetProcAddress("glGetError");
	glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC ) SDL_GL_GetProcAddress("glBlitFramebuffer");
	//glDepthMask = (PFNGLDEPTHMASKPROC ) SDL_GL_GetProcAddress("glDepthMask");
	//glGetTexImage = (PFNGLGETTEXIMAGEPROC ) SDL_GL_GetProcAddress("glGetTexImage");
	glBindBufferBase = (PFNGLBINDBUFFERBASEPROC ) SDL_GL_GetProcAddress("glBindBufferBase");
	//glDrawElements = (PFNGLDRAWELEMENTSPROC ) SDL_GL_GetProcAddress("glDrawElements");
	//glPolygonMode = (PFNGLPOLYGONMODEPROC ) SDL_GL_GetProcAddress("glPolygonMode");
	//glDisable = (PFNGLDISABLEPROC ) SDL_GL_GetProcAddress("glDisable");
	//glCullFace = (PFNGLCULLFACEPROC ) SDL_GL_GetProcAddress("glCullFace");
	//glFrontFace = (PFNGLFRONTFACEPROC ) SDL_GL_GetProcAddress("glFrontFace");
	glBlendFunci = (PFNGLBLENDFUNCIPROC ) SDL_GL_GetProcAddress("glBlendFunci");
	glUniform1fv = (PFNGLUNIFORM1FVPROC ) SDL_GL_GetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC ) SDL_GL_GetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC ) SDL_GL_GetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC ) SDL_GL_GetProcAddress("glUniform4fv");
	glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC ) SDL_GL_GetProcAddress("glBlendFuncSeparate");
	glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC ) SDL_GL_GetProcAddress("glBlendEquationSeparate");
	glTexStorage2D = (PFNGLTEXSTORAGE2DPROC ) SDL_GL_GetProcAddress("glTexStorage2D");
	//glDeleteTextures = (PFNGLDELETETEXTURESPROC ) SDL_GL_GetProcAddress("glDeleteTextures");
	//glTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC ) SDL_GL_GetProcAddress("glTexSubImage2D");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC ) SDL_GL_GetProcAddress("glGenerateMipmap");
	glSamplerParameterf= (PFNGLSAMPLERPARAMETERFPROC ) SDL_GL_GetProcAddress("glSamplerParameterf");
	glUniform1i = (PFNGLUNIFORM1IPROC) SDL_GL_GetProcAddress("glUniform1i");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glDisableVertexAttribArray");


#else
	//glActiveTexturePls = glActiveTexture;


/*
	glActiveTexture
	glGetTexImage
	glTexStorage2D 
	glFramebufferTexture
	glDrawBuffers 
	glBlitFramebuffer 
	glBindBufferBase 
*/
#endif
}

//===============================================================================================
bool GLCheckError(char const * file, int line)
{
#if defined(_DEBUG)
	#ifndef EMSCRIPTEN_PORT
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) 
	{
		DebuggerPrintf( "\nGL ERROR [0x%04x] at [%s(%i)] \n", error, file, line );
		return true; 
	}
	#endif
#endif
	return false;
}

//-----------------------------------------------------------------------------------------------
bool GLFailed()
{
#ifndef EMSCRIPTEN_PORT
	return GL_CHECK_ERROR(); 
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------------------------
bool GLSucceeded()
{
	return !GLFailed();
}

//-----------------------------------------------------------------------------------------------
void GLCheckErrorAndDie(char const * file, char const * function, int line)
{
#if defined(_DEBUG)
	#ifndef EMSCRIPTEN_PORT
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) 
	{
		UNUSED(file);
		UNUSED(function);
		UNUSED(line);
		//ERROR_RECOVERABLE( Stringf("\nGL ERROR [0x%04x] in [%s] at [%s(%i)] \n", error, function, file, line).c_str() );
	}
	#endif
#endif
}


//===============================================================================================



