#pragma once
#include "EditorHistory.hpp"
#include "Game/Editor/EditorMode.hpp"
#include "Game/Editor/TileEditor.hpp"

//===============================================================================================
TileChangeAction::TileChangeAction(const TileSpriteInfo& previousInfo, const IntVector2& tilePos, Map* theMap)
{
	m_previousInfo = previousInfo;
	m_tilePos = tilePos;
	m_map = theMap;
}

//-----------------------------------------------------------------------------------------------
void TileChangeAction::Undo()
{
	m_map->ChangeTileAtTilePos(m_tilePos, m_previousInfo);
}

//===============================================================================================
MultiTileChangeAction::MultiTileChangeAction(const std::vector<TileChangeAction*>& tilesToChange)
{
	m_tilesToChange = tilesToChange;
}

//-----------------------------------------------------------------------------------------------
MultiTileChangeAction::MultiTileChangeAction(const std::vector<TileChangeAction*>& tilesToChange, const Vector2& lastPosition, EditorMode* editor)
{
	m_tilesToChange = tilesToChange;
	m_lastDrawnPosition = lastPosition;
	m_editor = editor;
}

//-----------------------------------------------------------------------------------------------
void MultiTileChangeAction::Undo()
{
	for(uint i = 0; i < m_tilesToChange.size(); i++)
	{
		TileChangeAction*& current = m_tilesToChange.at(i);
		current->Undo();

		delete current;
		current = nullptr;
	}

	if(m_editor != nullptr)
	{
		TileEditor* tileEditor = (TileEditor*)m_editor;
		tileEditor->m_lastSelectedTilePosition = m_lastDrawnPosition;
	}
}
