#include "MeshBuilder.hpp"
#include "Engine/Renderer/Components/Mesh.hpp"
#include "Engine\Math\MathUtils.hpp"
#include "Engine/Renderer/Images/Sprite.hpp"
#include "Engine/Math/Vectors/Vector3.hpp"


//===============================================================================================
MeshBuilder::MeshBuilder()
{
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::Begin(PrimitiveType theType, bool useIndices)
{
	m_draw.primitiveType = theType;
	m_draw.usingIndices = useIndices;

	if (useIndices) {
		m_draw.startIndex = (uint) m_indices.size(); 
	} else {
		m_draw.startIndex = (uint) m_vertices.size();
	}
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::End()
{
	uint end_idx;

	if (m_draw.usingIndices) {
		end_idx = (uint) m_indices.size(); 
	} else {
		end_idx = (uint) m_vertices.size(); 
	}

	m_draw.elemCount = end_idx - m_draw.startIndex; 
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::Clear()
{
	m_vertices.clear();
	m_indices.clear();
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::ReserveSpace(int size)
{
	m_vertices.reserve((size_t)size);
	m_indices.reserve((size_t)size);
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::SetColor(Rgba const & c)
{
	m_stamp.m_color = c;
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::SetUV(Vector2 const & uv)
{
	m_stamp.m_uvTexCoords = uv;
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::SetUV(float x, float y)
{
	m_stamp.m_uvTexCoords = Vector2(x,y);
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::SetNormal(const Vector3 & theNormal)
{
	m_stamp.m_normal = theNormal;
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::SetTangents(const Vector3 & theTangents)
{
	m_stamp.m_tangent = theTangents;
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::SetBitangents(const Vector3 & theBitangents)
{
	m_stamp.m_biTangent = theBitangents;
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::SetNormalAndTangents(const Vector3& theNormal)
{
	// This only works for sphere, do not use for 
	
	m_stamp.m_normal = theNormal;

	// Creates and sets the stamp
	//GenerateArbitraryTangents(&m_stamp.m_tangent, &m_stamp.m_biTangent, theNormal);
}

//-----------------------------------------------------------------------------------------------
uint MeshBuilder::PushVertex(const Vector3& position)
{
	m_stamp.m_position = position;
	m_vertices.emplace_back(m_stamp);

	return (uint) m_vertices.size() -1;
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::ChangeUVOfVertexAtPosition(uint pos, const Vector2& uvs)
{
	m_vertices.at(pos).m_uvTexCoords = uvs;
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::ChangeColorOfVertexAtPosition(uint pos, const Rgba& color)
{
	m_vertices.at(pos).m_color = color;
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AddFace(uint a, uint b, uint c)
{
	m_indices.emplace_back(a);
	m_indices.emplace_back(b);
	m_indices.emplace_back(c);
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AddQuad(uint a, uint b, uint c, uint d)
{
	m_indices.emplace_back(a);
	m_indices.emplace_back(b);
	m_indices.emplace_back(c);

	m_indices.emplace_back(a);
	m_indices.emplace_back(c);
	m_indices.emplace_back(d);
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AppendPoint(const Vector3& pos, const Rgba& color /*= Rgba::WHITE*/)
{
	SetColor(color);
	PushVertex(pos);
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AppendLine(const Vector3& startPos, const Vector3& endPos, const Rgba& color /*= Rgba::WHITE*/)
{
	SetColor(color);
	PushVertex(startPos);

	SetColor(color);
	PushVertex(endPos);
}

//===============================================================================================
void MeshBuilder::AddPoint(const Vector3& pos, const Rgba& color /*= Rgba::WHITE*/)
{
	Begin(PRIMITIVE_POINTS, false);

	SetColor(color);
	PushVertex(pos);

	End();
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AddPoints(const std::vector<Vector3>& positions, const Rgba& color /*= Rgba::WHITE*/)
{
	Begin(PRIMITIVE_POINTS, false);

	for(uint i = 0; i < positions.size(); i++)
	{
		AppendPoint(positions.at(i), color);
	}

	End();
}

void MeshBuilder::AddDottedLines(const std::vector<Vector3>& positions, const Rgba& color /*= Rgba::WHITE*/)
{
	Begin(PRIMITIVE_LINES, false);

	for (uint i = 0; i < positions.size(); i++)
	{
		AppendPoint(positions.at(i), color);
	}

	End();
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AddLines(const std::vector<Vector3>& positions, const Rgba& color /*= Rgba::WHITE*/)
{
	Begin(PRIMITIVE_LINES, false);

	for (uint i = 0; i < positions.size() - 1; i++)
	{
		AppendLine(positions.at(i),positions.at(i + 1), color);
	}

	End();
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AddPlane(const Vector3& center, const Vector3& dimensions, const AABB2& theUvs, Rgba color)
{
	AABB2 uvs = theUvs;

	Begin(PRIMITIVE_TRIANGLES, true); // true means you also need to push indices
	SetColor(color);

	SetNormal(-Vector3::FORWARD);
	SetTangents(Vector3::RIGHT);
	SetBitangents(Vector3::UP);

	SetUV(uvs.mins);
	uint idx = PushVertex(Vector3((center.x - dimensions.x), (center.y - dimensions.y),(center.z - dimensions.z)));

	SetUV(uvs.maxs.x, uvs.mins.y);
	PushVertex(Vector3((center.x + dimensions.x), (center.y - dimensions.y), (center.z - dimensions.z)));

	SetUV(uvs.mins.x,uvs.maxs.y);
	PushVertex(Vector3((center.x - dimensions.x), (center.y + dimensions.y),(center.z - dimensions.z)));

	SetUV(uvs.maxs);
	PushVertex(Vector3((center.x + dimensions.x), (center.y + dimensions.y),(center.z - dimensions.z)));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 1, idx + 3);

	End();
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AddPlane(const Vector3& center, const Vector3& dimensions, Vector3 right /*= Vector3::RIGHT*/, Vector3 up /*= Vector3::UP*/, Rgba color /*= Rgba::WHITE*/)
{
	//////////////////////////////////////////////////////////////////////////

	Begin(PRIMITIVE_TRIANGLES, true); // true means you also need to push indices

	SetColor(color);

	Vector3 r = right * .5f;
	Vector3 u = up * .5f;

	SetUV(0,0);
	SetNormal(-Vector3::FORWARD);
	SetTangents(Vector3::RIGHT);
	SetBitangents(Vector3::UP);

	//uint idx = PushVertex(Vector3((center.x - dimensions.x), (center.y - dimensions.y),(center.z - dimensions.z)) + -r  -u);
	uint idx = PushVertex(center + (-r * dimensions.x) + (-u * dimensions.y));

	SetUV(1,0);
	//PushVertex(Vector3((center.x + dimensions.x), (center.y - dimensions.y), (center.z - dimensions.z)) + r + -u);
	PushVertex(center + (r * dimensions.x) + (-u * dimensions.y));
	
	SetUV(0,1);
	//PushVertex(Vector3((center.x - dimensions.x), (center.y + dimensions.y),(center.z - dimensions.z)) + -r + u);
	PushVertex(center + (-r * dimensions.x) + (u * dimensions.y));
	
	SetUV(1,1);
	//PushVertex(Vector3((center.x + dimensions.x), (center.y + dimensions.y),(center.z - dimensions.z)) + r + u);
	PushVertex(center + (r * dimensions.x) + (u * dimensions.y));
	
	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 1, idx + 3);

	End();
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AddQuad(const Vector3& position, const AABB2& bounds, const AABB2& theUvs)
{
	AABB2 uvs = theUvs;

	Begin(PRIMITIVE_TRIANGLES, true); // true means you also need to push indices
	SetColor(Rgba::WHITE);

	SetUV(uvs.mins);
	uint idx = PushVertex(Vector3((position.x - bounds.mins.x), (position.y - bounds.mins.y), (position.z)));

	SetUV(uvs.maxs.x, uvs.mins.y);
	PushVertex(Vector3((position.x + bounds.maxs.x), (position.y - bounds.mins.y), (position.z)));

	SetUV(uvs.mins.x,uvs.maxs.y);
	PushVertex(Vector3((position.x - bounds.mins.x), (position.y + bounds.maxs.y), (position.z)));

	SetUV(uvs.maxs);
	PushVertex(Vector3((position.x + bounds.maxs.x), (position.y + bounds.maxs.y), (position.z)));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 1, idx + 3);
	
	End();
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AddPlaneFromFourPoints(const Vector3& bl, const Vector3& br, const Vector3& tr, const Vector3& tl)
{
	Begin(PRIMITIVE_TRIANGLES, true); // true means you also need to push indices
	SetColor(Rgba::WHITE);

	SetUV(0,0);
	uint idx = PushVertex(bl);

	SetUV(1,0);
	PushVertex(br);

	SetUV(1,1);
	PushVertex(tr);

	SetUV(0,1);
	PushVertex(tl);

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 3, idx);

	End();
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::AddFromSprite(const Vector2& pos, const Sprite& theSprite)
{
	Begin(PRIMITIVE_TRIANGLES, true); // true means you also need to push indices
	SetColor(Rgba::WHITE);

	AABB2 uvs = theSprite.m_uvs;
	
	// this may be wrong
	Vector2 dims = theSprite.m_dimensions;
	AABB2 quad = AABB2(pos - (dims * .5f), pos + (dims * .5f)); 
	
	SetUV(uvs.mins);
	uint idx = PushVertex(Vector3(quad.mins.x, quad.mins.y, 0.f));

	SetUV(uvs.maxs.x, uvs.mins.y);
	PushVertex(Vector3(quad.maxs.x, quad.mins.y, 0.f));

	SetUV(uvs.mins.x, uvs.maxs.y);
	PushVertex(Vector3(quad.mins.x, quad.maxs.y, 0.f));

	SetUV(uvs.maxs);
	PushVertex(Vector3(quad.maxs.x, quad.maxs.y, 0.f));

	AddFace(idx + 0, idx + 1, idx + 3);
	AddFace(idx + 0, idx + 3, idx + 2);

	End();
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::Add2DPlane(const AABB2& bounds, Rgba color /*= Rgba::WHITE*/)
{
	Begin(PRIMITIVE_TRIANGLES, true); // true means you also need to push indices

	SetColor(color);

	SetUV(0,0);
	uint idx = PushVertex(Vector3(bounds.mins.x, bounds.mins.y, .01f));

	SetUV(1,0);
	PushVertex(Vector3(bounds.maxs.x, bounds.mins.y, .01f));

	SetUV(0,1);
	PushVertex(Vector3(bounds.mins.x, bounds.maxs.y, .01f));

	SetUV(1,1);
	PushVertex(Vector3(bounds.maxs.x, bounds.maxs.y, .01f));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 1, idx + 3);

	End();
}

//-----------------------------------------------------------------------------------------------
void MeshBuilder::Add2DPlane(const AABB2& bounds, const AABB2& uvs, const Rgba& color)
{
	Begin(PRIMITIVE_TRIANGLES, true); // true means you also need to push indices

	SetUV(uvs.mins);
	SetColor(color);
	uint idx = PushVertex(Vector3(bounds.mins.x, bounds.mins.y, .01f));

	SetUV(Vector2(uvs.maxs.x, uvs.mins.y));
	SetColor(color);
	PushVertex(Vector3(bounds.maxs.x, bounds.mins.y, .01f));

	SetUV(Vector2(uvs.mins.x, uvs.maxs.y));
	SetColor(color);

	PushVertex(Vector3(bounds.mins.x, bounds.maxs.y, .01f));

	SetUV(uvs.maxs);
	SetColor(color);
	PushVertex(Vector3(bounds.maxs.x, bounds.maxs.y, .01f));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 1, idx + 3);

	End();
}