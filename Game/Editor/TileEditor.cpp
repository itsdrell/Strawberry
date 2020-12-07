#include "TileEditor.hpp"
#include "Game/Editor/MapEditor.hpp"
#include "Game/General/Map/Map.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Systems/MeshBuilder.hpp"
#include "Engine/Renderer/RenderTypes.hpp"
#include <deque>

//===============================================================================================
String GetTileDrawModeAsString(TileDrawModes mode)
{
	switch (mode)
	{
	case TILE_DRAW_MODE_NORMAL:
		return "Normal";
		break;
	case TILE_DRAW_MODE_FILL:
		return "Fill";
		break;
	default:
		return "idk";
		break;
	}
}

//===============================================================================================
TileEditor::TileEditor( MapEditor * theMapEditor)
{
	m_mapEditor = theMapEditor;
	m_tileSelectPreviewMB = new MeshBuilder();
}

//-----------------------------------------------------------------------------------------------
TileEditor::~TileEditor()
{
	delete m_tileSelectPreviewMB;
	m_tileSelectPreviewMB = nullptr;
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
	RenderTilePlacementPreview();
	RenderUI();
}

//-----------------------------------------------------------------------------------------------
void TileEditor::HandleInput()
{
	if(WasKeyJustPressed('f'))
	{
		m_drawMode = TILE_DRAW_MODE_FILL;
	}
	
	if (IsKeyPressed(KEYBOARD_LSHIFT))
	{
		CreateTilePlacementPreview();
	}
	
	if (IsMouseButtonPressed(LEFT_MOUSE_BUTTON))
	{
		LeftClick();
	}

	if(WasMouseButtonJustReleased(LEFT_MOUSE_BUTTON))
	{
		LeftClickRelease();
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

	for (int i = 0; i < MAX_AMOUNT_OF_SPRITE_SHEETS; i++)
	{
		AABB2 currentBounds = m_spriteSheetButtonBounds[i];

		if (currentBounds.IsPointInBox(mousePos))
		{
			m_selectedSpriteSheet = i;
			return;
		}
	}

	//-----------------------------------------------------------------------------------------------
	// should happen after UI stuff since UI gets priority (since its on top)
	//-----------------------------------------------------------------------------------------------
	AABB2 mapBounds = m_mapEditor->m_map->GetBounds();
	if (mapBounds.IsPointInBox(mousePos) && !m_selectedSpriteInfo.IsDefault())
	{
		m_lastSelectedTilePosition = mousePos;
		
		if(m_drawMode == TILE_DRAW_MODE_FILL)
		{
			DrawModeFill();
		}
		else
		{
			DrawModeNormal();
		}
		
		return;
	}
}

//-----------------------------------------------------------------------------------------------
void TileEditor::LeftClickRelease()
{
	Vector2 mousePos = GetMousePosition(m_mapEditor->m_cameraBounds);
	
	if (m_mapEditor->m_optionsBounds.IsPointInBox(mousePos))
	{
		if (m_drawMode == TILE_DRAW_MODE_FILL)
		{
			m_drawMode = TILE_DRAW_MODE_NORMAL;
		}
		else
		{
			m_drawMode = TILE_DRAW_MODE_FILL;
		}

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
		TileSpriteInfo theInfo = DEFAULT_TILE_SPRITE_INFO; // emscripten complaint
		m_mapEditor->m_map->ChangeTileAtMousePos(mousePos, theInfo);
		return;
	}
}

//-----------------------------------------------------------------------------------------------
void TileEditor::SelectSpriteSheetTile(const Vector2& mousePos)
{
	SpriteSheet* selectedSpriteSheet = g_allSpriteSheets[m_selectedSpriteSheet];
	int index = selectedSpriteSheet->GetSpriteIndexFromPositionInBounds(mousePos, m_tileSelectBounds);

	TileSpriteInfo newInfo;
	newInfo.SetSpriteIndex(index);
	newInfo.SetSpriteSheet(m_selectedSpriteSheet);
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

	AABB2 unitBox = GetAABB2FromAABB2(Vector2(0.f, 0.f), Vector2(.1f, .5f), m_tileSelectBounds);
	Vector2 minPos = Vector2(m_tileSelectBounds.maxs.x, m_tileSelectBounds.maxs.y - unitBox.GetHeight());

	m_spriteSheetIndexBounds = AABB2(minPos.x, minPos.y, minPos.x + unitBox.GetWidth(), minPos.y + unitBox.GetHeight());

	float amount = 1.f;
	float step = .25f;
	for (uint i = 0; i < MAX_AMOUNT_OF_SPRITE_SHEETS; i++)
	{
		m_spriteSheetButtonBounds[i] = GetAABB2FromAABB2(Vector2(.0f, amount - step), Vector2(1.f, amount), m_spriteSheetIndexBounds);
		amount -= step;
	}
}

//-----------------------------------------------------------------------------------------------
void TileEditor::CreateTilePlacementPreview()
{
	Vector2 mousePos = GetMousePosition(m_mapEditor->m_cameraBounds);
	Map& theMap = *m_mapEditor->m_map;
	m_tilesToChange.clear();

	IntVector2 currentMousePos = IntVector2((int) (mousePos.x / TILE_SIZE), (int)(mousePos.y / TILE_SIZE));
	IntVector2 lastPos = IntVector2((int)(m_lastSelectedTilePosition.x / TILE_SIZE), (int)(m_lastSelectedTilePosition.y / TILE_SIZE));

	IntVector2 distance = currentMousePos - lastPos;

	Vector2 currentPos = lastPos.ToVector2() + Vector2(.5f,.5f); // center of tile so both sides do the same pattern
	int amoutOfSteps = Max(abs(distance.x), abs(distance.y));

	if (amoutOfSteps == 0)
		return;

	float xStep = ((float) distance.x) / (float) amoutOfSteps;
	float yStep = (float) distance.y / (float) amoutOfSteps;

	SpriteSheet* selectedSpriteSheet = g_allSpriteSheets[m_selectedSpriteSheet];

	for (int i = 0; i < amoutOfSteps; i++)
	{
		currentPos.x += xStep;
		currentPos.y += yStep;

		if (!theMap.GetBounds().IsPointInBox(currentPos))
			return;

		IntVector2 tilePos = IntVector2((int) currentPos.x, (int) currentPos.y);
		AABB2 bounds = AABB2((float)(tilePos.x * TILE_SIZE), (float)(tilePos.y * TILE_SIZE),
			(float)((tilePos.x  * TILE_SIZE) + TILE_SIZE), (float)((tilePos.y * TILE_SIZE) + TILE_SIZE));
		AABB2 uvs = selectedSpriteSheet->GetTexCoordsForSpriteIndex(m_selectedSpriteInfo.GetSpriteIndex());

		m_tileSelectPreviewMB->Add2DPlane(
			bounds,
			uvs, 
			Rgba(255, 255, 255, 200));

		m_tilesToChange.push_back(tilePos);
	}
}

//-----------------------------------------------------------------------------------------------
void TileEditor::DrawModeNormal()
{
	m_mapEditor->m_map->ChangeTileAtMousePos(m_lastSelectedTilePosition, m_selectedSpriteInfo);

	if (IsKeyPressed(KEYBOARD_LSHIFT))
	{
		for (uint i = 0; i < m_tilesToChange.size(); i++)
		{
			m_mapEditor->m_map->ChangeTileAtTilePos(m_tilesToChange.at(i), m_selectedSpriteInfo);
		}
	}
}

//-----------------------------------------------------------------------------------------------
void TileEditor::DrawModeFill()
{
	Map& theMap = *m_mapEditor->m_map;
	m_tilesToChange.clear();

	IntVector2 currentMousePos = IntVector2((int)(m_lastSelectedTilePosition.x / TILE_SIZE), (int)(m_lastSelectedTilePosition.y / TILE_SIZE));
	Tile& theSelectedTile = theMap.GetTileByTilePos(currentMousePos);

	if(theSelectedTile.m_spriteInfo.GetData() == m_selectedSpriteInfo.GetData())
	{
		return; // already the same
	}

	// need to find the bounds of our 16x16 box so we only change those tiles
	Vector2 pos = currentMousePos.ToVector2();
	float scale = 16.f; // could change this to make it cover more than 1 square (32,64,etc)
	IntVector2 min = IntVector2((int) std::floor(pos.x / scale) * (int) scale, (int) std::floorf(pos.y / scale) * (int) scale);
	IntVector2 max = IntVector2((int) std::ceil(pos.x / scale) * (int) scale, (int)std::ceil(pos.y / scale) * (int) scale);
	max = max - IntVector2(1, 1); // accounting for zero indexing

	std::deque<IntVector2> heatmap;
	heatmap.push_back(currentMousePos);

	while(heatmap.empty() == false)
	{
		IntVector2 currentTile = heatmap[0];
		
		IntVector2 north = currentTile + IntVector2(0,1);
		IntVector2 south = currentTile + IntVector2(0, -1);
		IntVector2 east = currentTile + IntVector2(1, 0);
		IntVector2 west = currentTile + IntVector2(-1, 0);

		TileSpriteInfo northTileInfo = theMap.GetTileByTilePos(north).m_spriteInfo;
		TileSpriteInfo southTileInfo = theMap.GetTileByTilePos(south).m_spriteInfo;
		TileSpriteInfo eastTileInfo = theMap.GetTileByTilePos(east).m_spriteInfo;
		TileSpriteInfo westTileInfo = theMap.GetTileByTilePos(west).m_spriteInfo;

		// needs out of bounds checking as well as in tile grid (should be same)
		if((TileSpriteInfo::AreSpritesSame(northTileInfo, theSelectedTile.m_spriteInfo)) 
			&& (TileSpriteInfo::AreSpritesSame(northTileInfo, m_selectedSpriteInfo) == false)
			&& (IsTileInsideSector(min, max, north)))
		{
			if(std::find(m_tilesToChange.begin(), m_tilesToChange.end(), north) == m_tilesToChange.end())
			{
				heatmap.push_back(north);
				m_tilesToChange.push_back(north);
			}
		}

		if ((TileSpriteInfo::AreSpritesSame(southTileInfo, theSelectedTile.m_spriteInfo))
			&& (TileSpriteInfo::AreSpritesSame(southTileInfo, m_selectedSpriteInfo) == false)
			&& (IsTileInsideSector(min, max, south)))
		{
			if (std::find(m_tilesToChange.begin(), m_tilesToChange.end(), south) == m_tilesToChange.end())
			{
				heatmap.push_back(south);
				m_tilesToChange.push_back(south);
			}
		}

		if ((TileSpriteInfo::AreSpritesSame(eastTileInfo, theSelectedTile.m_spriteInfo))
			&& (TileSpriteInfo::AreSpritesSame(eastTileInfo, m_selectedSpriteInfo) == false)
			&& (IsTileInsideSector(min, max, east)))
		{
			if (std::find(m_tilesToChange.begin(), m_tilesToChange.end(), east) == m_tilesToChange.end())
			{
				heatmap.push_back(east);
				m_tilesToChange.push_back(east);
			}
		}

		if ((TileSpriteInfo::AreSpritesSame(westTileInfo, theSelectedTile.m_spriteInfo))
			&& (TileSpriteInfo::AreSpritesSame(westTileInfo, m_selectedSpriteInfo) == false)
			&& (IsTileInsideSector(min, max, west)))
		{
			if (std::find(m_tilesToChange.begin(), m_tilesToChange.end(), west) == m_tilesToChange.end())
			{
				heatmap.push_back(west);
				m_tilesToChange.push_back(west);
			}
		}

		heatmap.pop_front();
	}

	for (uint i = 0; i < m_tilesToChange.size(); i++)
	{
		m_mapEditor->m_map->ChangeTileAtTilePos(m_tilesToChange.at(i), m_selectedSpriteInfo);
	}

	m_tilesToChange.clear();
}

//-----------------------------------------------------------------------------------------------
bool TileEditor::IsTileInsideSector(const IntVector2& min, const IntVector2& max, const IntVector2& pointToCheck)
{
	return (pointToCheck.x >= min.x) && (pointToCheck.x <= max.x)
		&& (pointToCheck.y >= min.y) && (pointToCheck.y <= max.y);
}

//-----------------------------------------------------------------------------------------------
void TileEditor::RenderUI() const
{
	Renderer* r = Renderer::GetInstance();

	SpriteSheet* selectedSpriteSheet = g_allSpriteSheets[m_selectedSpriteSheet];

	RenderSpriteSheetButtons();

	// tile select
	r->DrawAABB2Filled(m_tileSelectBounds, Rgba(0, 0, 0, 100));
	r->DrawTexturedAABB2(m_tileSelectBounds, *selectedSpriteSheet->m_texture, Vector2(0, 0), Vector2(1, 1), Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(m_tileSelectBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_tileSelectBounds, 1.f, 1.f), Rgba(0, 0, 0, 255));

	// options
	r->DrawAABB2Outline(m_mapEditor->m_optionsBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_mapEditor->m_optionsBounds, 1.f, 1.f), Rgba(0, 0, 0, 255));

	// sprite preview (little box)
	AABB2 spritePreviewUVs = selectedSpriteSheet->GetTexCoordsForSpriteIndex(m_selectedSpriteInfo.GetSpriteIndex());
	r->DrawTexturedAABB2(m_selectedTilePreviewBounds, *selectedSpriteSheet->m_texture, spritePreviewUVs.mins, spritePreviewUVs.maxs, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(m_selectedTilePreviewBounds, Rgba(255, 255, 255, 255));
	r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_selectedTilePreviewBounds, 1.f, 1.f), Rgba(0, 0, 0, 255));

	// zoom in
	Vector2 mousePos = GetMousePosition(m_mapEditor->m_cameraBounds);
	if (m_tileSelectBounds.IsPointInBox(mousePos))
	{
		int index = selectedSpriteSheet->GetSpriteIndexFromPositionInBounds(mousePos, m_tileSelectBounds);
		AABB2 uvs = selectedSpriteSheet->GetTexCoordsForSpriteIndex(index);
		r->DrawTexturedAABB2(m_tilePreviewBounds, *selectedSpriteSheet->m_texture, uvs.mins, uvs.maxs, Rgba(255, 255, 255, 255));
		r->DrawAABB2Outline(m_tilePreviewBounds);
		r->DrawAABB2Outline(GetBiggerAABB2FromAABB2(m_tilePreviewBounds, 1.f, 1.f), Rgba(0, 0, 0, 255));

		r->DrawLine2D(mousePos, m_tilePreviewBounds.mins, Rgba(255, 255, 255, 255));
		r->DrawLine2D(mousePos, Vector2(m_tilePreviewBounds.maxs.x, m_tilePreviewBounds.mins.y), Rgba(255, 255, 255, 255));

		RenderTileSelectHoverOutline();
	}
	else if(m_spriteSheetIndexBounds.IsPointInBox(mousePos) == false)
	{
		// Draw the box outline on the current hovered tile
		if (m_mapEditor->m_map->GetBounds().IsPointInBox(mousePos))
		{
			IntVector2 tilePos = Vector2(mousePos.x / 16, mousePos.y / 16).GetAsIntVector2();

			AABB2 highlightBounds = AABB2(tilePos.x * TILE_SIZE, tilePos.y * TILE_SIZE, (tilePos.x + 1) * TILE_SIZE, (tilePos.y + 1) * TILE_SIZE);
			r->DrawAABB2Outline(highlightBounds, Rgba(0, 0, 0, 255));
		}
	}

	RenderOptionsBar();
}

//-----------------------------------------------------------------------------------------------
void TileEditor::RenderTilePlacementPreview() const
{
	if (IsKeyPressed(KEYBOARD_LSHIFT))
	{
		Mesh* tempMesh = m_tileSelectPreviewMB->CreateMesh<Vertex3D_PCU>();
		Renderer::GetInstance()->DrawMesh(tempMesh, true);
	}
}

//-----------------------------------------------------------------------------------------------
void TileEditor::RenderTileSelectHoverOutline() const
{
	Vector2 mousePos = GetMousePosition(m_mapEditor->m_cameraBounds);
	Renderer* r = Renderer::GetInstance();

	SpriteSheet* selectedSpriteSheet = g_allSpriteSheets[m_selectedSpriteSheet];
	
	// Draw little black box on the tile select bounds
	IntVector2 spriteCoords;
	spriteCoords.x = (int)RangeMapFloat(mousePos.x, m_tileSelectBounds.mins.x, m_tileSelectBounds.maxs.x, 0.f, (float)selectedSpriteSheet->m_spriteLayout.x);
	spriteCoords.y = (int)RangeMapFloat(mousePos.y, m_tileSelectBounds.mins.y, m_tileSelectBounds.maxs.y, (float)selectedSpriteSheet->m_spriteLayout.y, 0.f);
	spriteCoords.x = ClampInt(spriteCoords.x, 0, (selectedSpriteSheet->m_spriteLayout.x - 1));
	spriteCoords.y = ClampInt(spriteCoords.y, 0, (selectedSpriteSheet->m_spriteLayout.y - 1));

	float minX = RangeMapFloat((float)spriteCoords.x, 0.f, (float)selectedSpriteSheet->m_spriteLayout.x, m_tileSelectBounds.mins.x, m_tileSelectBounds.maxs.x);
	float maxX = RangeMapFloat((float)(spriteCoords.x + 1), 0.f, (float)selectedSpriteSheet->m_spriteLayout.x, m_tileSelectBounds.mins.x, m_tileSelectBounds.maxs.x);
	float minY = RangeMapFloat((float)spriteCoords.y, 0.f, (float)selectedSpriteSheet->m_spriteLayout.y, m_tileSelectBounds.maxs.y, m_tileSelectBounds.mins.y);
	float maxY = RangeMapFloat((float)(spriteCoords.y + 1.f), 0.f, (float)selectedSpriteSheet->m_spriteLayout.y, m_tileSelectBounds.maxs.y, m_tileSelectBounds.mins.y);

	r->DrawAABB2Outline(AABB2(minX, minY, maxX, maxY), Rgba(0, 0, 0, 255));
}

//-----------------------------------------------------------------------------------------------
void TileEditor::RenderSpriteSheetButtons() const
{
	Renderer* r = Renderer::GetInstance();
	
	// spritesheet buttons
	r->DrawAABB2Outline(m_spriteSheetIndexBounds, Rgba::WHITE);

	for (int i = 0; i < MAX_AMOUNT_OF_SPRITE_SHEETS; i++)
	{
		AABB2 currentBounds = m_spriteSheetButtonBounds[i];
		Rgba backgroundColor = (m_selectedSpriteSheet == i) ? Rgba(100, 100, 100, 155) : Rgba::STRAWBERRY_RED;

		r->DrawAABB2Filled(currentBounds, backgroundColor);
		r->DrawAABB2Outline(currentBounds, Rgba::WHITE);
		r->DrawTextInBox(std::to_string(i), currentBounds, 6.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	}

	AABB2 selectedBounds = m_spriteSheetButtonBounds[m_selectedSpriteSheet];
	r->DrawAABB2Outline(selectedBounds, Rgba::GREEN);
}

//-----------------------------------------------------------------------------------------------
void TileEditor::RenderOptionsBar() const
{
	Renderer* r = Renderer::GetInstance();
	
	AABB2 optionBounds = m_mapEditor->m_optionsBounds;

	r->DrawAABB2Filled(optionBounds, Rgba::STRAWBERRY_RED);
	r->DrawTextInBox(GetTileDrawModeAsString(m_drawMode), optionBounds, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
}
