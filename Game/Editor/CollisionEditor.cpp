#include "CollisionEditor.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Editor/MapEditor.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/General/BlackBoard.hpp"

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

	r->DrawWrappedTextInBox2D(m_nameOfChannel, m_textBounds, m_textBounds.GetHeight() * .2f);
}

//===============================================================================================
CollisionEditor::CollisionEditor( MapEditor * theMapEditor)
{
	m_mapEditor = theMapEditor;

	GenerateAllBounds();
}

//-----------------------------------------------------------------------------------------------
CollisionEditor::~CollisionEditor()
{
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

	for (uint i = 0; i < MAX_AMOUNT_OF_COLLISION_CHANNELS; i++)
	{
		m_channelButtons[i].Render();
	}

	r->DrawAABB2Outline(m_channelSelectBounds, Rgba::WHITE);
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::HandleInput()
{
	Vector2 mousePos = GetMousePosition(m_mapEditor->m_cameraBounds);
	
	if (WasMouseButtonJustPressed(LEFT_MOUSE_BUTTON))
	{
		for (uint i = 0; i < MAX_AMOUNT_OF_COLLISION_CHANNELS; i++)
		{
			m_channelButtons[i].OnLeftClick(mousePos);
		}
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
void CollisionEditor::UpdateNames()
{
	m_channelButtons[0].m_nameOfChannel = g_theGameBlackboard->GetValue("channelOneName", "Channel 1");
	m_channelButtons[1].m_nameOfChannel = g_theGameBlackboard->GetValue("channelTwoName", "Channel 2");
	m_channelButtons[2].m_nameOfChannel = g_theGameBlackboard->GetValue("channelThreeName", "Channel 3");
	m_channelButtons[3].m_nameOfChannel = g_theGameBlackboard->GetValue("channelFourName", "Channel 4");
	m_channelButtons[4].m_nameOfChannel = g_theGameBlackboard->GetValue("channelFiveName", "Channel 5");
}
