#include "MapEditor.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/General/Map/Map.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Tools/Clock.hpp"

//===============================================================================================
MapEditor::MapEditor()
{
	m_map = new Map(IntVector2(128, 128));

	Renderer* r = Renderer::GetInstance();
	m_camera = new Camera();
	m_camera->SetColorTarget(r->m_defaultColorTarget);
	m_camera->SetProjectionOrthoByAspect(Window::GetInstance()->GetHeight() * DEFAULT_ZOOMED_IN_LEVEL);
	r->SetCamera();

	GenerateAllBounds();
}

//-----------------------------------------------------------------------------------------------
MapEditor::~MapEditor()
{
	delete m_map;
	m_map = nullptr;

	delete m_camera;
	m_camera = nullptr;
}

//-----------------------------------------------------------------------------------------------
void MapEditor::Update()
{
	m_map->Update();
	HandleInput();
	GenerateAllBounds();
}

//-----------------------------------------------------------------------------------------------
void MapEditor::HandleInput()
{
	if (IsMouseButtonPressed(LEFT_MOUSE_BUTTON))
	{
		LeftClick();
	}

	if (IsKeyPressed(KEYBOARD_CTRL) && WasKeyJustPressed('s'))
	{
		m_map->SaveMap();
	}

	if (IsMouseButtonPressed(RIGHT_MOUSE_BUTTON))
	{
		RightClick();
	}

	if (WasKeyJustPressed('z') || DidMouseWheelScrollUp())
	{
		m_currentZoom -= ZOOM_STEP_SIZE;
		m_currentZoom = ClampFloat(m_currentZoom, MIN_ZOOMED_IN_LEVEL, MAX_ZOOMED_OUT_LEVEL);
		m_camera->SetProjectionOrthoByAspect(Window::GetInstance()->GetHeight() * m_currentZoom);

	}

	if (WasKeyJustPressed('x') || DidMouseWheelScrollDown())
	{
		m_currentZoom += ZOOM_STEP_SIZE;
		m_currentZoom = ClampFloat(m_currentZoom, MIN_ZOOMED_IN_LEVEL, MAX_ZOOMED_OUT_LEVEL);
		m_camera->SetProjectionOrthoByAspect(Window::GetInstance()->GetHeight() * m_currentZoom);
	}

	if (WasKeyJustPressed('g'))
	{
		m_showGrid = !m_showGrid;
	}

	KeyboardMovement();
}

//-----------------------------------------------------------------------------------------------
void MapEditor::RenderUI() const
{
	Renderer* r = Renderer::GetInstance();

	// tile select
	r->DrawTexturedAABB2(m_tileSelectBounds, *g_theSpriteSheet->m_texture, Vector2(0, 0), Vector2(1, 1), Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(m_tileSelectBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_tileSelectBounds, 1.f, 1.f), Rgba(0, 0, 0, 255));

	// options
	r->DrawAABB2Outline(m_optionsBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_optionsBounds, 1.f, 1.f), Rgba(0, 0, 0, 255));

	// sprite preview (little box)
	AABB2 spritePreviewUVs = g_theSpriteSheet->GetTexCoordsForSpriteIndex(m_selectedSpriteInfo.GetSpriteIndex());
	r->DrawTexturedAABB2(m_selectedTilePreviewBounds, *g_theSpriteSheet->m_texture, spritePreviewUVs.mins, spritePreviewUVs.maxs, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(m_selectedTilePreviewBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_selectedTilePreviewBounds, 1.f, 1.f), Rgba(0,0,0,255));

	// zoom in
	Vector2 mousePos = GetMousePosition(m_cameraBounds);
	if (m_tileSelectBounds.IsPointInBox(mousePos))
	{
		int index = g_theSpriteSheet->GetSpriteIndexFromPositionInBounds(mousePos, m_tileSelectBounds);
		AABB2 uvs = g_theSpriteSheet->GetTexCoordsForSpriteIndex(index);
		r->DrawTexturedAABB2(m_tilePreviewBounds, *g_theSpriteSheet->m_texture, uvs.mins, uvs.maxs, Rgba(255, 255, 255, 255));
		r->DrawAABB2Outline(m_tilePreviewBounds);
		r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_tilePreviewBounds, 1.f, 1.f), Rgba(0, 0, 0, 255));

		r->DrawLine2D(mousePos, m_tilePreviewBounds.mins, Rgba(255, 255, 255, 255));
		r->DrawLine2D(mousePos, Vector2(m_tilePreviewBounds.maxs.x, m_tilePreviewBounds.mins.y), Rgba(255, 255, 255, 255));
	}

	// mouse
	r->DrawCircleFilled2D(GetMousePosition(m_cameraBounds), 1.f, Rgba(0, 255, 0, 255));
}

//-----------------------------------------------------------------------------------------------
void MapEditor::LeftClick()
{
	Vector2 mousePos = GetMousePosition(m_cameraBounds);
	
	// do tile select first since thats above the map
	if (m_tileSelectBounds.IsPointInBox(mousePos))
	{
		SelectSpriteSheetTile(mousePos);
		return;
	}

	if (m_optionsBounds.IsPointInBox(mousePos))
	{
		return;
	}
	
	AABB2 mapBounds = m_map->GetBounds();
	if (mapBounds.IsPointInBox(mousePos) && !m_selectedSpriteInfo.IsDefault())
	{
		m_map->ChangeTileAtMousePos(mousePos, m_selectedSpriteInfo);
		return;
	}
}

//-----------------------------------------------------------------------------------------------
void MapEditor::RightClick()
{
	Vector2 mousePos = GetMousePosition(m_cameraBounds);

	AABB2 mapBounds = m_map->GetBounds();
	if (mapBounds.IsPointInBox(mousePos) && !m_selectedSpriteInfo.IsDefault())
	{
		m_map->ChangeTileAtMousePos(mousePos, DEFAULT_TILE_SPRITE_INFO);
		return;
	}
}

//-----------------------------------------------------------------------------------------------
void MapEditor::KeyboardMovement()
{
	float stepSize = 16.f;
	float speed = 10.f;
	Vector2 dir;

	if (IsKeyPressed(KEYBOARD_LSHIFT))
	{
		speed *= 4.f;
	}
	
	if (IsKeyPressed('d'))
	{
		dir += Vector2(stepSize, 0.f);
	}

	if (IsKeyPressed('a'))
	{
		dir -= Vector2(stepSize, 0.f);
	}

	if (IsKeyPressed('w'))
	{
		dir += Vector2(0.f, stepSize);
	}

	if (IsKeyPressed('s'))
	{
		dir -= Vector2(0.f, stepSize);
	}

	AABB2 mapBounds = m_map->GetBounds();
	m_cameraPosition += (dir * g_theMasterClock->deltaTime * speed);
	m_cameraPosition.x = ClampFloat(m_cameraPosition.x, -TILE_SIZE, mapBounds.maxs.x + TILE_SIZE);
	m_cameraPosition.y = ClampFloat(m_cameraPosition.y, -TILE_SIZE, mapBounds.maxs.y + TILE_SIZE);
	m_camera->GoToPosition2D(m_cameraPosition);
}

//-----------------------------------------------------------------------------------------------
void MapEditor::SelectSpriteSheetTile(const Vector2& mousePos)
{
	int index = g_theSpriteSheet->GetSpriteIndexFromPositionInBounds(mousePos, m_tileSelectBounds);

	TileSpriteInfo newInfo;
	newInfo.SetSpriteIndex(index);
	newInfo.SetSpriteSheet(0);
	m_selectedSpriteInfo = newInfo;
}

//-----------------------------------------------------------------------------------------------
void MapEditor::GenerateAllBounds()
{
	m_cameraBounds = m_camera->GetBounds();
	m_tileSelectBounds = GetAABB2FromAABB2(Vector2(0.f, 0.1f), Vector2(.3f, .55f), m_cameraBounds);
	m_tileSelectBounds.ShrinkToSquare();
	m_optionsBounds = GetAABB2FromAABB2(Vector2(0.f, 0.f), Vector2(.3f, .1f), m_cameraBounds);

	m_selectedTilePreviewBounds = GetAABB2FromAABB2(Vector2(.9f, .05f), Vector2(.95f, .1f), m_cameraBounds);
	m_selectedTilePreviewBounds.GrowToSquare();

	m_tilePreviewBounds = GetAABB2FromAABB2(Vector2(.1f, .6f), Vector2(.2f, .75f), m_cameraBounds);
	m_tilePreviewBounds.ShrinkToSquare();
}

//-----------------------------------------------------------------------------------------------
void MapEditor::Render() const
{
	Renderer* r = Renderer::GetInstance();

	r->m_clearScreen = true;
	r->SetCamera(m_camera);
	r->SetShader(r->m_defaultShader);
	r->SetCurrentTexture();

	// bg
	r->DrawAABB2Filled(m_cameraBounds, Rgba(0, 0, 0, 255));

	m_map->Render(m_showGrid);
	RenderUI();
	
	r->SetCamera();
}


