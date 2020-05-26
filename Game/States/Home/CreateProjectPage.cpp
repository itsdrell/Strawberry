#include "CreateProjectPage.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/States/Home/Home.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Game/Main/TheApp.hpp"

//===============================================================================================
CreateProjectPage::CreateProjectPage(Home* ownerPage)
{
	m_ownerPage = ownerPage;

	GenerateBounds();
}

//-----------------------------------------------------------------------------------------------
void CreateProjectPage::Update()
{
	HandleInput();
}

//-----------------------------------------------------------------------------------------------
void CreateProjectPage::Render() const
{
	Renderer* r = Renderer::GetInstance();
	Vector2 mousePos = GetMousePosition(Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds());

	//bg
	r->DrawAABB2Filled(m_bounds, Rgba::STRAWBERRY_RED);
	r->DrawAABB2Outline(m_bounds);

	// help text
	r->DrawTextInBox("Project Name", m_helperTextNameBounds, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));

	// text box
	r->DrawAABB2Outline(m_textBoxBounds);
	r->DrawTextInBox(m_enteredName, m_textBoxBounds, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));

	// create
	if(m_enteredName.empty() == false)
	{
		Rgba color = m_createButtonBounds.IsPointInBox(mousePos) ? Rgba::GREEN : Rgba::WHITE;
		r->DrawAABB2Outline(m_createButtonBounds, color);
		r->DrawTextInBox("Create!", m_createButtonBounds, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	}
	
	// exit
	Rgba color = m_exitButtonBounds.IsPointInBox(mousePos) ? Rgba::GREEN : Rgba::WHITE;
	r->DrawAABB2Outline(m_exitButtonBounds, color);
	r->DrawTextInBox("X", m_exitButtonBounds, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	


}

//-----------------------------------------------------------------------------------------------
void CreateProjectPage::GenerateBounds()
{
	m_bounds = GetAABB2FromAABB2(Vector2(.2f, .2f), Vector2(.8f, .8f), Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds());

	m_textBoxBounds = GetAABB2FromAABB2(Vector2(.1f, .4f), Vector2(.9f,.6f), m_bounds);
	m_helperTextNameBounds = GetAABB2FromAABB2(Vector2(.1f, .65f), Vector2(.9f, .75f), m_bounds);

	m_createButtonBounds = GetAABB2FromAABB2(Vector2(.3f, .05f), Vector2(.7f, .15f), m_bounds);
	m_exitButtonBounds = GetAABB2FromAABB2(Vector2(.85f,.85f), Vector2(.95f, .95f), m_bounds);
}

//-----------------------------------------------------------------------------------------------
void CreateProjectPage::HandleInput()
{
	HandleTextInput();

	bool wasMousePressed = WasMouseButtonJustPressed(LEFT_MOUSE_BUTTON);
	Vector2 mousePos = GetMousePosition(Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds());
	
	if (WasKeyJustPressed(KEYBOARD_ESC) || (m_exitButtonBounds.IsPointInBox(mousePos) && wasMousePressed))
	{
		m_ownerPage->ClosePopup();
	}

	if(WasKeyJustPressed(KEYBOARD_ENTER) || (m_createButtonBounds.IsPointInBox(mousePos) && wasMousePressed))
	{
		CommandRun("create " + m_enteredName); // create the project

		DebugRenderLog("Project: " + m_enteredName + " created!", 3);

		g_theApp->LoadAndRunGame(m_enteredName);

		m_ownerPage->ClosePopup();
	}
}

//-----------------------------------------------------------------------------------------------
void CreateProjectPage::HandleTextInput()
{
	if (WasKeyJustPressed(KEYBOARD_BACKSPACE) && m_enteredName.empty() == false)
	{
		m_enteredName.erase(m_enteredName.begin() + (m_enteredName.size() - 1));
	}

	String input = g_theInputSystem->GetTextInputedThisFrame();
	if (input.empty() == false)
		m_enteredName += input;
}
