#pragma once
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/RenderTypes.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class MeshBuilder;
enum PrimitiveType;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct DrawInstruction
{
	bool usingIndices;
	uint startIndex;
	uint elemCount;

	PrimitiveType primitiveType;
};

//====================================================================================
// Classes
//====================================================================================
class Mesh
{
public: 
	Mesh() {}

public:
	// definition in cpp
	template< typename VERTTYPE >
	void FromBuilderForType(MeshBuilder &mb);

public:
	void SetIndices(uint count, uint const *indices);
	void SetDrawInstruction(PrimitiveType type, bool usesIndices, uint startIndex, uint elementCount);
	void SetDrawInstruction(DrawInstruction theDraw);

public:
	uint GetVertexStride() const { return m_layout->m_stride; }

public:
	//-----------------------------------------------------------------------------------------------
	template <typename VERTTYPE>
	void SetVertices(uint vcount, VERTTYPE const *vertices)
	{
		m_vbo.CopyToGPU(vcount * sizeof(VERTTYPE), vertices);
		m_layout = &VERTTYPE::LAYOUT;
		// this is a form of duck-typing.  Works for any class that has a 
		// LAYOUT defined that is a VertexLayout;
	}

public:
	VertexBuffer 			m_vbo;
	IndexBuffer 			m_ibo;
	DrawInstruction 		m_drawInstruction;

	VertexLayout*			m_layout; // describes the vertices in the mesh
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/25/2019]
//====================================================================================