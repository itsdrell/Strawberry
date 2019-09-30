#include "CollisionEditor.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Editor/MapEditor.hpp"


//===============================================================================================
CollisionEditor::CollisionEditor( MapEditor * theMapEditor)
{
	m_mapEditor = theMapEditor;
}

//-----------------------------------------------------------------------------------------------
CollisionEditor::~CollisionEditor()
{
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::Update()
{
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::Render() const
{
	Renderer* r = Renderer::GetInstance();

	r->DrawAABB2Filled(m_mapEditor->m_cameraBounds, Rgba::STRAWBERRY_RED);
}

//-----------------------------------------------------------------------------------------------
void CollisionEditor::HandleInput()
{
}
