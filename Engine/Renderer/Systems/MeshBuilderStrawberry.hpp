#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/RenderTypes.hpp"
#include "Engine/Renderer/Components/Mesh.hpp"


//====================================================================================
// Forward Declare
//====================================================================================
class Rgba;
class Vector2;
class Vector3;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================
enum BoundTextureIDs
{
	BOUND_TEXTURE_DEFAULT = 0,
	BOUND_TEXTURE_FONT,
	BOUND_TEXTURE_SPRITE_SHEET_0
};

//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class StrawberryMeshBuilder
{
public:
	StrawberryMeshBuilder();

public:
	void Begin();
	void End();
	void Clear();
	void ReserveSpace(int size);

private:
	void SetColor(const Rgba& color);
	void SetUV(const Vector2& uv);
	void SetUV(float x, float y);
	void SetTextureID(uint id);

private:
	uint PushVertex(const Vector3& position);
	void AddFace(uint a, uint b, uint c);
	void AddQuad(uint a, uint b, uint c, uint d);

public:
	Mesh* CreateMesh(bool flush = true);

public:
	// AABB2, Circle, Text, Line, Sprite
	void AppendLine(const Vector2& start, const Vector2& end, const Rgba& color = Rgba(0, 0, 0, 255), float lineWidth = 1.f);

	void AppendCircleOutline2D(const Vector2& center, float radius, const Rgba& color = Rgba(255, 255, 255, 255), float lineWidth = 1.f, int numberOfEdges = 100);
	void AppendCircleFilled2D(const Vector2& center, float radius, const Rgba& color = Rgba(255, 255, 255, 255), int numberOfEdges = 100);

	void AppendAABB2Outline(const AABB2& bounds, Rgba color = Rgba::WHITE, float lineWidth = 1.f);
	void AppendAABB2Filled(const AABB2& bounds, Rgba color = Rgba::WHITE);

	void AppendSprite(const Vector3& position, const Sprite& theSprite,
		float rotation = 0.f, bool flipX = false, bool flipY = false);

	void AppendTextInBox();

public:
	StrawberryVertex 					m_stamp; // this is a value. We overwrite it
	std::vector<StrawberryVertex>		m_vertices;
	std::vector<uint>					m_indices;

	// Values used to create the mesh at the end
	DrawInstruction						m_draw;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================
extern StrawberryMeshBuilder g_theMeshBuilder;

//====================================================================================
// Written by Zachary Bracken : [6/27/2020]
//====================================================================================