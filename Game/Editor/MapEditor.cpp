#include "MapEditor.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/General/Map/Map.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"

//===============================================================================================
MapEditor::MapEditor()
{
	m_map = new Map(IntVector2(4, 4));

	Renderer* r = Renderer::GetInstance();
	m_camera = new Camera();
	m_camera->SetColorTarget(r->m_defaultColorTarget);
	m_camera->SetProjectionOrthoByAspect(Window::GetInstance()->GetHeight() * .5f);
	r->SetCamera();

	m_cameraBounds = m_camera->GetOrthoBounds();
	m_tileSelectBounds = GetAABB2FromAABB2(Vector2(0.f, 0.1f), Vector2(.3f, .55f), m_cameraBounds);
	m_tileSelectBounds.ShrinkToSquare();
	m_optionsBounds = GetAABB2FromAABB2(Vector2(0.f, 0.f), Vector2(.3f, .1f), m_cameraBounds);
	
	m_selectedTilePreviewBounds = GetAABB2FromAABB2(Vector2(.9f, .05f), Vector2(.95f, .1f), m_cameraBounds);
	m_selectedTilePreviewBounds.GrowToSquare();

	m_tilePreviewBounds = GetAABB2FromAABB2(Vector2(.1f, .6f), Vector2(.2f, .75f), m_cameraBounds);
	m_tilePreviewBounds.ShrinkToSquare();
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
	HandleInput();
}

//-----------------------------------------------------------------------------------------------
void MapEditor::HandleInput()
{
	if (IsMouseButtonPressed(LEFT_MOUSE_BUTTON))
	{
		LeftClick();
	}
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
	if (mapBounds.IsPointInBox(mousePos) && m_selectedSpriteInfo != DEFAULT_TILE_SPRITE_INFO)
	{
		m_map->ChangeTileAtMousePos(mousePos, m_selectedSpriteInfo);
		return;
	}
}

//-----------------------------------------------------------------------------------------------
void MapEditor::SelectSpriteSheetTile(const Vector2& mousePos)
{
	int index = g_theSpriteSheet->GetSpriteIndexFromPositionInBounds(mousePos, m_tileSelectBounds);
	m_selectedSpriteInfo = TileSpriteInfo(0, index);
}

//-----------------------------------------------------------------------------------------------
void MapEditor::Render() const
{
	Renderer* r = Renderer::GetInstance();

	r->m_clearScreen = true;
	r->SetCamera(m_camera);
	r->SetShader(r->m_defaultShader);
	r->SetCurrentTexture();

	r->DrawAABB2Filled(m_cameraBounds, Rgba(255, 20, 147, 255));

	r->DrawTexturedAABB2(m_tileSelectBounds, *g_theSpriteSheet->m_texture, Vector2(0, 0), Vector2(1, 1), Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(m_tileSelectBounds, Rgba(255, 255, 255, 255));

	r->DrawAABB2Outline(m_optionsBounds, Rgba(255, 255, 255, 255));

	AABB2 spritePreviewUVs = g_theSpriteSheet->GetTexCoordsForSpriteIndex(m_selectedSpriteInfo.y);
	r->DrawTexturedAABB2(m_selectedTilePreviewBounds, *g_theSpriteSheet->m_texture, spritePreviewUVs.mins, spritePreviewUVs.maxs, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(m_selectedTilePreviewBounds, Rgba(255, 255, 255, 255));


	Vector2 mousePos = GetMousePosition(m_cameraBounds);
	if (m_tileSelectBounds.IsPointInBox(mousePos))
	{
		int index = g_theSpriteSheet->GetSpriteIndexFromPositionInBounds(mousePos, m_tileSelectBounds);
		AABB2 uvs = g_theSpriteSheet->GetTexCoordsForSpriteIndex(index);
		r->DrawTexturedAABB2(m_tilePreviewBounds, *g_theSpriteSheet->m_texture, uvs.mins, uvs.maxs, Rgba(255, 255, 255, 255));
		r->DrawAABB2Outline(m_tilePreviewBounds);

		float midPoint = m_tileSelectBounds.maxs.x - (m_tileSelectBounds.GetWidth() * .5f);
		r->DrawLine2D(Vector2(midPoint, m_tileSelectBounds.maxs.y), m_tilePreviewBounds.mins, Rgba(255,255,255,255));
		r->DrawLine2D(Vector2(midPoint, m_tileSelectBounds.maxs.y), Vector2(m_tilePreviewBounds.maxs.x, m_tilePreviewBounds.mins.y), Rgba(255, 255, 255, 255));

	}
	
	m_map->Render();
	
	r->DrawCircleFilled2D(GetMousePosition(m_cameraBounds), 1.f, Rgba(0, 255, 0, 255));

	r->SetCamera();
}


