#pragma once
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Renderer/Pipeline/VertexLayout.hpp"
#include "Engine/Renderer/GLFunctions.hpp"
#include "Engine/Math/Matrices/Matrix44.hpp"

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================
enum PrimitiveType
{
	PRIMITIVE_POINTS,		// in OpenGL, for example, this becomes GL_POINTS
	PRIMITIVE_LINES,		// in OpenGL, for example, this becomes GL_LINES
	PRIMITIVE_TRIANGLES,		// in OpenGL, for example, this becomes GL_TRIANGLES
	PRIMITIVE_POLYGON,		// in OpenGl, for example, this becomes GL_POLYGON
	PRIMITIVE_QUADS,		// in OpenGL, for example, this becomes GL_QUADS
	NUM_PRIMITIVE_TYPES
};

//-----------------------------------------------------------------------------------------------
enum TextDrawMode 
{
	TEXT_DRAW_SHRINK_TO_FIT,
	TEXT_DRAW_WORD_WRAP,
	TEXT_DRAW_OVERRUN,
	NUM_DRAW_MODES
};

//-----------------------------------------------------------------------------------------------
enum eTextureFormat 
{
	TEXTURE_FORMAT_RGBA8, // default color format
	TEXTURE_FORMAT_D24S8, 
}; 

//-----------------------------------------------------------------------------------------------
enum eTextureCubeSide
{
	TEXCUBE_RIGHT, 
	TEXCUBE_LEFT,  
	TEXCUBE_TOP,   
	TEXCUBE_BOTTOM,
	TEXCUBE_FRONT, 
	TEXCUBE_BACK
};
GLenum ToGLCubeSide(eTextureCubeSide side);

//-----------------------------------------------------------------------------------------------
enum eCullMode 
{
	CULLMODE_BACK,          // GL_BACK     glEnable(GL_CULL_FACE); glCullFace(GL_BACK); 
	CULLMODE_FRONT,         // GL_FRONT    glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); 
	CULLMODE_NONE           // GL_NONE     glDisable(GL_CULL_FACE)
};
GLenum ToGLCullMode(eCullMode mode);

//-----------------------------------------------------------------------------------------------
enum eFillMode 
{
	FILLMODE_SOLID,         // GL_FILL     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL )
	FILLMODE_WIRE           // GL_LINE     glPolygonMode( GL_FRONT_AND_BACK, GL_LINE )
};
GLenum ToGLFillMode(eFillMode mode);

//-----------------------------------------------------------------------------------------------
enum eWindOrder 
{
	WIND_CLOCKWISE,         // GL_CW       glFrontFace( GL_CW ); 
	WIND_COUNTER_CLOCKWISE // GL_CCW      glFrontFace( GL_CCW ); 
};
GLenum ToGLWindOrder(eWindOrder order);

//-----------------------------------------------------------------------------------------------
// https://www.khronos.org/opengl/wiki/Blending
enum eBlendOperation
{
	COMPARE_ADD,					// GL_FUNC_ADD
	COMPARE_SUBTRACT,				// GL_FUNC_SUBTRACT
	COMPARE_REVERSE_SUBTRACT,		// GL_FUNC_REVERSE_SUBTRACT
	COMPARE_MIN,					// GL_MIN
	COMPARE_MAX						// GL_MAX
};
GLenum ToGLBlendOperation(eBlendOperation operation);

//-----------------------------------------------------------------------------------------------
// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBlendFunc.xml
enum eBlendFactor
{
	BLEND_ZERO,						   //GL_ZERO,
	BLEND_ONE,						   //GL_ONE,
	BLEND_SRC_COLOR,				   //GL_SRC_COLOR,
	BLEND_ONE_MINUS_SRC_COLOR,		   //GL_ONE_MINUS_SRC_COLOR,
	BLEND_DST_COLOR,				   //GL_DST_COLOR,
	BLEND_ONE_MINUS_DST_COLOR,		   //GL_ONE_MINUS_DST_COLOR,
	BLEND_SRC_ALPHA,				   //GL_SRC_ALPHA,
	BLEND_ONE_MINUS_SRC_ALPHA,		   //GL_ONE_MINUS_SRC_ALPHA,
	BLEND_DST_ALPHA,				   //GL_DST_ALPHA,
	BLEND_ONE_MINUS_DST_ALPHA,		   //GL_ONE_MINUS_DST_ALPHA,
	BLEND_CONSTANT_COLOR,			   //GL_CONSTANT_COLOR,
	BLEND_ONE_MINUS_CONSTANT_COLOR,	   //GL_ONE_MINUS_CONSTANT_COLOR,
	BLEND_CONSTANT_ALPHA,			   //GL_CONSTANT_ALPHA,
	BLEND_ONE_MINUS_CONSTANT_ALPHA,	   //GL_ONE_MINUS_CONSTANT_ALPHA,
	BLEND_SRC_ALPHA_SATURATE		   //GL_SRC_ALPHA_SATURATE

};
GLenum ToGLBlendFactor(eBlendFactor factor);

//-----------------------------------------------------------------------------------------------
// See: http://docs.gl/gl4/glClearDepth
enum DepthBufferComparisons
{
	COMPARE_NEVER,       // GL_NEVER
	COMPARE_LESS,        // GL_LESS
	COMPARE_LEQUAL,      // GL_LEQUAL
	COMPARE_GREATER,     // GL_GREATER
	COMPARE_GEQUAL,      // GL_GEQUAL
	COMPARE_EQUAL,       // GL_EQUAL
	COMPARE_NOT_EQUAL,   // GL_NOTEQUAL
	COMPARE_ALWAYS       // GL_ALWAYS
};
GLenum ToGLDepthBufferCompare(DepthBufferComparisons compare);

//-----------------------------------------------------------------------------------------------
enum eRenderDataType 
{
	RENDER_DATA_TYPE_FLOAT,           // GL_FLOAT
	RENDER_DATA_TYPE_UNSIGNED_BYTE,   // GL_UNSIGNED_BYTE
	RENDER_DATA_TYPE_INT,			
									  // ... add more as you need them
};
GLenum ToGLType( eRenderDataType type);

//====================================================================================
// Structs
//====================================================================================
struct VertexMaster
{
	Vector3 	m_position;
	Vector2 	m_uvTexCoords;
	Rgba 		m_color;

	Vector3		m_normal;

	// Tangent could be a vector4 which we use to make the bi tangent
	Vector3		m_tangent;
	Vector3		m_biTangent;
};

//-----------------------------------------------------------------------------------------------
struct Vertex3D_PCU 
{
public:

	Vertex3D_PCU(Vector3 thePosition, Rgba theColor, Vector2 UVcoords);
	Vertex3D_PCU(const VertexMaster& other);
	Vertex3D_PCU(){} // for array

public:
	Vector3		m_position;
	Rgba		m_color;
	Vector2		m_uvTexCoords;

	static const VertexAttributeT ATTRIBUTES[]; 
	static VertexLayout  LAYOUT; 

};

//-----------------------------------------------------------------------------------------------
struct StrawberryVertex
{
public:
	StrawberryVertex(const Vector3& thePosition, const Rgba& theColor, const Vector2& UVcoords, int textureIndex);
	StrawberryVertex() {} // for array

public:
	Vector3		m_position;
	Rgba		m_color;
	Vector2		m_uvTexCoords;
	float		m_textureID;

	static const VertexAttributeT ATTRIBUTES[];
	static VertexLayout  LAYOUT;
};

//-----------------------------------------------------------------------------------------------
struct TimeDataT 
{
	float gameDeltaTime; 
	float gameTime; 
	float systemDeltaTime; 
	float systemTime;
};
#define TIME_BUFFER_BINDING   (1)

//-----------------------------------------------------------------------------------------------
struct CameraMatrixT
{
	Matrix44 view;
	Matrix44 projection;
	Matrix44 viewProjection;
	
	Matrix44 inverseView; // camera matrix
	Matrix44 inverseProjection;
	Matrix44 inverseViewProjection;
	
	Vector3 cameraRight;			float pad0;
	Vector3 cameraUp;				float pad1;
	Vector3 cameraForward;			float pad2;
	Vector3 cameraPosition;			float pad3;

};
#define CAMERA_BUFFER_BINDING (2)

//-----------------------------------------------------------------------------------------------
struct ModelT
{
	Matrix44 model;
};
#define MODEL_BUFFER_BINDING (3)

//====================================================================================
// Classes
//====================================================================================


//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================


