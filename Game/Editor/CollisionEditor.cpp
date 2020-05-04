#include "CollisionEditor.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Editor/MapEditor.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/General/BlackBoard.hpp"
#include "Game/General/Map/Map.hpp"
#include "Engine/Renderer/Components/Mesh.hpp"
#include "Engine/Renderer/Systems/MeshBuilder.hpp"

//===============================================================================================
bool CollisionChannelButton::IsMousePositionOnButton(const Vector2& mousePos)
{
	return m_buttonBounds.IsPointInBox(mousePos);
}

//-----------------------------------------------------------------------------------------------
bool CollisionChannelButton::OnLeftClick(const Vector2& mousePos)
{
	if (IsMousePositionOnButton(mousePos))
	{
		m_isOn = !m_isOn;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
void CollisionChannelButton::UpdateBounds(const AABB2& textBox, const AABB2& buttonBounds)
{
	m_textBounds = textBox;
	m_buttonBounds = buttonBounds;
}

//-----------------------------------------------------------------------------------------------
void CollisionChannelButton::Render() const
{
	Renderer* r = Renderer::GetInstance();

	if (m_isOn)
	{
		r->DrawAABB2Filled(m_buttonBounds, Rgba(0, 255, 0, 255));
		r->DrawAABB2Filled(m_textBounds, Rgba(0, 0, 0, 255));
	}

	r->DrawAABB2Outline(m_buttonBounds, Rgba::WHITE);
	r->DrawAABB2Outline(m_textBounds, Rgba(255, 255, 255, 255));

	r->DrawTextInBox(m_nameOfChannel, m_textBounds, m_textBounds.GetHeight() * .2f, DRAW_TEXT_MODE_WRAPPED);
}

//===============================================================================================
CollisionEditor::CollisionEditor( MapEditor * theMapEditor)
{
	m_mapEditor = theMapEditor;

	m_meshBuilder = new MeshBuilder();

	GenerateAllBounds();
	GenerateCurrentCollisionChannelMesh();
}

//-----------------------------------------------------------------------------------------------
CollisionEditor::~CollisionEditor()
{
	delete m_meshBuilder;
	m_meshBuilder = nullptr;

	delete m_mesh;
	m_mesh = nullptr;
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::Update()
{
	HandleInput();
	GenerateAllBounds();
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::Render() const
{
	Renderer* r = Renderer::GetInstance();
	r->DrawAABB2Filled(m_mapEditor->m_map->GetBounds(), Rgba(0, 0, 0, 150));
	
	for (uint i = 0; i < MAX_AMOUNT_OF_COLLISION_CHANNELS; i++)
	{
		m_channelButtons[i].Render();
	}

	r->DrawAABB2Outline(m_channelSelectBounds, Rgba::WHITE);

	r->DrawMesh(m_mesh, false);
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::HandleInput()
{
	Vector2 mousePos = GetMousePosition(m_mapEditor->m_cameraBounds);
	
	if (IsMouseButtonPressed(LEFT_MOUSE_BUTTON))
	{
		LeftClick();
	}

	if (WasKeyJustPressed(KEYBOARD_F5))
	{
		UpdateNames();
	}
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::Enter()
{
	UpdateNames();
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::GenerateAllBounds()
{
	m_channelSelectBounds = GetAABB2FromAABB2(Vector2(0.f, 0.1f), Vector2(.3f, .55f), m_mapEditor->m_cameraBounds);
	m_channelSelectBounds.ShrinkToSquare();

	for (int i = 0; i < MAX_AMOUNT_OF_COLLISION_CHANNELS; i++)
	{
		float minPercent = (float)(MAX_AMOUNT_OF_COLLISION_CHANNELS - i- 1) / (float)MAX_AMOUNT_OF_COLLISION_CHANNELS;
		float maxPercent = (float)(MAX_AMOUNT_OF_COLLISION_CHANNELS - i) / (float)MAX_AMOUNT_OF_COLLISION_CHANNELS;

		AABB2 buttonBox = GetAABB2FromAABB2(Vector2(0.f, minPercent), Vector2(1.f, maxPercent), m_channelSelectBounds);
		AABB2 textBox = GetAABB2FromAABB2(Vector2(0.1f, .1f), Vector2(.9f, .9f), buttonBox);

		m_channelButtons[i].UpdateBounds(textBox, buttonBox);
	}
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::GenerateCurrentCollisionChannelMesh()
{
	Vector2 currentPos = Vector2(0.f, 0.f);
	Map& theMap = *m_mapEditor->m_map;
	m_meshBuilder->Clear();

	for (uint yIndex = 0; yIndex < (uint)theMap.m_dimensions.y; yIndex++)
	{
		for (uint xIndex = 0; xIndex < (uint)theMap.m_dimensions.x; xIndex++)
		{
			uint currentTileIndex = (theMap.m_dimensions.x * yIndex) + xIndex;
			Tile& currentTile = theMap.GetTileByIndex(currentTileIndex);
			int tileChannelValue = currentTile.m_spriteInfo.GetCollisionChannelValue();

			AABB2 currentBounds = AABB2(currentPos.x, currentPos.y, currentPos.x + TILE_SIZE, currentPos.y + TILE_SIZE);
			if (tileChannelValue == m_channelBits && tileChannelValue != 0)
			{
				m_meshBuilder->Add2DPlane(currentBounds, AABB2(), m_tileColor);
			}
			else
			{
				m_meshBuilder->Add2DPlane(currentBounds, AABB2(0.f, 0.f, 1.f, 1.f), Rgba(0, 0, 0, 50));
			}

			currentPos.x += TILE_SIZE;
		}

		currentPos.x = 0;
		currentPos.y += TILE_SIZE;
	}

	if (m_mesh != nullptr)
		delete m_mesh;
	
	m_mesh = m_meshBuilder->CreateMesh<Vertex3D_PCU>(false);
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::UpdateCollisionMesh(int index)
{
	index *= 4;

	m_meshBuilder->ChangeColorOfVertexAtPosition(index, m_tileColor);
	m_meshBuilder->ChangeColorOfVertexAtPosition(index + 1, m_tileColor);
	m_meshBuilder->ChangeColorOfVertexAtPosition(index + 2, m_tileColor);
	m_meshBuilder->ChangeColorOfVertexAtPosition(index + 3, m_tileColor);

	if (m_mesh)
		delete m_mesh;

	m_mesh = m_meshBuilder->CreateMesh<Vertex3D_PCU>(false);
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::UpdateNames()
{
	m_channelButtons[0].m_nameOfChannel = g_theGameBlackboard->GetValue("channelOneName", "Channel 1");
	m_channelButtons[1].m_nameOfChannel = g_theGameBlackboard->GetValue("channelTwoName", "Channel 2");
	m_channelButtons[2].m_nameOfChannel = g_theGameBlackboard->GetValue("channelThreeName", "Channel 3");
	m_channelButtons[3].m_nameOfChannel = g_theGameBlackboard->GetValue("channelFourName", "Channel 4");
	m_channelButtons[4].m_nameOfChannel = g_theGameBlackboard->GetValue("channelFiveName", "Channel 5");
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::LeftClick()
{
	Vector2 mousePos = GetMousePosition(m_mapEditor->m_cameraBounds);
	
	// see if we were touching a UI button
	for (uint i = 0; i < MAX_AMOUNT_OF_COLLISION_CHANNELS; i++)
	{
		CollisionChannelButton& current = m_channelButtons[i];
		if (current.OnLeftClick(mousePos))
		{
			// turn on bits
			if (current.m_isOn)
			{
				m_channelBits |= (1 << i);
			}
			else // off
			{
				Byte mask = ~(1 << i);
				m_channelBits &= mask;
			}

			GenerateCurrentCollisionChannelMesh();
			return; // ui gets priority "consumes" input
		}
	}

	// Check map
	AABB2 mapBounds = m_mapEditor->m_map->GetBounds();
	if (mapBounds.IsPointInBox(mousePos))
	{
		m_mapEditor->m_map->ChangeTilesCollisionChannel(mousePos, m_channelBits);
		UpdateCollisionMesh(m_mapEditor->m_map->GetTileIndexFromWorldPos(mousePos));
		return;
	}
}
