#include "Mesh.hpp"
#include "Engine/Renderer/Systems/MeshBuilder.hpp"

//-----------------------------------------------------------------------------------------------
void Mesh::SetIndices(uint count, uint const * indices)
{
	m_ibo.m_indexCount = count;
	m_ibo.m_indexStride = sizeof(uint);

	uint size = (m_ibo.m_indexCount * m_ibo.m_indexStride);
	
	m_ibo.CopyToGPU(size,indices);
}

//-----------------------------------------------------------------------------------------------
void Mesh::SetDrawInstruction(PrimitiveType type, bool usesIndices, uint startIndex, uint elementCount)
{
	m_drawInstruction.primitiveType = type;
	m_drawInstruction.elemCount = elementCount;
	m_drawInstruction.startIndex = startIndex;
	m_drawInstruction.usingIndices = usesIndices;
}

//-----------------------------------------------------------------------------------------------
void Mesh::SetDrawInstruction(DrawInstruction theDraw)
{
	m_drawInstruction = theDraw;
}

//===============================================================================================
// This had to be declared in the cpp because emscripten
//===============================================================================================
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

// Add one per type of vert we use
template void Mesh::FromBuilderForType<Vertex3D_PCU>(MeshBuilder &mb);