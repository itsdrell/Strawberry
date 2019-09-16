#include "Game.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Renderer/RenderTypes.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Math/Vectors/Vector3.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Lua/LuaScript.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/General/BlackBoard.hpp"
#include "Engine/Audio/AudioSystem.hpp"

//===============================================================================================
Game* g_theGame = nullptr;

//===============================================================================================
Game::Game()
{
	String path = "Projects/" + g_currentProjectName;
	
	m_mainLuaScript = new LuaScript(path + "/Scripts/Main.lua");
	if (m_mainLuaScript == nullptr)
		PrintLog("Error creating main lua script");

	Renderer* r = Renderer::GetInstance();
	m_texturePath = path + "/Images/SpriteSheet.png";
	
	if(g_theSpriteSheet != nullptr)
	{
		r->DeleteTexture(m_texturePath);
		delete g_theSpriteSheet;
	}
	g_theSpriteSheet = new SpriteSheet(r->CreateOrGetTexture(m_texturePath), 16, 16);

	g_theGameCamera = new Camera();
	g_theGameCamera->SetColorTarget(r->m_defaultColorTarget);

	g_theGameClock = new Clock(g_theMasterClock);

	if (g_theGameBlackboard != nullptr)
		delete g_theGameBlackboard;
	new BlackBoard(path + "/Scripts/GameConfig.lua", GAME_BLACKBOARD);
	
}

//-----------------------------------------------------------------------------------------------
Game::~Game()
{
	delete m_mainLuaScript;
	m_mainLuaScript = nullptr;

	delete g_theGameCamera;
	g_theGameCamera = nullptr;

	delete g_theGameClock;
	g_theGameClock = nullptr;

	delete g_theGameBlackboard;
	g_theGameBlackboard = nullptr;

	// don't delete the spritesheet, the game will delete it on startup and the app on shutdown
}

//-----------------------------------------------------------------------------------------------
void Game::StartUp()
{
	if(!LuaStartUp(*m_mainLuaScript))
		PrintLog("Error calling Lua start up script! \n");
}

//-----------------------------------------------------------------------------------------------
void Game::CleanUp()
{
	String path = "Projects/" + g_currentProjectName;

	// delete sounds
	AudioSystem::GetInstance()->StopAllSounds();
	AudioSystem::GetInstance()->FlushRegisteredSounds();
}

//-----------------------------------------------------------------------------------------------
void Game::Update()
{
	if(!m_mainLuaScript->HasError())
		LuaUpdate(*m_mainLuaScript, g_theGameClock->deltaTime);
}

//-----------------------------------------------------------------------------------------------
void Game::Render() const
{
	if (m_mainLuaScript->HasError())
	{
		RenderError();
	}
	else
	{
		RenderGame();
	}
}

//-----------------------------------------------------------------------------------------------
void Game::RenderGame() const
{
	Renderer* r = Renderer::GetInstance();

	// The code we want
	g_theGameCamera->SetProjectionOrthoByAspect(128.f);

	r->SetCamera(g_theGameCamera);
	r->SetShader(r->m_defaultShader);
	r->SetCurrentTexture();

	LuaRender(*m_mainLuaScript);
}

//-----------------------------------------------------------------------------------------------
void Game::RenderError() const
{
	Renderer* r = Renderer::GetInstance();

	r->m_clearScreen = true;
	r->SetCamera(r->m_defaultUICamera);
	r->SetShader(r->m_defaultShader);
	r->SetCurrentTexture();

	AABB2 cameraBounds = r->m_defaultUICamera->GetOrthoBounds();
	AABB2 errorTextBounds = GetAABB2FromAABB2(Vector2(.05f, .25f), Vector2(.95f, .95f), cameraBounds);
	AABB2 inspirationTextBounds = GetAABB2FromAABB2(Vector2(.05f, .05f), Vector2(.95f, .2f), cameraBounds);
	float fontHeight = RangeMapFloat(.03f, 0.f, 1.f, 0.f, cameraBounds.maxs.y);

	r->DrawAABB2Filled(cameraBounds, Rgba(210, 74, 97, 255));
	r->DrawAABB2Outline(errorTextBounds, Rgba::WHITE);
	r->DrawAABB2Outline(inspirationTextBounds, Rgba::WHITE);

	r->DrawWrappedTextInBox2D(m_mainLuaScript->GetErrorMessage(), errorTextBounds, fontHeight, Window::GetInstance()->GetAspect());
}

//-----------------------------------------------------------------------------------------------
Game* Game::GetInstance()
{
	return g_theGame;
}
