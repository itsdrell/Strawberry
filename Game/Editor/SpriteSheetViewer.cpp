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
#include "Game/Main/Game.hpp"

//===============================================================================================
SpriteSheetView::SpriteSheetView()
{
	m_cameraBounds = Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds();
	m_textureBounds = GetAABB2FromAABB2(Vector2(.1f, .1f), Vector2(.9f, .9f), m_cameraBounds);
	m_textureBounds.ShrinkToSquare();
} 

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::Update()
{
	HandleInput();

	if (g_theSpriteSheet == nullptr)
		return;
	
	CalculateSpritePositions();
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::HandleInput()
{
	if (WasKeyJustPressed('r'))
	{
		Game::GetInstance()->LoadOrReloadSpriteSheet();
	}
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::CalculateSpritePositions()
{
	AABB2 cameraBounds = Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds();
	Vector2 mousePos = GetMousePosition(cameraBounds);

	m_spriteCoords.x = (int) RangeMapFloat(mousePos.x, m_textureBounds.mins.x, m_textureBounds.maxs.x, 0.f, (float) g_theSpriteSheet->m_spriteLayout.x);
	m_spriteCoords.y = (int) RangeMapFloat(mousePos.y, m_textureBounds.mins.y, m_textureBounds.maxs.y, (float) g_theSpriteSheet->m_spriteLayout.y, 0.f);
	m_spriteCoords.x = ClampInt(m_spriteCoords.x, 0, (g_theSpriteSheet->m_spriteLayout.x - 1));
	m_spriteCoords.y = ClampInt(m_spriteCoords.y, 0, (g_theSpriteSheet->m_spriteLayout.y - 1));

	m_spriteIndex = m_spriteCoords.x + (m_spriteCoords.y * g_theSpriteSheet->m_spriteLayout.x);
	m_spriteIndex = ClampInt(m_spriteIndex, 0, g_theSpriteSheet->m_spriteLayout.x * g_theSpriteSheet->m_spriteLayout.y);
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

	r->DrawAABB2Filled(m_cameraBounds, Rgba(255, 20, 147, 255));

	RenderGrid();
	RenderTexture();
	RenderIndex();
	RenderCursor();

}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderTexture() const
{
	Renderer* r = Renderer::GetInstance();

	r->DrawAABB2Outline(m_textureBounds, Rgba(255, 255, 255, 255));
	r->DrawTexturedAABB2(m_textureBounds, *g_theSpriteSheet->m_texture, Vector2(0.f, 0.f), Vector2(1.f, 1.f), Rgba(255, 255, 255, 255));
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderCursor() const
{
	Renderer* r = Renderer::GetInstance();

	float minX = RangeMapFloat((float) m_spriteCoords.x,		 0.f, (float) g_theSpriteSheet->m_spriteLayout.x, m_textureBounds.mins.x, m_textureBounds.maxs.x);
	float maxX = RangeMapFloat((float) (m_spriteCoords.x + 1),	 0.f, (float) g_theSpriteSheet->m_spriteLayout.x, m_textureBounds.mins.x, m_textureBounds.maxs.x);
	float minY = RangeMapFloat((float) m_spriteCoords.y,		 0.f, (float) g_theSpriteSheet->m_spriteLayout.y, m_textureBounds.maxs.y, m_textureBounds.mins.y);
	float maxY = RangeMapFloat((float) (m_spriteCoords.y + 1.f), 0.f, (float) g_theSpriteSheet->m_spriteLayout.y, m_textureBounds.maxs.y, m_textureBounds.mins.y);
	
	r->DrawAABB2Outline(AABB2(minX, minY, maxX, maxY), Rgba(0, 0, 0, 255));

	// mouse cursor
	r->DrawCircleFilled2D(GetMousePosition(m_cameraBounds), .01f, Rgba(0, 255, 0, 255));
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderGrid() const
{
	Renderer* r = Renderer::GetInstance();

	// white backgroud
	r->DrawAABB2Filled(m_textureBounds, Rgba(255, 255, 255, 255));

	Rgba gridColor = Rgba(171, 183, 183, 150);
	for (uint i = 1; i < (uint) g_theSpriteSheet->m_spriteLayout.x; i++)
	{
		float xpos = RangeMapFloat((float) i, 0.f, (float) g_theSpriteSheet->m_spriteLayout.x, m_textureBounds.mins.x, m_textureBounds.maxs.x);
		float ypos = RangeMapFloat((float) i, 0.f, (float) g_theSpriteSheet->m_spriteLayout.y, m_textureBounds.mins.y, m_textureBounds.maxs.y);

		Vector2 verticalStart = Vector2(xpos, m_textureBounds.maxs.y);
		Vector2 verticalEnd = Vector2(xpos, m_textureBounds.mins.y);

		Vector2 horizontalStart = Vector2(m_textureBounds.mins.x, ypos);
		Vector2 horizontalEnd = Vector2(m_textureBounds.maxs.x, ypos);

		r->DrawLine2D(verticalStart, verticalEnd, gridColor);
		r->DrawLine2D(horizontalStart, horizontalEnd, gridColor);
	}
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderIndex() const
{
	Renderer* r = Renderer::GetInstance();
	AABB2 textBox = GetAABB2FromAABB2(Vector2(.01f, .02f), Vector2(.2f, .1f), m_cameraBounds);
	
	r->DrawAABB2Outline(textBox, Rgba(255, 255, 255, 255));
	r->DrawText2D(textBox.mins, std::to_string(m_spriteIndex), .05f, Rgba(255, 255, 255, 255));
}
