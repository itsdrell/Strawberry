#include "BitmapFont.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"
#include "Engine/Math/MathUtils.hpp"

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
float BitmapFont::GetFontSizeToFitInBox(const Strings& text, const AABB2& bounds, float cellHeight, float aspectScale)
{
	std::string longestString = GetLargestStringInStrings(text);
	uint longestSize = longestString.size();

	uint amountOfVerticalLines = text.size() + 1;
	
	float widthOfBox = bounds.GetWidth();
	float heightOfBox = bounds.GetHeight();

	// we already fit!
	if(GetStringWidth(longestString, cellHeight, aspectScale) < widthOfBox 
		&&  GetHeightOfStrings(text, cellHeight) < heightOfBox)
	{
		return cellHeight;
	}
	
	float widthThatWillWork = (widthOfBox / longestSize) / (aspectScale * m_baseAspect);
	float heightThatWillWork = (heightOfBox / (amountOfVerticalLines -1)) / (aspectScale * m_baseAspect);

	return Min(widthThatWillWork, heightThatWillWork);
}

//-----------------------------------------------------------------------------------------------
void BitmapFont::GetTextWrapped(const Strings& words, const AABB2& bounds, float cellHeight, Strings* result, float aspect)
{
	float width = bounds.GetWidth();
	float fontWidth = cellHeight * aspect;
	float fontHeightSpace = cellHeight * 2.f;
	Vector2 currentPosition = Vector2(bounds.mins.x + fontWidth, bounds.maxs.y - (fontHeightSpace));

	Strings requestedLines;
	MakeWordsIntoLines(words, &requestedLines);

	for(int j = 0; j < requestedLines.size(); j++)
	{
		Strings wordsInLine = BreakSentenceIntoWords(requestedLines.at(j));
		String currentLine = "";

		for (int i = 0; i < (int)wordsInLine.size(); i++)
		{
			// Get the length of the word
			String currentWord = wordsInLine.at(i);
			float wordSize = ((float)currentWord.size() * fontWidth);

			float endPos = currentPosition.x + wordSize;
			bool didWeGoOverBounds = endPos >= (bounds.mins.x + width);

			if (didWeGoOverBounds)
			{
				// the word itself may NEVER fit, lets add it to the line and continue
				// need to include the padding on the left as our starting point or it wont ever work
				float widthOfWord = GetStringWidth(currentWord, cellHeight, aspect) + bounds.mins.x + (2 * cellHeight);
				if (widthOfWord > (bounds.mins.x + width))
				{
					result->push_back(currentWord);
				}
				else
				{
					result->push_back(currentLine);

					// add the current word and leftover words on the original line to 
					// a NEW line that we will go onto next
					for (uint k = i; k < wordsInLine.size() - 1; k++)
					{
						currentWord += (" " + wordsInLine.at(k + 1));
					}
					requestedLines.insert(requestedLines.begin() + j + 1, currentWord);
				}
				
				currentLine.clear();

				currentPosition.x = (bounds.mins.x + fontWidth);
				currentPosition.y -= fontHeightSpace;
			}
			else
			{
				currentLine += (" " + currentWord);

				currentPosition.x += (wordSize + fontWidth);
			}
		}

		if(currentLine != " ")
			result->push_back(currentLine);

		currentPosition.x = (bounds.mins.x + fontWidth);
		currentPosition.y -= fontHeightSpace;
	}

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
