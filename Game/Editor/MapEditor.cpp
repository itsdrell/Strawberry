#include "MapEditor.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Input/InputSystem.hpp"

//===============================================================================================
void MapEditor::Update()
{
}

//-----------------------------------------------------------------------------------------------
void MapEditor::Render() const
{
	Renderer* r = Renderer::GetInstance();

	r->m_clearScreen = true;
	r->SetCamera(r->m_defaultUICamera);
	r->SetShader(r->m_defaultShader);
	r->SetCurrentTexture();

	AABB2 cameraBounds = r->m_defaultUICamera->GetOrthoBounds();
	r->DrawAABB2Filled(cameraBounds, Rgba(255, 255, 0, 255));
	r->DrawCircleFilled2D(GetMousePosition(cameraBounds), .05f, Rgba(0, 255, 0, 255));
}
