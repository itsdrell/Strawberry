#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/RenderTypes.hpp"

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const String& name, const char* fs, const char* vs, Strings defines);


public:
	GLuint		m_programHandle; // OpenGL handle for this program, default 0
	String		m_name;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
static GLuint LoadShaderFromLiteral(const char*  shaderSource, GLenum type, Strings defines);
static GLuint CreateAndLinkProgram( GLint vs, GLint fs );
static void LogProgramError(GLuint program_id);
static void LogShaderError(GLuint shader_id, const char* filename, uint defineOffset);
static void FormatAndPrintShaderErrors(std::string log, std::string path, uint defineOffset);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/31/2019]
//====================================================================================