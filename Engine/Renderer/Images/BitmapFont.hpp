#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class AABB2;
class Texture;

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
class BitmapFont
{
	friend class Renderer;

private:
	explicit BitmapFont(const String& path, const SpriteSheet& spriteSheet, float baseAspect);

public:
	AABB2		GetUVsForGlyph(int glyphUnicode) const; //
	float		GetGlyphAspect() const { return m_baseAspect; } // will change later
	float		GetWidthOfAGlyph(float cellHeight, float aspectScale = 1.f);
	
	float		GetStringWidth(const std::string& asciiText, float cellHeight, float aspectScale);
	float		GetHeightOfStrings(const Strings& text, float cellHeight, float padding = .5f);
	void		GetTextWrapped(const Strings& words, const AABB2& bounds, float cellHeight, Strings* result, float aspect = 1.77f);

	Texture*	GetTexture() const { return m_spriteSheet->m_texture; }

private:
	const SpriteSheet*			m_spriteSheet = nullptr;
	float						m_baseAspect = 1.77f;
	String						m_path;

};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [4/2/2019]
//====================================================================================