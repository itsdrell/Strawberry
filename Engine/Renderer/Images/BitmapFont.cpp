#include "BitmapFont.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"

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
float BitmapFont::GetHeightOfStrings(const Strings& text, float cellHeight, float padding /*= .5f*/)
{
	uint amountOfLines = text.size();
	float lineHeight = amountOfLines * cellHeight;
	float amountOfPadding = (cellHeight * padding) * (amountOfLines - 1);
	
	return amountOfPadding + lineHeight;
}

//-----------------------------------------------------------------------------------------------
void BitmapFont::GetTextWrapped(const Strings& words, const AABB2& bounds, float cellHeight, Strings* result, float aspect)
{
	float width = bounds.GetWidth();
	float fontWidth = cellHeight * aspect;
	float fontHeightSpace = cellHeight * 2.f;
	Vector2 currentPosition = Vector2(bounds.mins.x + fontWidth, bounds.maxs.y - (fontHeightSpace));

	String line = "";
	for (int i = 0; i < (int)words.size(); i++)
	{
		// Get the length of the word
		String currentWord = words.at(i);
		float wordSize = ((float)currentWord.size() * fontWidth);

		float endPos = currentPosition.x + wordSize;
		bool didWeGoOverBounds = endPos >= (bounds.mins.x + width);

		if (currentWord == "\n" || didWeGoOverBounds)
		{
			currentPosition.x = (bounds.mins.x + fontWidth);
			currentPosition.y -= fontHeightSpace;

			result->push_back(line);
			line.clear();

			if(didWeGoOverBounds)
				i--;
		}
		else
		{
			line += (" " + currentWord);

			currentPosition.x += (wordSize + fontWidth);
		}
	}
	result->push_back(line);

	for(uint i = 0; i < result->size(); i++)
	{
		String* current = &result->at(i);
		RemoveLeadingAndEndingWhitespace(current);
	}
}

//-----------------------------------------------------------------------------------------------
float BitmapFont::GetWidthOfAGlyph(float cellHeight, float aspectScale)
{
	return cellHeight * aspectScale * m_baseAspect;
}
