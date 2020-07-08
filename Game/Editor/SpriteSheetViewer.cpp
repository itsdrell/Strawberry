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
#include "Engine/Core/Tools/DebugRendering.hpp"
#include "Engine/Core/General/Rgba.hpp"

//===============================================================================================
SpriteSheetView::SpriteSheetView()
{
	m_cameraBounds = Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds();
	m_textureBounds = GetAABB2FromAABB2(Vector2(.1f, .1f), Vector2(.9f, .9f), m_cameraBounds);
	m_textureBounds.ShrinkToSquare();

	m_switchBoxBounds = GetAABB2FromAABB2(Vector2(.825f, .4f), Vector2(.95f, .6f), m_cameraBounds);
	m_spriteButtonBounds = GetAABB2FromAABB2(Vector2(0.f, .5f), Vector2(1.f, 1.f), m_switchBoxBounds);
	m_colorButtonBounds = GetAABB2FromAABB2(Vector2(0.f, 0.f), Vector2(1.f, .5f), m_switchBoxBounds);

	float height = .9f;
	float step = .05;
	for(uint i = 0; i < MAX_AMOUNT_OF_SPRITE_SHEETS; i++)
	{
		m_spriteSheetButtonBounds[i] = GetAABB2FromAABB2(Vector2(.18f, height - step), Vector2(.23f, height), m_cameraBounds);
		height -= step;
	}

	CreateDisplayColors();
} 

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::Update()
{
	HandleInput();
	
	CalculateSpritePositions();
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::HandleInput()
{
	if (WasKeyJustPressed('r'))
	{
		Game::GetInstance()->LoadOrReloadSpriteSheets();
		CreateDisplayColors();
	}

	if (WasKeyJustPressed('c'))
	{
		m_currentMode = COLOR_VIEW_MODE;
	}

	if(WasKeyJustPressed('s'))
	{
		m_currentMode = SPRITE_VIEW_MODE;
	}

	if (m_currentMode == COLOR_VIEW_MODE)
	{
		int size = (int)m_displayColors.size() - 1;
		if (DidMouseWheelScrollUp())
		{

			if (m_colorIndex != 0)
			{
				m_colorIndex--;
			}
		}

		if (DidMouseWheelScrollDown())
		{
			if (m_colorIndex <= size - m_maxAmountOfColors)
			{
				m_colorIndex++;
			}
		}
	}

	if(WasKeyJustPressed('g'))
	{
		m_showGrid = !m_showGrid;
	}

	if (WasMouseButtonJustReleased(LEFT_MOUSE_BUTTON))
	{
		Vector2 mousePos = GetMousePosition(m_cameraBounds);
		
		if(m_currentMode == COLOR_VIEW_MODE && m_spriteButtonBounds.IsPointInBox(mousePos))
		{
			m_currentMode = SPRITE_VIEW_MODE;
		}

		if (m_currentMode == SPRITE_VIEW_MODE && m_colorButtonBounds.IsPointInBox(mousePos))
		{
			m_currentMode = COLOR_VIEW_MODE;
		}

		for(uint i = 0; i < MAX_AMOUNT_OF_SPRITE_SHEETS; i++)
		{
			if(m_spriteSheetButtonBounds[i].IsPointInBox(mousePos))
			{
				m_currentSpriteSheet = i;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::CalculateSpritePositions()
{
	AABB2 cameraBounds = Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds();
	Vector2 mousePos = GetMousePosition(cameraBounds);

	SpriteSheet* current = g_allSpriteSheets[m_currentSpriteSheet];

	m_spriteCoords.x = (int) RangeMapFloat(mousePos.x, m_textureBounds.mins.x, m_textureBounds.maxs.x, 0.f, (float)current->m_spriteLayout.x);
	m_spriteCoords.y = (int) RangeMapFloat(mousePos.y, m_textureBounds.mins.y, m_textureBounds.maxs.y, (float)current->m_spriteLayout.y, 0.f);
	m_spriteCoords.x = ClampInt(m_spriteCoords.x, 0, (current->m_spriteLayout.x - 1));
	m_spriteCoords.y = ClampInt(m_spriteCoords.y, 0, (current->m_spriteLayout.y - 1));

	m_spriteIndex = m_spriteCoords.x + (m_spriteCoords.y * current->m_spriteLayout.x);
	m_spriteIndex = ClampInt(m_spriteIndex, 0, current->m_spriteLayout.x * current->m_spriteLayout.y);

	m_spriteIndex += (m_currentSpriteSheet * 256);
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::CreateDisplayColors()
{
	for (auto it = Rgba::s_defaultColors.begin(); it != Rgba::s_defaultColors.end(); it++)
	{
		DisplayColor newColor = DisplayColor(it->first, it->second);
		m_displayColors.push_back(newColor);
	}

	m_displayColors.push_back({ "random", Rgba::GetRandomColor() });
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::Render() const
{
	Renderer* r = Renderer::GetInstance();

	r->m_clearScreen = true;
	r->SetCamera(r->m_defaultUICamera);
	r->SetShader(r->m_defaultShader);
	r->SetCurrentTexture();

	r->DrawAABB2Filled(m_cameraBounds, Rgba::STRAWBERRY_RED);

	if(m_currentMode == SPRITE_VIEW_MODE)
	{
		RenderTexture();
		RenderGrid();
		RenderIndex();
		RenderSpriteSheetButtons();
	}
	else
	{
		RenderColors();
	}

	RenderSwitchButtons();
	RenderCursor();
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::Enter()
{
	CreateDisplayColors();
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderTexture() const
{
	Renderer* r = Renderer::GetInstance();

	Texture* theTexture = g_allSpriteSheets[m_currentSpriteSheet]->m_texture;

	r->DrawAABB2Outline(m_textureBounds, Rgba(255, 255, 255, 255));
	r->DrawTexturedAABB2(m_textureBounds, *theTexture, Vector2(0.f, 0.f), Vector2(1.f, 1.f), Rgba(255, 255, 255, 255));
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderCursor() const
{
	Renderer* r = Renderer::GetInstance();

	SpriteSheet* current = g_allSpriteSheets[m_currentSpriteSheet];

	if(m_currentMode == SPRITE_VIEW_MODE)
	{
		float minX = RangeMapFloat((float)m_spriteCoords.x, 0.f, (float)current->m_spriteLayout.x, m_textureBounds.mins.x, m_textureBounds.maxs.x);
		float maxX = RangeMapFloat((float)(m_spriteCoords.x + 1), 0.f, (float)current->m_spriteLayout.x, m_textureBounds.mins.x, m_textureBounds.maxs.x);
		float minY = RangeMapFloat((float)m_spriteCoords.y, 0.f, (float)current->m_spriteLayout.y, m_textureBounds.maxs.y, m_textureBounds.mins.y);
		float maxY = RangeMapFloat((float)(m_spriteCoords.y + 1.f), 0.f, (float)current->m_spriteLayout.y, m_textureBounds.maxs.y, m_textureBounds.mins.y);

		r->DrawAABB2Outline(AABB2(minX, minY, maxX, maxY), Rgba(0, 0, 0, 255));
	}

	// mouse cursor
	r->DrawCircleFilled2D(GetMousePosition(m_cameraBounds), .01f, Rgba(0, 255, 0, 255));
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderGrid() const
{
	if (m_showGrid == false)
		return;
	
	Renderer* r = Renderer::GetInstance();

	// magenta so it wont have the same color as a sprite backgroud
	r->DrawAABB2Filled(m_textureBounds, Rgba(255, 0, 255, 0));

	SpriteSheet* current = g_allSpriteSheets[m_currentSpriteSheet];

	Rgba gridColor = Rgba(171, 183, 183, 150);
	for (uint i = 1; i < (uint)current->m_spriteLayout.x; i++)
	{
		float xpos = RangeMapFloat((float) i, 0.f, (float) current->m_spriteLayout.x, m_textureBounds.mins.x, m_textureBounds.maxs.x);
		float ypos = RangeMapFloat((float) i, 0.f, (float) current->m_spriteLayout.y, m_textureBounds.mins.y, m_textureBounds.maxs.y);

		Vector2 verticalStart = Vector2(xpos, m_textureBounds.maxs.y);
		Vector2 verticalEnd = Vector2(xpos, m_textureBounds.mins.y);

		Vector2 horizontalStart = Vector2(m_textureBounds.mins.x, ypos);
		Vector2 horizontalEnd = Vector2(m_textureBounds.maxs.x, ypos);

		r->DrawLine2D(verticalStart, verticalEnd, gridColor);
		r->DrawLine2D(horizontalStart, horizontalEnd, gridColor);
	}
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderSpriteSheetButtons() const
{
	Renderer* r = Renderer::GetInstance();
	
	for(uint i = 0; i < MAX_AMOUNT_OF_SPRITE_SHEETS; i++)
	{
		AABB2 currentBounds = m_spriteSheetButtonBounds[i];
		Rgba backgroundColor = (i == m_currentSpriteSheet) ? Rgba(100, 100, 100, 155) : Rgba::STRAWBERRY_RED;
		
		r->DrawAABB2Filled(currentBounds, backgroundColor);
		r->DrawAABB2Outline(currentBounds, Rgba::WHITE);
		r->DrawTextInBox(std::to_string(i), currentBounds, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	}
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderIndex() const
{
	Renderer* r = Renderer::GetInstance();
	AABB2 textBox = GetAABB2FromAABB2(Vector2(.01f, .02f), Vector2(.2f, .1f), m_cameraBounds);
	
	r->DrawAABB2Outline(textBox, Rgba(255, 255, 255, 255));
	r->DrawTextInBox(std::to_string(m_spriteIndex), textBox, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderSwitchButtons() const
{
	Renderer* r = Renderer::GetInstance();

	r->DrawAABB2Outline(m_switchBoxBounds);
	r->DrawAABB2Outline(m_colorButtonBounds);
	r->DrawAABB2Outline(m_spriteButtonBounds);

	if (m_currentMode == SPRITE_VIEW_MODE)
		r->DrawAABB2Filled(m_spriteButtonBounds, Rgba(100,100,100,155));
	else
		r->DrawAABB2Filled(m_colorButtonBounds, Rgba(100, 100, 100,155));

	r->DrawTextInBox("Sprite", m_spriteButtonBounds, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	r->DrawTextInBox("Colors", m_colorButtonBounds, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
}

//-----------------------------------------------------------------------------------------------
void SpriteSheetView::RenderColors() const
{
	DebugRenderLog("colors", 0.f);

	Renderer* r = Renderer::GetInstance();

	r->DrawAABB2Outline(m_textureBounds);

	float percent = 1.f;
	float step = 1.f / (float)m_maxAmountOfColors;
	for(int i = m_colorIndex; i < m_colorIndex + m_maxAmountOfColors; i++)
	{
		AABB2 currentBounds = GetAABB2FromAABB2(Vector2(0.f, percent - step), Vector2(1.f, percent), m_textureBounds);
		DisplayColor currentColor = m_displayColors.at(i);

		Rgba color = (currentColor.m_name == "random") ? Rgba::GetRandomColor() : currentColor.m_color;

		r->DrawAABB2Filled(currentBounds, color);
		r->DrawTextInBox(currentColor.m_name, currentBounds, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f), Rgba::WHITE, true);

		percent -= step;
	}

	AABB2 hintTextBox = GetAABB2FromAABB2(Vector2(0.15f, 0.f), Vector2(.85f, .1f), m_cameraBounds);
	r->DrawTextInBox("Can be edited in gameconfig.lua :)", hintTextBox, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
}
