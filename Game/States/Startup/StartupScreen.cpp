#include "StartupScreen.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Game/Main/TheApp.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"
#include "Engine/Core/General/BlackBoard.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Math/Splines/CubicSpline.hpp"
#include "Engine/Input/InputSystem.hpp"


//===============================================================================================
StartupScreen::StartupScreen()
{
	m_strawberrySprite = new Sprite("Data/LoadingStrawberry.png", Vector2(.4f, .4f));
}

//-----------------------------------------------------------------------------------------------
void StartupScreen::Update()
{
	if(m_loadTimer.HasElapsed())
	{
		DecideWhereToGoAfterLoading();
	}

	if(WasKeyJustPressed('n'))
	{
		DecideWhereToGoAfterLoading();
	}
}

//-----------------------------------------------------------------------------------------------
void StartupScreen::Render() const
{
	// camera is -1,1
	Renderer* r = Renderer::GetInstance();

	r->DrawAABB2Filled(AABB2(-1.f, -1.f, 1.f, 1.f), Rgba::BLACK);

	float t = m_loadTimer.GetNormalizedElapsedTime();
	// background fill
	// could do random patters per start up? so sometimes it circle fill sometimes its a rectangle etc
	if (t > .5f)
	{
		float fillT = RangeMapFloat(t, .5f, .95f, 0.f, 1.f);
		float radius = Interpolate(0.f, 1.5f, fillT);
		r->DrawCircleFilled2D(Vector2(0.f, 0.f), radius, Rgba::STRAWBERRY_RED);
	}

	// strawberry bounce
	Vector2 pos;
	if(t < .5f)
	{
		float initT = RangeMapFloat(t, 0.f, .5f, 0.0f, 1.f);
		Vector2 goalLocation = Vector2(0, 0);
		Vector2 lerpAmout = EvaluateCubicBezier(Vector2(0.f, 0.f), Vector2(0.626f, 2.3f), Vector2(0.815f, 0.536f), Vector2(1.f, 1.f), initT);
		pos = Interpolate(Vector2(0.f, -1.f), Vector2(0, 0), lerpAmout.y);
	}
	else
	{
		pos = Vector2(0.f, 0.f);
	}
	r->DrawSprite(Vector3(pos.x, pos.y, 0.f), *m_strawberrySprite);

	// loading text
	AABB2 textBox = GetAABB2FromAABB2(Vector2(0.01f, .8f), Vector2(.4f, .95f), AABB2(-1.f, -1.f, 1.f, 1.f));
	r->DrawTextInBox("Loading...", textBox, .045f, t, DRAW_TEXT_MODE_OVERFLOW, Vector2(0.f, 0.0f));
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

