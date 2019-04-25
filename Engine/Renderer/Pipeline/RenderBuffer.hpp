#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/GLFunctions.hpp"

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
class RenderBuffer
{
public:
	RenderBuffer();   // initialize data
	~RenderBuffer();  // cleanup OpenGL resource 

	// copies data to the GPU
	bool CopyToGPU( size_t const byteCount, void const *data ); 

public:
	GLuint m_handle;       // OpenGL handle to the GPU buffer, defualt = NULL; 
	size_t m_bufferSize;  // how many bytes are in this buffer, default = 0
};
typedef RenderBuffer				UniformBuffer;

//-----------------------------------------------------------------------------------------------
class VertexBuffer : public RenderBuffer
{
public:
	VertexBuffer() {}
	VertexBuffer(uint vertexCount, uint vertexStride);

public:
	// meta
	uint m_vertexCount;  
	uint m_vertexStride; 
};

//-----------------------------------------------------------------------------------------------
class IndexBuffer : public RenderBuffer
{
public:
	IndexBuffer() {}
	IndexBuffer(uint indexCount, uint indexStride);

public:
	uint m_indexCount; 
	uint m_indexStride; 
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