#include "SpriteSheetViewer.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"

//===============================================================================================
SpriteSheetView::SpriteSheetView()
{
	m_cameraBounds = Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds();
	m_textureBounds = GetAABB2FromAABB2(Vector2(.1f, .1f), Vector2(.9f, .9f), m_cameraBounds);
} 

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::Update()
{
	AABB2 cameraBounds = Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds();
	Vector2 mousePos = GetMousePosition(cameraBounds);

	if (g_theSpriteSheet == nullptr)
		return;

	m_spriteCoords.x = RangeMapFloat(mousePos.x, m_textureBounds.mins.x, m_textureBounds.maxs.x, 0.f, g_theSpriteSheet->m_spriteLayout.x);
	m_spriteCoords.y = RangeMapFloat(mousePos.y, m_textureBounds.mins.y, m_textureBounds.maxs.y, g_theSpriteSheet->m_spriteLayout.y, 0.f);
	m_spriteCoords.x = ClampFloat(m_spriteCoords.x, 0, g_theSpriteSheet->m_spriteLayout.x - 1);
	m_spriteCoords.y = ClampFloat(m_spriteCoords.y, 0, g_theSpriteSheet->m_spriteLayout.y - 1);
	
	m_spriteIndex = m_spriteCoords.x + (m_spriteCoords.y * g_theSpriteSheet->m_spriteLayout.x);
	m_spriteIndex = ClampFloat(m_spriteIndex, 0, g_theSpriteSheet->m_spriteLayout.x * g_theSpriteSheet->m_spriteLayout.y);
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::Render() const
{
	Renderer* r = Renderer::GetInstance();

	if (g_theSpriteSheet == nullptr)
		return;

	r->m_clearScreen = true;
	r->SetCamera(r->m_defaultUICamera);
	r->SetShader(r->m_defaultShader);
	r->SetCurrentTexture();

	AABB2 cameraBounds = r->m_defaultUICamera->GetOrthoBounds();

	// texture and border
	r->DrawAABB2Filled(m_textureBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(m_textureBounds, Rgba(0, 0, 0, 255));
	r->DrawTexturedAABB2(m_textureBounds, *g_theSpriteSheet->m_texture, Vector2(0.f, 0.f), Vector2(1.f, 1.f), Rgba(255, 255, 255, 255));
	
	// mouse cursor
	r->DrawCircleFilled2D(GetMousePosition(cameraBounds), .01f, Rgba(0, 255, 0, 255));

	// Index and border
	AABB2 textBox = GetAABB2FromAABB2(Vector2(.1f, .02f), Vector2(.3f, .1f), m_cameraBounds);
	r->DrawAABB2Filled(textBox, Rgba(0, 0, 255, 255));
	r->DrawAABB2Outline(textBox, Rgba(0, 0, 0, 255));
	r->DrawText2D(textBox.mins, std::to_string(m_spriteIndex), .05f, Rgba(255, 255, 255, 255));

	// cursor? 
	float minX = RangeMapFloat(m_spriteCoords.x, 0, g_theSpriteSheet->m_spriteLayout.x, m_textureBounds.mins.x, m_textureBounds.maxs.x);
	float maxX = RangeMapFloat(m_spriteCoords.x + 1, 0, g_theSpriteSheet->m_spriteLayout.x, m_textureBounds.mins.x, m_textureBounds.maxs.x);
	float minY = RangeMapFloat(m_spriteCoords.y, 0, g_theSpriteSheet->m_spriteLayout.y, m_textureBounds.maxs.y, m_textureBounds.mins.y);
	float maxY = RangeMapFloat(m_spriteCoords.y + 1, 0, g_theSpriteSheet->m_spriteLayout.y, m_textureBounds.maxs.y, m_textureBounds.mins.y);
	r->DrawAABB2Outline(AABB2(minX, minY, maxX, maxY), Rgba(0, 0, 0, 255));

}
