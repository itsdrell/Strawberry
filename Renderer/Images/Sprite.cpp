#include "Sprite.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Images/Texture.hpp"

//===============================================================================================
std::map<String, Sprite*> Sprite::s_sprites;

//===============================================================================================
Sprite::Sprite(const String & path, const Vector2 & dimensions, float pixelsPerUnit, const Vector2 & pivot, const AABB2 & uvs)
{
#ifdef EMSCRIPTEN_PORT
	path = "Run_Win32/" + path;
#endif

	Renderer* r = Renderer::GetInstance();
	m_texture = r->CreateOrGetTexture(path, true);

	m_path = path;
	m_dimensions = dimensions;
	m_pixelsPerUnit = pixelsPerUnit;
	m_pivot = pivot;
	m_uvs = uvs;

	s_sprites.insert(std::pair<String, Sprite*>(m_path, this));
}

//-----------------------------------------------------------------------------------------------
Sprite::Sprite(const Texture* theTexture, const Vector2& dimensions, const AABB2& uvs /*= AABB2::ZERO_TO_ONE*/, float pixelsPerUnit /*= 1.f*/, const Vector2& pivot /*= Vector2(.5f, .5f)*/)
{
	m_texture = theTexture;
	m_dimensions = dimensions;
	m_uvs = uvs;
	m_pixelsPerUnit = pixelsPerUnit;
	m_pivot = pivot;
}

//-----------------------------------------------------------------------------------------------
STATIC Sprite* Sprite::CreateOrGetSprite(const String& path, const Vector2& dimensions, float pixelsPerUnit /*= 1.f*/, const Vector2& pivot /*= Vector2(.5f, .5f)*/, const AABB2& uvs /*= AABB2::ZERO_TO_ONE*/)
{
#ifdef EMSCRIPTEN_PORT
	path = "Run_Win32/" + path;
#endif

	Sprite* theSprite = GetSprite(path);

	if (theSprite == nullptr)
	{
		theSprite = new Sprite(path, dimensions, pixelsPerUnit, pivot, uvs);
	}

	return theSprite;
}

//-----------------------------------------------------------------------------------------------
STATIC Sprite* Sprite::GetSprite(const String & name)
{
	std::map<String, Sprite*>::iterator theIterator = s_sprites.begin();
	
	for (theIterator; theIterator != s_sprites.end(); theIterator++)
	{
		if (theIterator->first == name)
			return theIterator->second;
	}
	
	return nullptr;
}

//-----------------------------------------------------------------------------------------------
void Sprite::DeleteSprites()
{
	std::map<String, Sprite*>::iterator theIterator;

	for (theIterator = s_sprites.begin(); theIterator != s_sprites.end(); theIterator++)
	{
		delete theIterator->second;
		theIterator->second = nullptr;
	}

	s_sprites.clear();
}
