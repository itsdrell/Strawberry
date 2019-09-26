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

