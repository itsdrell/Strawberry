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

}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::Update()
{
	AABB2 cameraBounds = Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds();
	Vector2 mousePos = GetMousePosition(cameraBounds);

	if (g_theSpriteSheet == nullptr)
		return;

	int x = (int) RangeMapFloat(mousePos.x, 0.f, cameraBounds.GetWidth(), 0, g_theSpriteSheet->m_spriteLayout.x);
	int y = (g_theSpriteSheet->m_spriteLayout.y - 1) - (int) RangeMapFloat(mousePos.y, 0.f, cameraBounds.GetHeight(), 0, g_theSpriteSheet->m_spriteLayout.y);
	int index = x + (y * g_theSpriteSheet->m_spriteLayout.x);
	PrintLog("XPos: " + std::to_string(x) + " Ypos: " + std::to_string(y) + " Index: " + std::to_string(index) + "\n");
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
	r->DrawAABB2Filled(cameraBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(cameraBounds, Rgba(0, 0, 0, 255));
	r->DrawTexturedAABB2(cameraBounds, *g_theSpriteSheet->m_texture, Vector2(0.f, 0.f), Vector2(1.f, 1.f), Rgba(255, 255, 255, 255));
	r->DrawCircleFilled2D(GetMousePosition(cameraBounds), .01f, Rgba(0, 255, 0, 255));
}
