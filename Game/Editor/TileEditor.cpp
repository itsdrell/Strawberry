#include "TileEditor.hpp"
#include "Game/Editor/MapEditor.hpp"
#include "Game/General/Map/Map.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"

//===============================================================================================
TileEditor::TileEditor( MapEditor * theMapEditor)
{
	m_mapEditor = theMapEditor;
}

//-----------------------------------------------------------------------------------------------
TileEditor::~TileEditor()
{
}

//-----------------------------------------------------------------------------------------------
void TileEditor::Update()
{
	HandleInput();
	GenerateAllBounds();
}

//-----------------------------------------------------------------------------------------------
void TileEditor::Render() const
{
	RenderUI();
}

//-----------------------------------------------------------------------------------------------
void TileEditor::HandleInput()
{
	if (IsMouseButtonPressed(LEFT_MOUSE_BUTTON))
	{
		LeftClick();
	}

	if (IsMouseButtonPressed(RIGHT_MOUSE_BUTTON))
	{
		RightClick();
	}
}

//-----------------------------------------------------------------------------------------------
void TileEditor::LeftClick()
{
	Vector2 mousePos = GetMousePosition(m_mapEditor->m_cameraBounds);

	// do tile select first since thats above the map
	if (m_tileSelectBounds.IsPointInBox(mousePos))
	{
		SelectSpriteSheetTile(mousePos);
		return;
	}

	if (m_mapEditor->m_optionsBounds.IsPointInBox(mousePos))
	{
		return;
	}

	AABB2 mapBounds = m_mapEditor->m_map->GetBounds();
	if (mapBounds.IsPointInBox(mousePos) && !m_selectedSpriteInfo.IsDefault())
	{
		m_mapEditor->m_map->ChangeTileAtMousePos(mousePos, m_selectedSpriteInfo);
		return;
	}
}

//-----------------------------------------------------------------------------------------------
void TileEditor::RightClick()
{
	Vector2 mousePos = GetMousePosition(m_mapEditor->m_cameraBounds);

	AABB2 mapBounds = m_mapEditor->m_map->GetBounds();
	if (mapBounds.IsPointInBox(mousePos) && !m_selectedSpriteInfo.IsDefault())
	{
		m_mapEditor->m_map->ChangeTileAtMousePos(mousePos, DEFAULT_TILE_SPRITE_INFO);
		return;
	}
}

//-----------------------------------------------------------------------------------------------
void TileEditor::SelectSpriteSheetTile(const Vector2& mousePos)
{
	int index = g_theSpriteSheet->GetSpriteIndexFromPositionInBounds(mousePos, m_tileSelectBounds);

	TileSpriteInfo newInfo;
	newInfo.SetSpriteIndex(index);
	newInfo.SetSpriteSheet(0);
	m_selectedSpriteInfo = newInfo;
}

//-----------------------------------------------------------------------------------------------
void TileEditor::GenerateAllBounds()
{
	m_tileSelectBounds = GetAABB2FromAABB2(Vector2(0.f, 0.1f), Vector2(.3f, .55f), m_mapEditor->m_cameraBounds);
	m_tileSelectBounds.ShrinkToSquare();

	m_selectedTilePreviewBounds = GetAABB2FromAABB2(Vector2(.9f, .05f), Vector2(.95f, .1f), m_mapEditor->m_cameraBounds);
	m_selectedTilePreviewBounds.GrowToSquare();

	m_tilePreviewBounds = GetAABB2FromAABB2(Vector2(.1f, .6f), Vector2(.2f, .75f), m_mapEditor->m_cameraBounds);
	m_tilePreviewBounds.ShrinkToSquare();
}

//-----------------------------------------------------------------------------------------------
void TileEditor::RenderUI() const
{
	Renderer* r = Renderer::GetInstance();

	// tile select
	r->DrawTexturedAABB2(m_tileSelectBounds, *g_theSpriteSheet->m_texture, Vector2(0, 0), Vector2(1, 1), Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(m_tileSelectBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_tileSelectBounds, 1.f, 1.f), Rgba(0, 0, 0, 255));

	// options
	r->DrawAABB2Outline(m_mapEditor->m_optionsBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_mapEditor->m_optionsBounds, 1.f, 1.f), Rgba(0, 0, 0, 255));

	// sprite preview (little box)
	AABB2 spritePreviewUVs = g_theSpriteSheet->GetTexCoordsForSpriteIndex(m_selectedSpriteInfo.GetSpriteIndex());
	r->DrawTexturedAABB2(m_selectedTilePreviewBounds, *g_theSpriteSheet->m_texture, spritePreviewUVs.mins, spritePreviewUVs.maxs, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(m_selectedTilePreviewBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_selectedTilePreviewBounds, 1.f, 1.f), Rgba(0, 0, 0, 255));

	// zoom in
	Vector2 mousePos = GetMousePosition(m_mapEditor->m_cameraBounds);
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
}
