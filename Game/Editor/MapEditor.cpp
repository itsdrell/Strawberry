#include "MapEditor.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/General/Map/Map.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Core/Platform/Window.hpp"

//===============================================================================================
MapEditor::MapEditor()
{
	m_map = new Map(IntVector2(4, 4));

	Renderer* r = Renderer::GetInstance();
	m_camera = new Camera();
	m_camera->SetColorTarget(r->m_defaultColorTarget);
	m_camera->SetProjectionOrthoByAspect(Window::GetInstance()->GetHeight() * .5f);
	r->SetCamera();
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
}

//-----------------------------------------------------------------------------------------------
void MapEditor::HandleInput()
{
}

//-----------------------------------------------------------------------------------------------
void MapEditor::Render() const
{
	Renderer* r = Renderer::GetInstance();

	r->m_clearScreen = true;
	r->SetCamera(m_camera);
	r->SetShader(r->m_defaultShader);
	r->SetCurrentTexture();

	AABB2 cameraBounds = m_camera->GetOrthoBounds();
	r->DrawAABB2Filled(cameraBounds, Rgba(255, 20, 147, 255));

	AABB2 tileBorder = GetAABB2FromAABB2(Vector2(0.f, 0.f), Vector2(.3f, 1.f), cameraBounds);
	r->DrawAABB2Outline(tileBorder, Rgba(255, 255, 255, 255));

	AABB2 optionsBorder = GetAABB2FromAABB2(Vector2(0.f, 0.f), Vector2(.3f, .1f), cameraBounds);
	r->DrawAABB2Outline(optionsBorder, Rgba(255, 255, 255, 255));

	m_map->Render();
	
	r->DrawCircleFilled2D(GetMousePosition(cameraBounds), 4.f, Rgba(0, 255, 0, 255));


	r->SetCamera();
}
