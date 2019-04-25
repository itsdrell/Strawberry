#include "BitmapFont.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"

//===============================================================================================
BitmapFont::BitmapFont(const String& path, const SpriteSheet& spriteSheet, float baseAspect)
{
	m_spriteSheet = &spriteSheet;
	m_baseAspect = baseAspect;
	m_path = path;
}

//-----------------------------------------------------------------------------------------------
AABB2 BitmapFont::GetUVsForGlyph(int glyphUnicode) const
{
	return m_spriteSheet->GetTexCoordsForSpriteIndex(glyphUnicode);
}

//-----------------------------------------------------------------------------------------------
float BitmapFont::GetStringWidth(const std::string & asciiText, float cellHeight, float aspectScale)
{
	return cellHeight * aspectScale * m_baseAspect * asciiText.length();
}

//-----------------------------------------------------------------------------------------------
float BitmapFont::GetWidthOfAGlyph(float cellHeight, float aspectScale)
{
	return cellHeight * aspectScale * m_baseAspect;
}
