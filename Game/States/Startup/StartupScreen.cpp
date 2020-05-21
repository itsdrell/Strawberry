#include "StartupScreen.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Game/Main/TheApp.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"
#include "Engine/Core/General/BlackBoard.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"

//===============================================================================================
StartupScreen::StartupScreen()
{

}

//-----------------------------------------------------------------------------------------------
void StartupScreen::Update()
{
	if(m_loadTimer.HasElapsed())
	{
		DecideWhereToGoAfterLoading();
	}
}

//-----------------------------------------------------------------------------------------------
void StartupScreen::Render() const
{
	// camera is -1,1
	Renderer* r = Renderer::GetInstance();

	float t = m_loadTimer.GetNormalizedElapsedTime();
	float size = Interpolate(-1, 1, t);

	r->DrawAABB2Filled(AABB2(-1, -1, 1, 1), Rgba::STRAWBERRY_RED);
	
	AABB2 bounds = AABB2(-1, -.2, size, .2);
	r->DrawAABB2Filled(bounds, Rgba(0,255,0,255));
	r->DrawAABB2Outline(bounds, Rgba::WHITE);

}

//-----------------------------------------------------------------------------------------------
void StartupScreen::OnEnter()
{
	m_loadTimer = Timer();
	m_loadTimer.SetTimer(c_STARTUP_SCREEN_LENGTH);
}

//-----------------------------------------------------------------------------------------------
void StartupScreen::OnExit()
{
	
}

//-----------------------------------------------------------------------------------------------
void StartupScreen::DecideWhereToGoAfterLoading()
{
#ifndef EMSCRIPTEN_PORT
	String startupGame = g_theEngineBlackboard->GetValue("startupGame", "idk");

	// in our export script we will set the flag to true
	if (g_theEngineBlackboard->GetValue("release", false))
	{
		BlackBoard temp = BlackBoard("Data/NameOfGame.lua", DATA_BLACKBOARD);
		startupGame = temp.GetValue("gameName", "idk");
		g_theApp->m_isReleaseVersion = true;
	}

	if (startupGame != "idk")
	{
		g_currentProjectName = startupGame;
		PrintLog("Project Name is: " + g_currentProjectName);
		g_theApp->ReloadAndRunGame();

		g_theApp->TransitionToState(APPSTATE_GAME);
	}
	else
	{
		g_theApp->TransitionToState(APPSTATE_HOME);
	}

#else
	BlackBoard temp = BlackBoard("Data/Web/NameOfGame.lua", DATA_BLACKBOARD);
	g_currentProjectName = temp.GetValue("gameName", "idk");
	PrintLog("Project Name is: " + g_currentProjectName);
	g_theApp->ReloadAndRunGame();
	g_theApp->m_isReleaseVersion = true;

	g_theApp->TransitionToState(APPSTATE_GAME);
#endif
}

