#pragma once
#include <vector>
#include "Engine/Renderer/Components/Mesh.hpp"
#include "Engine\Math\Geometry/AABB2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class Sprite;

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
class MeshBuilder
{
public:
	MeshBuilder();

public:
	void Begin(PrimitiveType theType, bool useIndices);
	void End();
	void Clear();

	void ReserveSpace(int size);

public:
	void SetColor(const Rgba& c);
	void SetUV(const Vector2& uv);
	void SetUV(float x, float y);
	void SetNormal(const Vector3& theNormal);
	void SetTangents(const Vector3& theTangents);
	void SetBitangents(const Vector3& theBitangents);
	void SetNormalAndTangents(const Vector3& theNormal);
	uint PushVertex(const Vector3& position);

	VertexMaster GetVertex(uint index) { return m_vertices.at(index); }

public:
	void ChangeUVOfVertexAtPosition(uint pos, const Vector2& uvs);
	void ChangeColorOfVertexAtPosition(uint pos, const Rgba& color);

public:
	void AddFace(uint a, uint b, uint c);
	void AddQuad(uint a, uint b, uint c, uint d);

	template< typename VERTTYPE >
	Mesh* CreateMesh(bool flush = true);

public:
	void AppendPoint(const Vector3& pos, const Rgba& color = Rgba::WHITE);
	void AppendLine(const Vector3& startPos, const Vector3& endPos, const Rgba& color = Rgba::WHITE);

public:
	void AddPoint(const Vector3& pos, const Rgba& color = Rgba::WHITE);
	void AddPoints(const std::vector<Vector3>& positions, const Rgba& color = Rgba::WHITE);

	void AddDottedLines(const std::vector<Vector3>& positions, const Rgba& color = Rgba::WHITE);
	void AddLines(const std::vector<Vector3>& positions, const Rgba& color = Rgba::WHITE);

	void AddPlane(const Vector3& center, const Vector3& dimensions, const AABB2& theUvs = AABB2(0.f,0.f, 1.f, 1.f), Rgba color = Rgba::WHITE);
	void AddPlane(const Vector3& center, const Vector3& dimensions, Vector3 right, Vector3 up, Rgba color = Rgba::WHITE);

	void AddQuad(const Vector3& position, const AABB2& bounds, const AABB2& theUvs = AABB2(0.f, 0.f, 1.f, 1.f));
	void AddPlaneFromFourPoints(const Vector3& bl, const Vector3& br, const Vector3& tr, const Vector3& tl);
	void AddFromSprite(const Vector2& pos, const Sprite& theSprite);

	void Add2DPlane(const AABB2& bounds, Rgba color = Rgba::WHITE);
	void Add2DPlane(const AABB2& bounds, const AABB2& uvs, const Rgba& color);

public:
	VertexMaster 						m_stamp; // this is a value. We overwrite it
	std::vector<VertexMaster>			m_vertices;
	std::vector<uint>					m_indices;

	// Values used to create the mesh at the end
	DrawInstruction						m_draw;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
template< typename VERTTYPE >
Mesh* MeshBuilder::CreateMesh(bool flush)
{
	Mesh *mesh = new Mesh();

	mesh->FromBuilderForType<VERTTYPE>(*this);

	// Flush
	if (flush)
	{
		m_vertices.clear();
		m_indices.clear();
	}

	return mesh;
}

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/25/2019]
//====================================================================================