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
template< typename VERTTYPE >
void Mesh::FromBuilderForType(MeshBuilder &mb)
{
	uint vcount = (uint)mb.m_vertices.size();
	VERTTYPE *temp = (VERTTYPE*)malloc(sizeof(VERTTYPE) * vcount);

	for (uint i = 0; i < vcount; ++i)
	{
		// copy each vertex
		temp[i] = VERTTYPE(mb.GetVertex(i));
	}

	//SetVertices<VERTTYPE>( vcount, temp );
	SetVertices(vcount, temp);

	// update indices as normal;
	if (mb.m_draw.usingIndices)
		SetIndices((uint)mb.m_indices.size(), mb.m_indices.data());


	SetDrawInstruction(mb.m_draw);

	// free our temp buffer
	free(temp);
}

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/25/2019]
//====================================================================================