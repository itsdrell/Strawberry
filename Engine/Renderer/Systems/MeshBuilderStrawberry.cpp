#include "MeshBuilderStrawberry.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Images/Sprite.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"
#include "Engine/Renderer/Images/BitmapFont.hpp"

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
void StrawberryMeshBuilder::ChangeAttributesForVertexIndex(int vertexIndex, const Vector2& uvs, int spriteSheetIndex /*= 0*/, Rgba color /*= Rgba::WHITE*/)
{
	m_vertices.at(vertexIndex).m_uvTexCoords = uvs;
	m_vertices.at(vertexIndex).m_color = color;
	m_vertices.at(vertexIndex).m_textureID = spriteSheetIndex;
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

	return;
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
void StrawberryMeshBuilder::AppendTexturedAABB2(const AABB2& bounds, const AABB2& uvs, int spriteSheetIndex /*= 0*/, Rgba color /*= Rgba::WHITE*/)
{
	SetColor(color);
	SetTextureID(spriteSheetIndex);

	SetUV(uvs.mins);
	uint idx = PushVertex(Vector3(bounds.mins.x, bounds.mins.y, .01f));

	SetUV(uvs.maxs.x, uvs.mins.y);
	PushVertex(Vector3(bounds.maxs.x, bounds.mins.y, .01f));

	SetUV(uvs.mins.x, uvs.maxs.y);
	PushVertex(Vector3(bounds.mins.x, bounds.maxs.y, .01f));

	SetUV(uvs.maxs);
	PushVertex(Vector3(bounds.maxs.x, bounds.maxs.y, .01f));

	AddFace(idx + 0, idx + 1, idx + 2);
	AddFace(idx + 2, idx + 1, idx + 3);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendSprite(const Vector3& position, const Sprite& theSprite, float rotation /*= 0.f*/, bool flipX /*= false*/, bool flipY /*= false*/, int spriteSheetIndex)
{
	Matrix44 theRotationMatrix = Matrix44::MakeRotationDegrees2D(rotation);
	Vector3 right = theRotationMatrix.GetRight();
	Vector3 up = theRotationMatrix.GetUp();

	// this needs to be passed in
	SetTextureID(spriteSheetIndex + 2);
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

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendTextInBox(const String& text, const AABB2& bounds, float cellHeight, float normalizedPercentIntoText /*= 1.f*/, 
	DrawTextMode mode /*= DRAW_TEXT_MODE_OVERFLOW*/, const Vector2& alignment /*= Vector2(0, 0)*/, const Rgba& color /*= Rgba::WHITE*/, bool drawOutline /*= false*/, 
	const Rgba& outlineColor /*= Rgba::BLACK*/, float aspect /*= 1.77f*/)
{
	String textToShow = GetPercentIntoString(text, normalizedPercentIntoText);

	switch (mode)
	{
	case DRAW_TEXT_MODE_WRAPPED:
		AppendWrappedTextInBox2D(textToShow, bounds, cellHeight, alignment, aspect, color, drawOutline, outlineColor);
		break;
	case DRAW_TEXT_MODE_SHRINKED:
		AppendShrinkToFitTextInBox2D(textToShow, bounds, cellHeight, alignment, aspect, color, drawOutline, outlineColor);
		break;
	case DRAW_TEXT_MODE_OVERFLOW:
		AppendOverflowTextInBox2D(textToShow, bounds, cellHeight, alignment, aspect, color, drawOutline, outlineColor);
		break;
	default:
		// error?
		break;
	}
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendWrappedTextInBox2D(const String& text, const AABB2& boxSize, float cellHeight /*= 1.f*/, 
	const Vector2& alignment /*= Vector2(0, 0)*/, float aspectScale /*= 1.77f*/, const Rgba& textColor /*= Rgba::WHITE*/, 
	bool drawOutline /*= false*/, const Rgba& outlineColor /*= Rgba::BLACK*/)
{
	// make a new box that has a little bit of paddin
	// so the text is never ON the box
	float offset = C_TEXT_PADDING * cellHeight;
	AABB2 newBox = boxSize;
	newBox.mins += Vector2(offset, offset);
	newBox.maxs -= Vector2(offset, offset);
	//DrawAABB2Outline(newBox, Rgba::GetRandomColor());

	Strings vectorOfWords = SplitString(text, " ");

	Strings lines;
	Renderer::GetInstance()->m_defaultFont->GetTextWrapped(vectorOfWords, boxSize, cellHeight, &lines, aspectScale);

	AppendTextWithAlignment(lines, newBox, cellHeight, alignment, aspectScale, textColor, drawOutline, outlineColor);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendShrinkToFitTextInBox2D(const String& text, const AABB2& boxSize, float cellHeight /*= 1.f*/, 
	const Vector2& alignment /*= Vector2(0, 0)*/, float aspectScale /*= 1.77f*/, const Rgba& textColor /*= Rgba::WHITE*/, 
	bool drawOutline /*= false*/, const Rgba& outlineColor /*= Rgba::BLACK*/)
{
	// this does new line if the \n is provided, no auto wrapping
	Strings lines = SplitString(text, "\n");

	for (uint i = 0; i < lines.size(); i++)
	{
		String* current = &lines.at(i);
		RemoveLeadingAndEndingWhitespace(current);
	}

	float newCellHeight = Renderer::GetInstance()->m_defaultFont->GetFontSizeToFitInBox(lines, boxSize, cellHeight, aspectScale);

	// make a new box that has a little bit of padding based on the cellheight 
	// so the text is never ON the box
	float offset = C_TEXT_PADDING * newCellHeight;
	AABB2 newBox = boxSize;
	newBox.mins += Vector2(offset, offset);
	newBox.maxs -= Vector2(offset, offset);

	AppendTextWithAlignment(lines, newBox, newCellHeight, alignment, aspectScale, textColor, drawOutline, outlineColor);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendOverflowTextInBox2D(const String& text, const AABB2& boxSize, float cellHeight /*= 1.f*/, 
	const Vector2& alignment /*= Vector2(0, 0)*/, float aspectScale /*= 1.77f*/, const Rgba& textColor /*= Rgba::WHITE*/, 
	bool drawOutline /*= false*/, const Rgba& outlineColor /*= Rgba::BLACK*/)
{
	Strings theLines = Strings({ text });
	AppendTextWithAlignment(theLines, boxSize, cellHeight, alignment, aspectScale, textColor, drawOutline, outlineColor);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendTextWithAlignment(const Strings& text, const AABB2& boxSize, float cellHeight /*= 1.f*/, 
	const Vector2& alignment /*= Vector2(0, 0)*/, float aspectScale /*= 1.77f*/, const Rgba& textColor /*= Rgba::WHITE*/, 
	bool drawOutline /*= false*/, const Rgba& outlineColor /*= Rgba::BLACK*/)
{
	float lineHeight = .5f; // can expose this if you want
	float lineSpacing = (cellHeight * lineHeight);
	float heightOfText = Renderer::GetInstance()->m_defaultFont->GetHeightOfStrings(text, cellHeight, lineHeight);

	float offsetFromTop = (boxSize.GetHeight() - heightOfText) * alignment.y;
	float startingY = boxSize.maxs.y - offsetFromTop - cellHeight; // minus cell height since we draw at the bottom of the box

	float currentY = startingY;
	for (uint i = 0; i < text.size(); i++)
	{
		String currentString = text.at(i);

		float width = Renderer::GetInstance()->m_defaultFont->GetStringWidth(currentString, cellHeight, aspectScale);
		float offsetFromLeft = (boxSize.GetWidth() - width) * alignment.x;
		float xPos = boxSize.mins.x + offsetFromLeft;

		AppendText2D(Vector2(xPos, currentY), currentString, cellHeight, textColor, drawOutline, outlineColor, aspectScale);

		currentY -= (lineSpacing + cellHeight);
	}
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendText2D(const Vector2& startPos, const String& text, float cellHeight, const Rgba& tint /*= Rgba::WHITE*/, 
	bool drawOutline /*= false*/, const Rgba& outlineColor /*= Rgba::BLACK*/, float aspectScale /*= 1.7f*/)
{
	if (drawOutline)
	{
		float sizeDifference = cellHeight * .06f; // maybe make this a param
		AppendText2DVerts(Vector2(startPos.x + sizeDifference, startPos.y), text, cellHeight, outlineColor, aspectScale);
		AppendText2DVerts(Vector2(startPos.x - sizeDifference, startPos.y), text, cellHeight, outlineColor, aspectScale);
		AppendText2DVerts(Vector2(startPos.x, startPos.y + sizeDifference), text, cellHeight, outlineColor, aspectScale);
		AppendText2DVerts(Vector2(startPos.x, startPos.y - sizeDifference), text, cellHeight, outlineColor, aspectScale);
	}

	AppendText2DVerts(Vector2(startPos.x, startPos.y), text, cellHeight, tint, aspectScale);
}

//-----------------------------------------------------------------------------------------------
void StrawberryMeshBuilder::AppendText2DVerts(const Vector2& startPos, const String& text, float cellHeight, const Rgba& tint /*= Rgba::WHITE*/, float aspectScale /*= 1.7f*/)
{
	SetTextureID(BOUND_TEXTURE_FONT);

	int length = (int)text.size();
	Vector2 startPoint = startPos;
	std::vector<Vertex3D_PCU>	vertices;
	
	BitmapFont* theFont = Renderer::GetInstance()->m_defaultFont;
	float cellWidth = theFont->GetGlyphAspect() * cellHeight * aspectScale;

	SetColor(tint);
	SetTextureID(BOUND_TEXTURE_FONT);

	// Draw
	for (int i = 0; i < length; i++)
	{
		// Get Current Letter
		char currentLetter = text.at(i);

		AABB2 posBox = AABB2(startPoint, Vector2(startPoint.x + cellWidth, startPoint.y + cellHeight));
		AABB2 uvBox = AABB2(theFont->GetUVsForGlyph(currentLetter));

		//vertices.push_back(Vertex3D_PCU(
		//	Vector3(posBox.mins.x, posBox.mins.y, 0.01f), tint, uvBox.mins)); // 0
		//
		//vertices.push_back(Vertex3D_PCU(
		//	Vector3(posBox.maxs.x, posBox.mins.y, 0.01f), tint, Vector2(uvBox.maxs.x, uvBox.mins.y))); //1
		//
		//vertices.push_back(Vertex3D_PCU(
		//	Vector3(posBox.maxs.x, posBox.maxs.y, 0.01f), tint, Vector2(uvBox.maxs.x, uvBox.maxs.y))); //2
		//
		//vertices.push_back(Vertex3D_PCU(
		//	Vector3(posBox.mins.x, posBox.mins.y, 0.01f), tint, Vector2(uvBox.mins.x, uvBox.mins.y))); //0
		//
		//vertices.push_back(Vertex3D_PCU(
		//	Vector3(posBox.maxs.x, posBox.maxs.y, 0.01f), tint, Vector2(uvBox.maxs.x, uvBox.maxs.y))); // 2
		//
		//vertices.push_back(Vertex3D_PCU(
		//	Vector3(posBox.mins.x, posBox.maxs.y, 0.01f), tint, Vector2(uvBox.mins.x, uvBox.maxs.y))); // 3




		SetUV(uvBox.mins);
		uint idx = PushVertex(Vector3(posBox.mins.x, posBox.mins.y, 0.01f));

		SetUV(Vector2(uvBox.maxs.x, uvBox.mins.y));
		PushVertex(Vector3(posBox.maxs.x, posBox.mins.y, 0.01f));

		SetUV(Vector2(uvBox.maxs.x, uvBox.maxs.y));
		PushVertex(Vector3(posBox.maxs.x, posBox.maxs.y, 0.01f));

		SetUV(Vector2(uvBox.mins.x, uvBox.maxs.y));
		PushVertex(Vector3(posBox.mins.x, posBox.maxs.y, 0.01f));

		AddFace(idx + 0, idx + 1, idx + 2);
		AddFace(idx + 0, idx + 2, idx + 3);

		startPoint.x += cellWidth;
	}
}

