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
#include "Game/General/Map/Map.hpp"
#include "Game/General/Lua/GameLuaFunctionBindings.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"

//===============================================================================================
Game* g_theGame = nullptr;

//===============================================================================================
Game::Game()
{
	String path = "Projects/" + g_currentProjectName;
	Renderer* r = Renderer::GetInstance();
	
	std::string fullLuaPath = path + "/Scripts/Main.lua";
	m_mainLuaScript = new LuaScript(fullLuaPath, BindGameSideLuaFunctions);
	if (m_mainLuaScript == nullptr)
		PrintLog("Error creating main lua script");

	LoadOrReloadSpriteSheet();
	m_texturePath = path + "/Images/SpriteSheet.png";

	g_theGameCamera = new Camera();
	g_theGameCamera->SetColorTarget(r->m_defaultColorTarget);

	g_theGameClock = new Clock(g_theMasterClock);

	if (g_theGameBlackboard != nullptr)
		delete g_theGameBlackboard;
	new BlackBoard(path + "/Scripts/GameConfig.lua", GAME_BLACKBOARD);
	
	m_map = new Map();
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

	delete m_map;
	m_map = nullptr;

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
		DebugUpdateAndRender();
	}
}

//-----------------------------------------------------------------------------------------------
void Game::RenderGame() const
{
	Renderer* r = Renderer::GetInstance();
	
	float aspect = Window::GetInstance()->GetAspect();
	float size = 16 * 16;
	float x = size * aspect;
	float padding = (x - size) * .5f;

	g_theGameCamera->SetProjectionOrtho2D(
		Vector2(-padding + m_cameraPos.x, m_cameraPos.y), 
		Vector2(m_cameraPos.x + size + padding, m_cameraPos.y + size));

	r->SetCamera(g_theGameCamera);
	r->SetShader(r->m_defaultShader);

	r->SetCurrentTexture();
	LuaRender(*m_mainLuaScript);
	
	// side bars to be aspect ratio and only show one cell at a time
	r->DrawAABB2Filled(AABB2(Vector2(-padding + m_cameraPos.x, m_cameraPos.y), Vector2(m_cameraPos.x, m_cameraPos.y + size)), Rgba(0, 0, 0, 255));
	r->DrawAABB2Filled(AABB2(Vector2(m_cameraPos.x + size, m_cameraPos.y), Vector2(m_cameraPos.x + size + padding, m_cameraPos.y + size)), Rgba(0, 0, 0, 255));
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
void Game::LoadOrReloadSpriteSheet()
{
	String path = "Projects/" + g_currentProjectName;

	Renderer* r = Renderer::GetInstance();
	String fullPath = path + "/Images/SpriteSheet.png";

	if (g_theSpriteSheet != nullptr)
	{
		r->DeleteTexture(fullPath);
		delete g_theSpriteSheet;
	}
	g_theSpriteSheet = new SpriteSheet(r->CreateOrGetTexture(fullPath), 16, 16);
}

//-----------------------------------------------------------------------------------------------
Game* Game::GetInstance()
{
	return g_theGame;
}
