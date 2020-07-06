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
#include "TheApp.hpp"
#include "Engine/Core/Utils/StrawberryFileUtils.hpp"
#include "Engine/Core/Platform/File.hpp"
#include "Engine/Renderer/Systems/MeshBuilderStrawberry.hpp"
#include "Engine/Renderer/BuiltInShaders.hpp"
#include "Engine/Renderer/Components/Shader.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Renderer/Images/BitmapFont.hpp"

//===============================================================================================
Game* g_theGame = nullptr;

//===============================================================================================
Game::Game()
{
	String path = "Projects/" + g_currentProjectName;
	Renderer* r = Renderer::GetInstance();
	
	std::string fullLuaPath = path + "/Scripts/Main.lua";
	m_mainLuaScript = new LuaScript(fullLuaPath, path, BindGameSideLuaFunctions);
	if (m_mainLuaScript == nullptr)
		PrintLog("Error creating main lua script");

	LoadOrReloadSpriteSheet();
	m_texturePath = path + "/Images/SpriteSheet.png";

	m_gameShader = BuiltInShaders::CreateStrawberryShader();

	g_theGameCamera = new Camera();
	g_theGameCamera->SetColorTarget(r->m_defaultColorTarget);

	g_theGameClock = new Clock(g_theMasterClock);

	if (g_theGameBlackboard != nullptr)
		delete g_theGameBlackboard;
	new BlackBoard(path + "/Scripts/GameConfig.lua", GAME_BLACKBOARD);
	
	m_map = new Map();

	if(DoesDirectoryExist(GetCurveDataFilePath().c_str()))
		LoadCurveDataFromFile(m_curveData);

	m_takeDefaultCoverScreenshotTimer = StopWatch(g_theMasterClock);
	m_takeDefaultCoverScreenshotTimer.SetTimer(m_lengthBeforeTakingScreenshot);

	m_showBorder = g_theEngineBlackboard->GetValue("showShell", m_showBorder);

	g_theApp->m_states[APPSTATE_GAME] = this;

	g_theMeshBuilder.Clear();

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

	g_theApp->m_states[APPSTATE_GAME] = nullptr;

	g_theMeshBuilder.Clear();

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
	if (!m_mainLuaScript->HasError())
	{
		float ds = g_theGameClock->deltaTime * m_timeScale;

		if(m_sleepTimer > 0.f)
		{
			ds = 0;
			m_sleepTimer -= g_theGameClock->deltaTime;
		}
		
		LuaUpdateTimers(*m_mainLuaScript, ds);
		LuaUpdate(*m_mainLuaScript, ds);
		ShakeCamera();


		if (m_takeDefaultCoverScreenshotTimer.HasElapsed())
		{
			if (!DoesProjectHaveCoverImage(g_currentProjectName))
				g_theApp->m_takeGameCoverScreenshot = true;
		}
	}

	DebugRenderLog(std::to_string(60 / g_theGameClock->deltaTime), 0.f);
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
void Game::OnExit()
{
	CleanUp();
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
	g_theGameCamera->m_projectionMatrix.Append(Matrix44::MakeRotationDegrees2D(m_cameraAngle));

	r->SetShader(m_gameShader);


	r->SetSamplerUniform("gDefaultTexDiffuse", 0);
	r->SetSamplerUniform("gSpriteSheet1TexDiffuse", 1);

	r->SetCurrentTexture(0, r->m_defaultTexture);
	r->SetCurrentTexture(1, g_theSpriteSheet->m_texture);

	r->SetCamera(g_theGameCamera);

	//r->SetCurrentTexture();
	LuaRender(*m_mainLuaScript);

	r->SetCurrentTexture(0, r->m_defaultTexture);
	r->SetCurrentTexture(1, g_theSpriteSheet->m_texture);

	Mesh* theMesh = g_theMeshBuilder.CreateMesh();
	r->DrawMesh(theMesh, true);

	//RenderGameShell(padding, size);

	r->SetCurrentTexture();
	r->SetShader();
}

//-----------------------------------------------------------------------------------------------
void Game::RenderGameShell(float padding, float size) const
{
	Renderer* r = Renderer::GetInstance();
	r->SetShader(r->m_defaultShader);

	// side bars to be aspect ratio and only show one cell at a time
	AABB2 leftBounds = AABB2(Vector2(-padding + m_cameraPos.x, m_cameraPos.y), Vector2(m_cameraPos.x, m_cameraPos.y + size));
	AABB2 rightBounds = AABB2(Vector2(m_cameraPos.x + size, m_cameraPos.y), Vector2(m_cameraPos.x + size + padding, m_cameraPos.y + size));
	if (m_showBorder)
	{
		SpriteSheet caseSprites = SpriteSheet(r->CreateOrGetTexture("Data/StrawberryCase.png"), 2, 1);
	
		AABB2 spriteUVs = caseSprites.GetTexCoordsForSpriteIndex(0);
		r->DrawTexturedAABB2(leftBounds, *r->CreateOrGetTexture("Data/StrawberryCase.png"),
			spriteUVs.mins, spriteUVs.maxs, Rgba::WHITE);
	
		spriteUVs = caseSprites.GetTexCoordsForSpriteIndex(1);
		r->DrawTexturedAABB2(rightBounds, *r->CreateOrGetTexture("Data/StrawberryCase.png"),
			spriteUVs.mins, spriteUVs.maxs, Rgba::WHITE);
	}
	else
	{
		r->DrawAABB2Filled(leftBounds, Rgba(0, 0, 0, 255));
		r->DrawAABB2Filled(rightBounds, Rgba(0, 0, 0, 255));
	}
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

	r->DrawTextInBox(m_mainLuaScript->GetErrorMessage(), errorTextBounds, fontHeight, 1.f, DRAW_TEXT_MODE_WRAPPED, Vector2(0,0), 
		Rgba::WHITE, false, Rgba::BLACK, Window::GetInstance()->GetAspect());
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

//-----------------------------------------------------------------------------------------------
void Game::ShakeCamera()
{
	if (m_screenshakeAmount > 0.f)
	{
		float maxAmount = 4.f;
		float maxAngle = 2.f;

		// Todo make the GetRandomFloat into a noise function
		m_cameraAngle = maxAngle * m_screenshakeAmount * GetRandomFloat(-1.f, 1.f);

		m_cameraPos.x = maxAmount * m_screenshakeAmount * GetRandomFloat(-1.f, 1.f);
		m_cameraPos.y = maxAmount * m_screenshakeAmount * GetRandomFloat(-1.f, 1.f);

		m_screenshakeAmount -= g_theGameClock->deltaTime;
	}
	else
	{
		m_cameraAngle = 0.f;
	}
}
