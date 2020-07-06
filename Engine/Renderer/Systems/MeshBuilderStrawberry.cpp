#include "MeshBuilderStrawberry.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "../Images/Sprite.hpp"

//===============================================================================================
StrawberryMeshBuilder g_theMeshBuilder;


//===============================================================================================
StrawberryMeshBuilder::StrawberryMeshBuilder()
{
	Begin();
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::Begin()
{
	m_draw.primitiveType = PRIMITIVE_TRIANGLES;
	m_draw.usingIndices = true;

	m_draw.startIndex = (uint)m_indices.size();
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::End()
{
	uint end_idx = (uint)m_indices.size();
	m_draw.elemCount = end_idx - m_draw.startIndex;
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::Clear()
{
	m_vertices.clear();
	m_indices.clear();

	Begin();
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::ReserveSpace(int size)
{
	m_vertices.reserve((size_t)size);
	m_indices.reserve((size_t)size);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::SetColor(const Rgba& color)
{
	m_stamp.m_color = color;
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::SetUV(const Vector2& uv)
{
	m_stamp.m_uvTexCoords = uv;
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::SetUV(float x, float y)
{
	m_stamp.m_uvTexCoords = Vector2(x, y);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::SetTextureID(uint id)
{
	m_stamp.m_textureID = id;
}

//-----------------------------------------------------------------------------------------------
uint StrawberryMeshBuilder::PushVertex(const Vector3& position)
{
	m_stamp.m_position = position;
	m_vertices.emplace_back(m_stamp);

	return (uint)m_vertices.size() - 1;
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AddFace(uint a, uint b, uint c)
{
	m_indices.emplace_back(a);
	m_indices.emplace_back(b);
	m_indices.emplace_back(c);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AddQuad(uint a, uint b, uint c, uint d)
{
	m_indices.emplace_back(a);
	m_indices.emplace_back(b);
	m_indices.emplace_back(c);

	m_indices.emplace_back(a);
	m_indices.emplace_back(c);
	m_indices.emplace_back(d);
}

//-----------------------------------------------------------------------------------------------
Mesh* StrawberryMeshBuilder::CreateMesh(bool flush /*= true*/)
{
	Mesh *mesh = new Mesh();

	uint vcount = (uint) m_vertices.size();
	StrawberryVertex *temp = (StrawberryVertex*) malloc(sizeof(StrawberryVertex) * vcount);

	for (uint i = 0; i < vcount; ++i)
	{
		// copy each vertex
		temp[i] = m_vertices.at(i);
	}

	mesh->SetVertices<StrawberryVertex>(vcount, temp);

	// update indices as normal;
	if (m_draw.usingIndices)
		mesh->SetIndices((uint)m_indices.size(), m_indices.data());

	mesh->SetDrawInstruction(m_draw);

	// free our temp buffer
	free(temp);

	// Flush
	if (flush)
	{
		m_vertices.clear();
		m_indices.clear();
	}

	return mesh;
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendLine(const Vector2& start, const Vector2& end, const Rgba& color /*= Rgba(0, 0, 0, 255)*/, float lineWidth)
{
	float halfWidth = lineWidth * .5f;
	
	Vector2 displacement = end - start;
	Vector2 dir = displacement.GetNormalized();
	Vector2 cross = Vector2(-dir.y, dir.x);
	Vector2 step = cross * halfWidth;
	
	Vector2 br = start + (step);
	Vector2 bl = start - (step);
	Vector2 tr = end + step;
	Vector2 tl = end - step;

	SetColor(color);
	SetTextureID(BOUND_TEXTURE_DEFAULT);

	// this ordering may be wrong
	SetUV(0, 0);
	uint idx = PushVertex(Vector3(br.x, br.y, 0.f));

	SetUV(1, 0);
	PushVertex(Vector3(bl.x, bl.y, 0.f));

	SetUV(0, 1);
	PushVertex(Vector3(tr.x, tr.y, 0.f));

	SetUV(1, 1);
	PushVertex(Vector3(tl.x, tl.y, 0.f));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 1, idx + 3);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendCircleOutline2D(const Vector2& center, float radius, const Rgba& color /*= Rgba(255, 255, 255, 255)*/, float lineWidth, int numberOfEdges /*= 100*/)
{
	float distanceInDegrees = 360.f / ((float)numberOfEdges);
	float degrees = 0.f;
	float halfWidth = lineWidth * .5f;

	SetColor(color);
	SetTextureID(BOUND_TEXTURE_DEFAULT);

	float previousPointX = center.x + (radius * (CosDegrees(degrees)));
	float previousPointY = center.y + (radius * (SinDegrees(degrees)));
	for (int i = 1; i < numberOfEdges + 1; i++)
	{
		// Increase degrees so that we can find the next point
		degrees += distanceInDegrees;

		// End point
		float nextPointX = center.x + (radius * (CosDegrees(degrees)));
		float nextPointY = center.y + (radius * (SinDegrees(degrees)));

		Vector2 previousPoint = Vector2(previousPointX, previousPointY);
		Vector2 nextPoint = Vector2(nextPointX, nextPointY);

		Vector2 displacement = previousPoint - center;
		Vector2 dir = displacement.GetNormalized();
		Vector2 step = dir * halfWidth;

		Vector2 NextDisplacement = nextPoint - center;
		Vector2 NextDir = NextDisplacement.GetNormalized();
		Vector2 NextStep = NextDir * halfWidth;

		Vector2 br = previousPoint + (step);
		Vector2 bl = previousPoint - (step);
		Vector2 tr = nextPoint + NextStep;
		Vector2 tl = nextPoint - NextStep;

		SetColor(color);
		SetTextureID(Renderer::GetInstance()->m_defaultTexture->GetID());

		SetUV(0, 0);
		uint idx = PushVertex(Vector3(bl.x, bl.y, 0.f));

		SetUV(1, 0);
		PushVertex(Vector3(br.x, br.y, 0.f));

		SetUV(0, 1);
		PushVertex(Vector3(tl.x, tl.y, 0.f));

		SetUV(1, 1);
		PushVertex(Vector3(tr.x, tr.y, 0.f));

		AddFace(idx + 0, idx + 1, idx + 2);
		AddFace(idx + 2, idx + 1, idx + 3);

		previousPointX = nextPointX;
		previousPointY = nextPointY;
	}
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendCircleFilled2D(const Vector2& center, float radius, const Rgba& color /*= Rgba(255, 255, 255, 255)*/, int numberOfEdges /*= 60*/)
{
	float distanceInDegrees = 360.f / ((float)numberOfEdges);
	float degrees = 0.f;

	SetColor(color);
	SetTextureID(BOUND_TEXTURE_DEFAULT);

	float previousPointX = center.x + (radius * (CosDegrees(degrees)));
	float previousPointY = center.y + (radius * (SinDegrees(degrees)));
	for (int i = 1; i < numberOfEdges + 1; i++)
	{
		// Increase degrees so that we can find the next point
		degrees += distanceInDegrees;

		// End point
		float nextPointX = center.x + (radius * (CosDegrees(degrees)));
		float nextPointY = center.y + (radius * (SinDegrees(degrees)));

		SetUV(0, 0);
		uint idx = PushVertex(Vector3(center.x, center.y, 0.f));

		SetUV(0, 0);
		PushVertex(Vector3(previousPointX, previousPointY, .0f));

		SetUV(0, 0);
		PushVertex(Vector3(nextPointX, nextPointY, .0f));

		AddFace(idx + 0, idx + 1, idx + 2);

		previousPointX = nextPointX;
		previousPointY = nextPointY;
	}
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendAABB2Outline(const AABB2& bounds, Rgba color /*= Rgba::WHITE*/, float lineWidth)
{
	SetColor(color);
	SetTextureID(BOUND_TEXTURE_DEFAULT);
	
	float stepSize = (lineWidth * .5f) * sqrt(2);
	
	Vector2 tlDir = Vector2(bounds.mins.x, bounds.maxs.y) - Vector2(bounds.maxs.x, bounds.mins.y);
	tlDir = tlDir.GetNormalized();

	Vector2 blDir = bounds.mins - bounds.maxs;
	blDir = blDir.GetNormalized();

	Vector2 trDir = blDir * -1.f;
	Vector2 brDir = tlDir * -1.f;

	Vector2 tlPos = Vector2(bounds.mins.x, bounds.maxs.y) + (tlDir * stepSize);
	Vector2 blPos = bounds.mins + (blDir * stepSize);
	Vector2 trPos = bounds.maxs + (trDir * stepSize);
	Vector2 brPos = Vector2(bounds.maxs.x, bounds.mins.y) + (brDir * stepSize);

	Vector2 insideTlPos = Vector2(bounds.mins.x, bounds.maxs.y) + (brDir * stepSize);
	Vector2 insideBlPos = bounds.mins + (trDir * stepSize);
	Vector2 insideBrPos = Vector2(bounds.maxs.x, bounds.mins.y) + (tlDir * stepSize);
	Vector2 insideTrPos = bounds.maxs + (blDir * stepSize);

	//-----------------------------------------------------------------------------------------------
	// left
	SetUV(0, 0);
	uint idx = PushVertex(Vector3(insideTlPos.x, insideTlPos.y, .01f));

	SetUV(1, 0);
	PushVertex(Vector3(tlPos.x, tlPos.y, .01f));

	SetUV(0, 1);
	PushVertex(Vector3(blPos.x, blPos.y, .01f));

	SetUV(1, 1);
	PushVertex(Vector3(insideBlPos.x, insideBlPos.y, .01f));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 3, idx + 0);

	//-----------------------------------------------------------------------------------------------
	// bottom
	SetUV(0, 0);
	idx = PushVertex(Vector3(insideBlPos.x, insideBlPos.y, .01f));

	SetUV(1, 0);
	PushVertex(Vector3(blPos.x, blPos.y, .01f));

	SetUV(0, 1);
	PushVertex(Vector3(brPos.x, brPos.y, .01f));

	SetUV(1, 1);
	PushVertex(Vector3(insideBrPos.x, insideBrPos.y, .01f));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 3, idx + 0);

	//-----------------------------------------------------------------------------------------------
	// right
	SetUV(0, 0);
	idx = PushVertex(Vector3(insideBrPos.x, insideBrPos.y, .01f));

	SetUV(1, 0);
	PushVertex(Vector3(brPos.x, brPos.y, .01f));

	SetUV(0, 1);
	PushVertex(Vector3(trPos.x, trPos.y, .01f));

	SetUV(1, 1);
	PushVertex(Vector3(insideTrPos.x, insideTrPos.y, .01f));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 3, idx + 0);

	//-----------------------------------------------------------------------------------------------
	// top
	SetUV(0, 0);
	idx = PushVertex(Vector3(insideTrPos.x, insideTrPos.y, .01f));

	SetUV(1, 0);
	PushVertex(Vector3(trPos.x, trPos.y, .01f));

	SetUV(0, 1);
	PushVertex(Vector3(tlPos.x, tlPos.y, .01f));

	SetUV(1, 1);
	PushVertex(Vector3(insideTlPos.x, insideTlPos.y, .01f));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 3, idx + 0);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendAABB2Filled(const AABB2& bounds, Rgba color /*= Rgba::WHITE*/)
{
	SetColor(color);
	SetTextureID(BOUND_TEXTURE_DEFAULT);

	SetUV(0, 0);
	uint idx = PushVertex(Vector3(bounds.mins.x, bounds.mins.y, .01f));

	SetUV(1, 0);
	PushVertex(Vector3(bounds.maxs.x, bounds.mins.y, .01f));

	SetUV(0, 1);
	PushVertex(Vector3(bounds.mins.x, bounds.maxs.y, .01f));

	SetUV(1, 1);
	PushVertex(Vector3(bounds.maxs.x, bounds.maxs.y, .01f));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 1, idx + 3);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendSprite(const Vector3& position, const Sprite& theSprite, float rotation /*= 0.f*/, bool flipX /*= false*/, bool flipY /*= false*/)
{
	Matrix44 theRotationMatrix = Matrix44::MakeRotationDegrees2D(rotation);
	Vector3 right = theRotationMatrix.GetRight();
	Vector3 up = theRotationMatrix.GetUp();

	// this needs to be passed in
	SetTextureID(1);
	SetColor(Rgba::WHITE);

	// calculating offsets from the pivot point
	float leftOffset = -1.0f * ((theSprite.m_pivot.x) * theSprite.m_dimensions.x);
	float rightOffset = leftOffset + theSprite.m_dimensions.x;
	float bottomOffset = -1.0f * ((theSprite.m_pivot.y) * theSprite.m_dimensions.y);
	float topOffset = bottomOffset + theSprite.m_dimensions.y;

	// calculating the vertex points 
	Vector3 p0 = position + (right * leftOffset) + (up * bottomOffset); // bottom left
	Vector3 p1 = position + (right * rightOffset) + (up * bottomOffset); // bottom right
	Vector3 p2 = position + (right * rightOffset) + (up * topOffset); // top right
	Vector3 p3 = position + (right * leftOffset) + (up * topOffset); // top left

	// Get the UVs
	AABB2 uvs = theSprite.m_uvs;

	if (flipY)
	{
		uvs.mins.y = theSprite.m_uvs.maxs.y;
		uvs.maxs.y = theSprite.m_uvs.mins.y;
	}

	if (flipX)
	{
		uvs.mins.x = theSprite.m_uvs.maxs.x;
		uvs.maxs.x = theSprite.m_uvs.mins.x;
	}

	Vector2 bl = uvs.mins;
	Vector2 br = Vector2(uvs.maxs.x, uvs.mins.y);
	Vector2 tl = Vector2(uvs.mins.x, uvs.maxs.y);
	Vector2 tr = uvs.maxs;

	SetUV(bl);
	uint idx = PushVertex(Vector3(p0.x, p0.y, .01f));

	SetUV(br);
	PushVertex(Vector3(p1.x, p1.y, .01f));

	SetUV(tr);
	PushVertex(Vector3(p2.x, p2.y, .01f));

	SetUV(tl);
	PushVertex(Vector3(p3.x, p3.y, .01f));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 0, idx + 2, idx + 3); // 023 ?
}

