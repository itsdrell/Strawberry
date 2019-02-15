#pragma once
#include "Engine/Renderer/RenderTypes.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class ShaderProgram;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================
enum RenderQueue
{
	// This is in order it needs to be rendered 
	RENDER_QUEUE_OPAQUE,
	RENDER_QUEUE_ALPHA
};
//RenderQueue RenderQueueFromString(std::string theValue);

//====================================================================================
// Structs
//====================================================================================
struct RenderState 
{
	RenderState();

	// Raster State Control
	eCullMode								m_cullMode;			// CULL_BACK
	eFillMode								m_fillMode;			// FILL_SOLID
	eWindOrder								m_frontFace;			// WIND_COUNTER_CLOCKWISE

	// Depth State Control
	DepthBufferComparisons					m_depthCompare;		// COMPARE_LESS
	bool									m_depthWrite;          // true

	// Blends
	// Disabled is: 
	// Color: Compare Add  1, zero. 
	// Alpha: Compare ADD, 1, 1
	eBlendOperation							m_colorBlendOp;		// COMPARE_ADD
	eBlendFactor							m_colorSrcFactor;		// BLEND_ONE
	eBlendFactor							m_colorDstFactor;		// BLEND_ZERO

	eBlendOperation							m_alphaBlendOp;		// COMPARE_ADD
	eBlendFactor							m_alphaSrcFactor;		// BLEND_ONE
	eBlendFactor							m_alphaDstFactor;		// BLEND_ONE
}; 

//====================================================================================
// Classes
//====================================================================================
class Shader
{
public:
	Shader() {}
	Shader( const String& path );

public:
	void SetProgram( ShaderProgram* theProgram); 

	void EnableBlending( eBlendOperation op, eBlendFactor src, eBlendFactor dst ); 
	void DisableBlending();

	void SetDepth( DepthBufferComparisons compare, bool write ); 
	void DisableDepth() { SetDepth( COMPARE_ALWAYS, false ); }

	void SetCullMode( eCullMode cullMode ); 
	void SetFillMode( eFillMode fillMode ); 
	void SetFrontFace( eWindOrder windOrder ); 

public:
	String 				m_name;
	ShaderProgram*		m_program; 
	RenderState			m_state; 

	// SORTING
	int					m_sortOrder;
	RenderQueue			m_renderQueue;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/31/2019]
//====================================================================================