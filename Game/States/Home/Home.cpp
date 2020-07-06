#include "Home.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Core/General/BlackBoard.hpp"
#include "Game/Main/TheApp.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Core/Utils/StrawberryFileUtils.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "CreateProjectPage.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include "Engine/Math/MathUtils.hpp"


//===============================================================================================
Home::Home()
{
	GenerateBounds();
	
	m_fadeAwayTimer = StopWatch(g_theMasterClock);
	m_fadeAwayTimer.SetTimer(m_fadeAwayLength);

	m_randomBackgroundColor = Rgba::RAINBOW_VIOLET;
}

//-----------------------------------------------------------------------------------------------
Home::~Home()
{
	if (m_createPagePopup != nullptr)
		delete m_createPagePopup;

	m_createPagePopup = nullptr;
}

//-----------------------------------------------------------------------------------------------
void Home::Update()
{
	// if the pop up is up, update that only
	if(m_createPagePopup != nullptr)
	{
		m_createPagePopup->Update();
	}
	else
	{
		HandleInput();
	}
}

//-----------------------------------------------------------------------------------------------
void Home::Render() const
{
	Renderer* r = Renderer::GetInstance();
	r->m_clearScreen = true;
	r->SetCamera(r->m_defaultUICamera);

	RenderUI();

	// Draw the popup over it
	if(m_createPagePopup != nullptr)
	{
		r->DrawAABB2Filled(r->m_defaultUICamera->GetOrthoBounds(), Rgba(0, 0, 0, 220));
		m_createPagePopup->Render();
	}

	// mouse
	Vector2 mousePos = GetMousePosition(r->m_defaultUICamera->GetOrthoBounds());
	r->DrawCircleOutline2D(mousePos, .01f, Rgba(0, 255, 0, 255));

	// transition
	float t = m_fadeAwayTimer.GetNormalizedElapsedTime();
	float radius = Interpolate(1.5f, 0.0f, t);
	r->DrawCircleFilled2D(Vector2(0.75f, 0.5f), radius, Rgba::STRAWBERRY_RED);

	r->SetCamera();
}

//-----------------------------------------------------------------------------------------------
void Home::RenderUI() const
{
	Renderer* r = Renderer::GetInstance();
	
	AABB2 bounds = r->m_defaultUICamera->GetOrthoBounds();
	Vector2 mousePos = GetMousePosition(Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds());

	ProjectData currentProjectInfo = m_allProjectsData.at(m_currentIndex);

	// BG
	r->DrawAABB2Filled(bounds, Rgba::STRAWBERRY_RED);

	// cover
	if (currentProjectInfo.m_hasCover)
	{
		r->DrawAABB2Outline(m_coverImageBounds, Rgba::WHITE);

		String coverImagePath = "Projects/" + currentProjectInfo.m_name + "/Cover.png";
		r->DrawTexturedAABB2(m_coverImageBounds, *r->CreateOrGetTexture(coverImagePath), Vector2(0, 0), Vector2(1, 1), Rgba::WHITE);
	}
	else if (m_currentIndex == 0) // new project
	{
		Rgba color = m_coverImageBounds.IsPointInBox(mousePos) ? Rgba::GREEN : Rgba::WHITE;
		r->DrawAABB2Outline(m_coverImageBounds, color);
		r->DrawTextInBox("+", m_coverImageBounds, 100.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	}
	else
	{
		r->DrawAABB2Outline(m_coverImageBounds, Rgba::WHITE);
		r->DrawAABB2Filled(m_coverImageBounds, m_randomBackgroundColor);

		r->DrawTextInBox("Make a screenshot \nwith F9!:)", m_coverImageBounds, 8.f, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	}


	// title
	String title = currentProjectInfo.m_name;
	//r->DrawAABB2Outline(m_gameTitleBounds, Rgba::WHITE);
	r->DrawTextInBox(title, m_gameTitleBounds, 8, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));

	// load
	if(m_currentIndex != 0)
	{
		Rgba color = m_loadGameBounds.IsPointInBox(mousePos) ? Rgba::GREEN : Rgba::WHITE;
		r->DrawAABB2Outline(m_loadGameBounds, color);
		r->DrawTextInBox("Load", m_loadGameBounds, 8, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	}

	// arrows
	if (m_currentIndex != (m_allProjectsData.size() - 1))
	{
		Rgba color = m_rightArrowBounds.IsPointInBox(mousePos) ? Rgba::GREEN : Rgba::WHITE;
		
		r->DrawAABB2Outline(m_rightArrowBounds, color);
		r->DrawTextInBox(">", m_rightArrowBounds, 8, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	}

	if (m_currentIndex != 0)
	{
		Rgba color = m_leftArrowBounds.IsPointInBox(mousePos) ? Rgba::GREEN : Rgba::WHITE;
		
		r->DrawAABB2Outline(m_leftArrowBounds, color);
		r->DrawTextInBox("<", m_leftArrowBounds, 8, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	}
}

//-----------------------------------------------------------------------------------------------
void Home::OnEnter()
{
	m_allProjectsData.clear();
	
	GenerateAllProjectData();
	PutTheStartUpFunctionAtFront();

	m_fadeAwayTimer.Reset();
}

//-----------------------------------------------------------------------------------------------
void Home::OnExit()
{

}

//-----------------------------------------------------------------------------------------------
void Home::GenerateBounds()
{
	AABB2 cameraBounds = Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds();
	
	m_coverImageBounds = GetAABB2FromAABB2(Vector2(.2f,.2f), Vector2(.8f,.8f), cameraBounds);
	m_gameTitleBounds = GetAABB2FromAABB2(Vector2(.2f, .85f), Vector2(.8f, .95f), cameraBounds);
	
	m_loadGameBounds = GetAABB2FromAABB2(Vector2(.4f, .05f), Vector2(.6f, .15f), cameraBounds);
	
	m_leftArrowBounds = GetAABB2FromAABB2(Vector2(.05f, .4f), Vector2(.15f, .6f), cameraBounds);
	m_rightArrowBounds = GetAABB2FromAABB2(Vector2(.85f, .4f), Vector2(.95f, .6f), cameraBounds);

}

//-----------------------------------------------------------------------------------------------
void Home::GenerateAllProjectData()
{
	Strings projectNames;
	GetAllProjectNames(&projectNames);

	m_allProjectsData.push_back(ProjectData(String("New Project"), false));

	for each(String project in projectNames)
	{
		bool doesHaveCoverImage = DoesProjectHaveCoverImage(project);

		ProjectData newData = ProjectData(project, doesHaveCoverImage);
		m_allProjectsData.push_back(newData);
	}

	if(m_allProjectsData.size() > 0)
	{
		m_currentIndex = 1;
	}
}

//-----------------------------------------------------------------------------------------------
void Home::PutTheStartUpFunctionAtFront()
{
	String startupName = g_theEngineBlackboard->GetValue("startupGame", "idk");

	for (uint i = 0; i < m_allProjectsData.size(); i++)
	{
		ProjectData& current = m_allProjectsData.at(i);
		if (current.m_name == startupName)
		{
			ProjectData temp = m_allProjectsData.at(1);
			m_allProjectsData.at(1) = current;
			m_allProjectsData.at(i) = temp;
			return;
		}
	}
}

//-----------------------------------------------------------------------------------------------
void Home::HandleInput()
{
	if (Console::GetInstance()->IsOpen()) { return; }
	
	if (WasKeyJustPressed('d') || WasKeyJustPressed(KEYBOARD_RIGHT_ARROW) || DidMouseWheelScrollDown())
	{
		if (m_currentIndex != (m_allProjectsData.size() - 1))
		{
			m_currentIndex++;
		}

		m_randomBackgroundColor = GetNextColorInRainbow(m_randomBackgroundColor);
	}

	if (WasKeyJustPressed('a') || WasKeyJustPressed(KEYBOARD_LEFT_ARROW) || DidMouseWheelScrollUp())
	{
		if (m_currentIndex != 0)
		{
			m_currentIndex--;
		}

		m_randomBackgroundColor = GetNextColorInRainbow(m_randomBackgroundColor);
	}

	if(WasKeyJustReleased(KEYBOARD_ENTER))
	{
		if(m_currentIndex != 0)
		{
			g_theApp->LoadAndRunGame(m_allProjectsData.at(m_currentIndex).m_name);
		}
		else
		{
			m_createPagePopup = new CreateProjectPage(this);
		}
	}

	HandleMouseClicks();
}

//-----------------------------------------------------------------------------------------------
void Home::HandleMouseClicks()
{
	if(WasMouseButtonJustPressed(LEFT_MOUSE_BUTTON))
	{
		Vector2 mousePos = GetMousePosition(Renderer::GetInstance()->m_defaultUICamera->GetOrthoBounds());

		// right arrow
		if(m_rightArrowBounds.IsPointInBox(mousePos) && m_currentIndex != m_allProjectsData.size() - 1)
		{
			m_currentIndex++;
		}

		// left arrow
		if(m_leftArrowBounds.IsPointInBox(mousePos) && m_currentIndex != 0)
		{
			m_currentIndex--;
		}

		if (m_loadGameBounds.IsPointInBox(mousePos) && m_currentIndex != 0)
		{
			g_theApp->LoadAndRunGame(m_allProjectsData.at(m_currentIndex).m_name);
		}

		if(m_currentIndex == 0 && m_coverImageBounds.IsPointInBox(mousePos))
		{
			m_createPagePopup = new CreateProjectPage(this);
		}
	}
}

//-----------------------------------------------------------------------------------------------
void Home::ClosePopup()
{
	delete m_createPagePopup;
	m_createPagePopup = nullptr;
}
