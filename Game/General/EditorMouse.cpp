#include "EditorMouse.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"

//===============================================================================================
EditorMouse* EditorMouse::s_theMouse = nullptr;

//===============================================================================================
EditorMouse::EditorMouse()
{
	s_theMouse = this;
}

//-----------------------------------------------------------------------------------------------
EditorMouse::~EditorMouse()
{
	s_theMouse = nullptr;
}

//-----------------------------------------------------------------------------------------------
void EditorMouse::BeginFrame()
{
	m_onClickable = false;
}

//-----------------------------------------------------------------------------------------------
void EditorMouse::Update()
{
	Renderer* r = Renderer::GetInstance();
	m_postion = GetMousePosition(r->m_defaultUICamera->GetOrthoBounds());

	// The pointer needs an offset since the sprite is centered but the normal one 
	// isn't and is more left aligned
	if (m_onClickable)
	{
		m_postion.x -= .01f;
	}
}

//-----------------------------------------------------------------------------------------------
void EditorMouse::Render() const
{
	Renderer* r = Renderer::GetInstance();
	r->SetCamera(r->m_defaultUICamera);

	float offset = .05f;
	int index = m_onClickable ? 1 : 0;

	AABB2 uvs = g_editorIconSpriteSheet->GetTexCoordsForSpriteIndex(index);
	AABB2 bounds = AABB2(Vector2(m_postion.x, m_postion.y - offset), Vector2(m_postion.x + offset, m_postion.y));

	r->DrawTexturedAABB2(bounds, *g_editorIconSpriteSheet->m_texture, uvs.mins, uvs.maxs, Rgba::WHITE);
}