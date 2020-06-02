#include "Editor.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Game/Editor/SpriteSheetViewer.hpp"
#include "Game/Editor/MapEditor.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "Game/Main/TheApp.hpp"
#include "Game/Editor/CurveEditor.hpp"

//===============================================================================================
Editor* g_theEditor = nullptr;  

//===============================================================================================
Editor::Editor()
{
	m_editorModes[SPRITE_SHEET_VIEW_MODE] = new SpriteSheetView();
	m_editorModes[MAP_EDITOR_MODE] = new MapEditor();
	m_editorModes[CURVE_EDITOR_MODE] = new CurveEditor();

	g_theApp->m_states[APPSTATE_EDITOR] = this;
}

//-----------------------------------------------------------------------------------------------
Editor::~Editor()
{
	for (uint i = 0; i < NUM_OF_EDITOR_MODES; i++)
	{
		delete m_editorModes[i];
		m_editorModes[i] = nullptr;
	}

	g_theApp->m_states[APPSTATE_EDITOR] = nullptr;
}

//-----------------------------------------------------------------------------------------------
void Editor::Update()
{
	HandleInput();
	m_editorModes[m_currentMode]->Update();
}

//-----------------------------------------------------------------------------------------------
void Editor::Render() const
{
	m_editorModes[m_currentMode]->Render();
}

//-----------------------------------------------------------------------------------------------
void Editor::HandleInput()
{
	KeyboardInput();
}

//-----------------------------------------------------------------------------------------------
void Editor::KeyboardInput()
{
	if (Console::GetInstance()->IsOpen())
		return;
	
	if (WasKeyJustPressed(KEYBOARD_RIGHT_ARROW))
	{
		m_currentMode = (TypesOfEditorModes)((m_currentMode + 1) % NUM_OF_EDITOR_MODES);
	}

	if (WasKeyJustPressed(KEYBOARD_LEFT_ARROW))
	{
		if (m_currentMode == 0)
			m_currentMode = (TypesOfEditorModes)(NUM_OF_EDITOR_MODES - 1);
		else
			m_currentMode = (TypesOfEditorModes)(m_currentMode - 1);
	}

	if (WasKeyJustPressed('1'))
	{
		m_currentMode = (TypesOfEditorModes)(0);
	}

	if (WasKeyJustPressed('2'))
	{
		m_currentMode = (TypesOfEditorModes)(1);
		MapEditor* mapEditor = (MapEditor*)m_editorModes[MAP_EDITOR_MODE];
		mapEditor->SetMode(TILE_EDITOR_MODE);
	}

	if (WasKeyJustPressed('3'))
	{
		m_currentMode = (TypesOfEditorModes)(1);
		MapEditor* mapEditor = (MapEditor*)m_editorModes[MAP_EDITOR_MODE];
		mapEditor->SetMode(COLLISION_EDITOR_MODE);
	}

	if (WasKeyJustPressed('4'))
	{
		m_currentMode = (TypesOfEditorModes)(2);
	}
}
