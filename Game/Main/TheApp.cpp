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
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Playground.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Engine/Core/Platform/File.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"
#include "Engine/Core/General/ScreenCaptures.hpp"
#include "Game/States/Startup/StartupScreen.hpp"
#include "Game/States/Home/Home.hpp"
#include "Game/General/GameConsoleCommands.hpp"

#ifdef EMSCRIPTEN_PORT
	#include "Engine/Internal/EmscriptenCommon.hpp"
#endif

// it has to be this directory path or you will have a bad time
#include "Engine/ThirdParty/SDL2/SDL.h"
#include "Engine/ThirdParty/SDL2/SDL_opengl.h"



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
	new BlackBoard("Data/AppConfig.lua", ENGINE_BLACKBOARD);

	new Window("Strawberry Engine <3 ");
	
	EngineStartUp();
	BindAllGameSideCommands();

	InputSystem::GetInstance()->SetShutdownFunction((ShutdownFunction) Quit);

	m_recorder = new GifRecorder();

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

	delete g_theGameBlackboard;
	g_theGameBlackboard = nullptr;

	delete m_recorder;
	m_recorder = nullptr;

	for (uint i = 0; i < NUM_OF_APP_STATES; ++i)
		delete m_states[i];

	EngineShutdown();
}

//-----------------------------------------------------------------------------------------------
void App::StartUp()
{
	CommandRegister("run", "run <projectName>", "runs project", RunProject, false);

	Playground::RunTestOnce();

	CreateStates();
	
	printf("Done with startup \n");	
}

//-----------------------------------------------------------------------------------------------
void App::CreateStates()
{
	m_states[APPSTATE_STARTUP] = (IAppState*) new StartupScreen();
	m_states[APPSTATE_HOME] = (IAppState*) new Home();

	TransitionToState(APPSTATE_STARTUP);
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

	m_takeScreenshot = false;
	m_takeGameCoverScreenshot = false;

	// so the computer isn't using all the cores
	SDL_Delay(1);
}

//-----------------------------------------------------------------------------------------------
void App::Update()
{
	Console::GetInstance()->Update();

	m_states[m_currentState]->Update();
	
	Playground::RunTestOnUpdate();
	 
	HandleInput();
}

//-----------------------------------------------------------------------------------------------
void App::HandleInput()
{
	if (WasKeyJustPressed(KEYBOARD_TILDE) && m_isReleaseVersion == false)
	{		
		Console::GetInstance()->Toggle();
	}

#ifndef EMSCRIPTEN_PORT
	if (WasKeyJustPressed(KEYBOARD_F7))
	{
		m_takeScreenshot = true;
	}

	if (WasKeyJustPressed(KEYBOARD_F8))
	{
		if(m_recorder->m_isRecording == false)
		{
			m_recorder->Start();
		}
		else
		{
			m_recorder->StopAndSave();
		}
	}

	if(WasKeyJustPressed(KEYBOARD_F9))
	{
		m_takeGameCoverScreenshot = true;
	}

#endif

	if (WasKeyJustPressed(KEYBOARD_F5))
	{
		// only works if the game is already loaded! 
		// could also make this so we check if the project exists 
		if (g_theGame || g_theEditor)
		{
			ReloadAndRunGame();
			TransitionToState(APPSTATE_GAME);
		}
	}

	// You can only go to editor if you have a game loaded and are in that mode.
	if (WasKeyJustPressed(KEYBOARD_ESC) && m_isReleaseVersion == false && m_currentState == APPSTATE_GAME)
	{
		if (g_theEditor == nullptr)
		{
			g_theEditor = new Editor();
		}
		
		bool wasInGameState = m_currentState == APPSTATE_GAME;
		TransitionToState(APPSTATE_EDITOR);
		
		if (wasInGameState)
		{
			delete g_theGame;
			g_theGame = nullptr;
		}

		Console::GetInstance()->Close();
	}

	if (IsKeyPressed(KEYBOARD_ALT) && WasKeyJustPressed(KEYBOARD_UP_ARROW))
	{
		Window::GetInstance()->GoUpResolution();
	}

	if (IsKeyPressed(KEYBOARD_ALT) && WasKeyJustPressed(KEYBOARD_DOWN_ARROW))
	{
		Window::GetInstance()->GoDownResolution();
	}

	if (WasKeyJustReleased(KEYBOARD_F11))
	{
		Window::GetInstance()->ToggleFullscreenMode();
	}
}

//-----------------------------------------------------------------------------------------------
void App::Render() const
{
	m_states[m_currentState]->Render();

	// Doing it before debug and console render not sure if that help or hurts?
	HandleCaptures();
	
	Playground::RenderTest();
	DebugUpdateAndRender();

	Console::GetInstance()->Render();
}

//-----------------------------------------------------------------------------------------------
void App::LoadAndRunGame(const String& projectName)
{
	g_currentProjectName = projectName;
	ReloadAndRunGame();

	TransitionToState(APPSTATE_GAME);
}

//-----------------------------------------------------------------------------------------------
void App::ReloadAndRunGame()
{
	// Later it might save the editor and things like that :o 
	if (g_theGame != nullptr)
	{
		g_theGame->CleanUp();

		delete g_theGame;
		g_theGame = nullptr;
	}
	
	if (g_theGame == nullptr)
	{
		g_theGame = new Game();
	}
	
	g_theGame->StartUp();

	Console::GetInstance()->Close();

	PrintLog("New game created!");
}

//-----------------------------------------------------------------------------------------------
void App::HandleCaptures() const
{
	if (m_takeScreenshot)
		TakeScreenshot();

	if (m_takeGameCoverScreenshot)
		TakeGameCover();

	if (m_recorder->m_isRecording)
		m_recorder->Record();
}

//-----------------------------------------------------------------------------------------------
void App::TakeScreenshot() const
{
	// not sure if we want to allow screenshots if you don't have a project open
	// maybe cool once we have a start up / attract screen tho 
	String folder = g_currentProjectName != "" ? g_currentProjectName : "Editor";

	Screenshot* shot = new Screenshot(*Renderer::GetInstance()->m_defaultColorTarget);
	shot->SaveToScreenshotProjectFolder(folder); // deletes

	// could open the folder as well?? maybe an animation 
	DebugRenderLog("Screenshot taken" , 3, Rgba::WHITE);
}

//-----------------------------------------------------------------------------------------------
void App::TakeGameCover() const
{
	if (g_currentProjectName == "")
		return;

	String path = GetWorkingDirectoryPath() + "\\Projects\\" + g_currentProjectName + "\\Cover.png";

	Screenshot* shot = new Screenshot(*Renderer::GetInstance()->m_defaultColorTarget);
	shot->SaveToFullDirectoryPath(path); // deletes

	// could open the folder as well?? maybe an animation 
	DebugRenderLog("Game Cover Captured", 3, Rgba::WHITE);
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

//-----------------------------------------------------------------------------------------------
void App::TransitionToState(AppState stateToGoTo)
{
	if(m_currentState != NUM_OF_APP_STATES)
		m_states[m_currentState]->OnExit();
	
	if(stateToGoTo != NUM_OF_APP_STATES)
		m_states[stateToGoTo]->OnEnter();

	m_currentState = stateToGoTo;
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
