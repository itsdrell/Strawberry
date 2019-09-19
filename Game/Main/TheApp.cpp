#include "TheApp.hpp"
#include "Game/Main/Game.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/ThirdParty/stbi/stb_image.h"
#include "Engine/ThirdParty/stbi/stb_image_write.h"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/General/BlackBoard.hpp"
#include "Game/Editor/Editor.hpp"

#ifdef EMSCRIPTEN_PORT
	#include "Engine/Internal/EmscriptenCommon.hpp"
#endif

// it has to be this directory path or you will have a bad time
#include "Engine/ThirdParty/SDL2/SDL.h"
#include "Engine/ThirdParty/SDL2/SDL_opengl.h"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Playground.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Engine/Core/Platform/File.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"


//===============================================================================================
App* g_theApp = nullptr;

//===============================================================================================
App::App()
{
	PrintLog("Initializing windows and SDL \n");

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER);

#ifndef EMSCRIPTEN_PORT
	// keep load library in app or move it and ^ to win 32 if want
	int result = SDL_GL_LoadLibrary("opengl32");
	if(result != 0)
	{
		String theError = SDL_GetError();
		PrintLog("Had error loading gl library. Error: %s" + theError);
	}
#endif

	SDL_StartTextInput();

	new Window("Strawberry Engine <3 ", 600, 400);
	
	EngineStartUp();
	InputSystem::GetInstance()->SetShutdownFunction((ShutdownFunction) Quit);

	new BlackBoard("Data/AppConfig.lua", ENGINE_BLACKBOARD);

	m_isQuitting = false;
	m_timeSinceStart = 0.f;
}

//-----------------------------------------------------------------------------------------------
App::~App()
{
	//SDL_GL_DeleteContext(maincontext);
	delete g_theGameBlackboard;
	g_theGameBlackboard = nullptr;

	if (g_theSpriteSheet != nullptr)
		delete g_theSpriteSheet;
	g_theSpriteSheet = nullptr;

	EngineShutdown();
}

//-----------------------------------------------------------------------------------------------
void App::StartUp()
{
	CommandRegister("run", "run <projectName>", "runs project", RunProject, false);

#ifndef EMSCRIPTEN_PORT
	Playground::RunTestOnce();
	Console::GetInstance()->Open();

	String startupGame = g_theEngineBlackboard->GetValue("startupGame", "idk");
	if (startupGame != "idk")
	{
		g_currentProjectName = startupGame;
		PrintLog("Project Name is: " + g_currentProjectName);
		ReloadAndRunGame();

		m_isReleaseVersion = g_theEngineBlackboard->GetValue("release", false);
	}

#else
	BlackBoard temp = BlackBoard("Data/Web/NameOfGame.lua", DATA_BLACKBOARD);
	g_currentProjectName = temp.GetValue("webName", "idk");
	PrintLog("Project Name is: " + g_currentProjectName);
	ReloadAndRunGame();
#endif
	
	printf("Done with startup \n");	
}

//-----------------------------------------------------------------------------------------------
void App::RunFrame()
{
	AudioSystem* theAudio = AudioSystem::GetInstance();
	
	ClockSystemBeginFrame();
	g_theInputSystem->BeginFrame();
	Renderer::GetInstance()->BeginFrame();
	//g_theRenderer->UpdateTime(g_theGameClock->deltaTime, g_theMasterClock->deltaTime);
	theAudio->BeginFrame();
	g_theInputSystem->Update();
	
	Update();
	Render();

	g_theInputSystem->EndFrame();
	Renderer::GetInstance()->EndFrame();
	theAudio->EndFrame();

	// so the computer isn't using all the cores
	SDL_Delay(1);
}

//-----------------------------------------------------------------------------------------------
void App::Update()
{
	Console::GetInstance()->Update();
	
	if(m_currentState == APPSTATE_GAME)
		g_theGame->Update();
	if (m_currentState == APPSTATE_EDITOR)
		g_theEditor->Update();
	
	Playground::RunTestOnUpdate();
	 
	if (WasKeyJustPressed(KEYBOARD_TILDE) && m_isReleaseVersion == false)
	{
		// can only toggle if there is a game!
		if (g_theGame || g_theEditor)
			Console::GetInstance()->Toggle();
	}

	if (WasKeyJustPressed(KEYBOARD_F5))
	{
		// only works if the game is already loaded! 
		// could also make this so we check if the project exists 
		if (g_theGame || g_theEditor)
			ReloadAndRunGame();
	}

	if (WasKeyJustPressed(KEYBOARD_ESC) && m_isReleaseVersion == false)
	{
		if (m_currentState == APPSTATE_GAME)
		{
			// leave the game / cleanup
			g_theGame->CleanUp();

			delete g_theGame;
			g_theGame = nullptr;
		}
		
		m_currentState = APPSTATE_EDITOR;

		if (g_theEditor == nullptr)
			g_theEditor = new Editor();

		Console::GetInstance()->Close();
	}
}

//-----------------------------------------------------------------------------------------------
void App::Render() const
{
	if(m_currentState == APPSTATE_GAME)
		g_theGame->Render();
	if (m_currentState == APPSTATE_EDITOR)
		g_theEditor->Render();
	
	Playground::RenderTest();

	Console::GetInstance()->Render();
}

//-----------------------------------------------------------------------------------------------
void App::ReloadAndRunGame()
{
	m_currentState = APPSTATE_GAME;
	
	// Later it might save the editor and things like that :o 
	if (g_theGame != nullptr)
	{
		g_theGame->CleanUp();

		delete g_theGame;
	}
	
	if(g_theGame == nullptr)
		g_theGame = new Game(); 
	
	g_theGame->StartUp();

	Console::GetInstance()->Close();

	PrintLog("New game created!");
}

void App::TestTexture()
{
	// the color format you request stb_image to output,
	// use STBI_rgb if you don't want/need the alpha channel
	int req_format = STBI_rgb_alpha;
	int width, height, orig_format;

#ifdef EMSCRIPTEN_PORT
	unsigned char* data = stbi_load("Run_Win32/Images/testSprite.png", &width, &height, &orig_format, req_format);
#else
	unsigned char* data = stbi_load("Images/testSprite.png", &width, &height, &orig_format, req_format);
#endif
	if (data == NULL) {
		SDL_Log("Loading image failed: %s", stbi_failure_reason());
		//exit(1);
	}

	int depth, pitch;
	Uint32 pixel_format;
	if (req_format == STBI_rgb) {
		depth = 24;
		pitch = 3*width; // 3 bytes per pixel * pixels per row
		pixel_format = SDL_PIXELFORMAT_RGB24;
	} else { // STBI_rgb_alpha (RGBA)
		depth = 32;
		pitch = 4*width;
		pixel_format = SDL_PIXELFORMAT_RGBA32;
	}

	SDL_Surface* TempSurface = SDL_CreateRGBSurfaceWithFormatFrom((void*)data, width, height,
		depth, pitch, pixel_format);

	if (TempSurface == NULL) {
		SDL_Log("Creating surface failed: %s", SDL_GetError());
		stbi_image_free(data);
		//exit(1);
	}

	//-----------------------------------------------------------------------------------------------
	// ... do something useful with the surface ...
	// ...
	if(TempSurface == NULL) {
		//Log("Unable to load image : %s : %s", Filename.c_str(), IMG_GetError());
		//return false;
	}

	// Convert SDL surface to a texture
	//if((m_testTexture = SDL_CreateTextureFromSurface(m_renderer, TempSurface)) == NULL) {
		//Log("Unable to create SDL Texture : %s : %s", Filename.c_str(), IMG_GetError());
		//return false;
	//}

	// Grab dimensions
	//int theWidth = 0;
	//int theHE = 0;
	//SDL_QueryTexture(m_testTexture, NULL, NULL, &width, &height);

	//Log("Texture Dimensions: %s : %d %d", Filename.c_str(), Width, Height);

	//SDL_FreeSurface(TempSurface);

	//-----------------------------------------------------------------------------------------------
	// .. *and* the data used by the surface!

	stbi_image_free(data);

}

//===============================================================================================
// This is a function we will use as a callback in the input system for quitting
void Quit()
{
	g_theApp->m_isQuitting = true;
}

//-----------------------------------------------------------------------------------------------
void RunProject(Command& command)
{
	UNUSED(command);
	g_theApp->ReloadAndRunGame();
	Console::GetInstance()->ClearHistory();
	//Renderer::GetInstance()->
}
