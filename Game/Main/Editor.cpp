#include "Editor.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Camera.hpp"

//===============================================================================================
Editor* g_theEditor = nullptr;  

//===============================================================================================


//-----------------------------------------------------------------------------------------------
void Editor::Update()
{
}

//-----------------------------------------------------------------------------------------------
void Editor::Render() const
{
	Renderer* r = Renderer::GetInstance();

	r->m_clearScreen = true;
	r->SetCamera(r->m_defaultUICamera);
	r->SetShader(r->m_defaultShader);
	r->SetCurrentTexture();

	AABB2 cameraBounds = r->m_defaultUICamera->GetOrthoBounds();
	r->DrawAABB2Filled(cameraBounds, Rgba(255,0,0,255));
}
