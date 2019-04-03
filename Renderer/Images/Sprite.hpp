#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include <map>

//====================================================================================
// Forward Declare
//====================================================================================
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
class Sprite
{
	friend class Renderer;

public:

	Sprite() {}
	Sprite(const String& path, const Vector2& dimensions, 
		float pixelsPerUnit = 1.f, const Vector2& pivot = Vector2(.5f, .5f), const AABB2& uvs = AABB2::ZERO_TO_ONE);

public:
	static Sprite* CreateOrGetSprite(const String& path, const Vector2& dimensions,
		float pixelsPerUnit = 1.f, const Vector2& pivot = Vector2(.5f, .5f), const AABB2& uvs = AABB2::ZERO_TO_ONE);

private:
	static Sprite* GetSprite(const String& name);

	// renderer calls this
	static void DeleteSprites();

public:
	Texture*		m_texture = nullptr;
	AABB2			m_uvs = AABB2::ZERO_TO_ONE;
	String			m_path;
	Vector2			m_dimensions;
	Vector2			m_pivot = Vector2(.5f, .5f);
	float			m_pixelsPerUnit = 1.f;

private:
	static std::map<String, Sprite*>			s_sprites;

};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [3/29/2019]
//====================================================================================