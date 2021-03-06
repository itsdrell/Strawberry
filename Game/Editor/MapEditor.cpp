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
#include "Game/Editor/TileEditor.hpp"
#include "Game/Editor/CollisionEditor.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"


//===============================================================================================
MapEditor::MapEditor()
{
	m_map = new Map(IntVector2(128, 128));

	Renderer* r = Renderer::GetInstance();
	m_camera = new Camera();
	m_camera->SetColorTarget(r->m_defaultColorTarget);
	m_camera->SetProjectionOrthoByAspect(Window::GetInstance()->GetHeight() * DEFAULT_ZOOMED_IN_LEVEL);
	r->SetCamera();

	m_mapEditorModes[TILE_EDITOR_MODE] = new TileEditor(this);
	m_mapEditorModes[COLLISION_EDITOR_MODE] = new CollisionEditor(this);

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
	HandleInput();
	GenerateAllBounds();
	m_mapEditorModes[m_currentMode]->Update();
	
	// needs to be last since it handles cleaning up the mesh if it gets dirty
	m_map->Update();
}

//-----------------------------------------------------------------------------------------------
void MapEditor::HandleInput()
{
	if (IsKeyPressed(KEYBOARD_CTRL) && WasKeyJustPressed('s'))
	{
		DebugRenderLog("MAP SAVED", 3.f);
		m_map->SaveMap();
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
void MapEditor::SetMode(MapEditorMode theMode)
{
	m_mapEditorModes[m_currentMode]->Exit();
	m_currentMode = theMode;
	m_mapEditorModes[m_currentMode]->Enter();
}

//-----------------------------------------------------------------------------------------------
void MapEditor::RenderUI() const
{
	Renderer* r = Renderer::GetInstance();

	// mouse
	Vector2 mousePos = GetMousePosition(m_cameraBounds);
	//r->DrawCircleOutline2D(mousePos, 1.f, Rgba(0, 255, 0, 255));

	// print mousePos
	if (m_map->GetBounds().IsPointInBox(mousePos))
	{
		IntVector2 tilePos = Vector2(mousePos.x / 16, mousePos.y / 16).GetAsIntVector2();
		IntVector2 worldPos = mousePos.GetAsIntVector2();
		String positions = "T(" + tilePos.ToString() + ") W(" + worldPos.ToString() + ")";
		
		// Kinda hacky way to get the font to not be really small based on the zoomed in level :l 
		AABB2 textBox = GetAABB2FromAABB2(Vector2(.31f, .01f), Vector2(.6f, .05f), m_cameraBounds);
		float textSize = RangeMapFloat(m_currentZoom, MIN_ZOOMED_IN_LEVEL, MAX_ZOOMED_OUT_LEVEL, 2, 8);
		r->DrawText2D(textBox.mins, positions, textSize);	
	}
}

//-----------------------------------------------------------------------------------------------
void MapEditor::KeyboardMovement()
{
	if (IsKeyPressed(KEYBOARD_CTRL))
		return;
	
	float stepSize = 16.f;
	float speed = 10.f;
	Vector2 dir;

	if (IsKeyPressed(KEYBOARD_SPACE))
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
void MapEditor::GenerateAllBounds()
{
	m_cameraBounds = m_camera->GetOrthoBounds();
	m_optionsBounds = GetAABB2FromAABB2(Vector2(0.f, 0.f), Vector2(.3f, .1f), m_cameraBounds);
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
	r->DrawAABB2Filled(m_cameraBounds, Rgba(255, 20, 147, 255));

	m_map->Render(m_showGrid);
	
	m_mapEditorModes[m_currentMode]->Render();
	RenderUI();

	r->SetCamera();
}


