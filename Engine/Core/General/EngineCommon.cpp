#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"

#ifdef EMSCRIPTEN_PORT
#else
#include "Engine/Core/Platform/Window.hpp"
#endif

//===============================================================================================
SpriteSheet*	g_theSpriteSheet = nullptr;
SpriteSheet*	g_allSpriteSheets[MAX_AMOUNT_OF_SPRITE_SHEETS];
SpriteSheet*	g_editorIconSpriteSheet = nullptr;

String			g_currentProjectName = "";

//===============================================================================================
void EngineStartUp()
{
	g_theMasterClock = new Clock();
	
	new AudioSystem();
	new InputSystem();
	Renderer* temp = new Renderer();
	new Console();


	g_editorIconSpriteSheet = new SpriteSheet(temp->CreateOrGetTexture("Data/StrawberryIconSpritesheet.png"), 4,4);
}

//-----------------------------------------------------------------------------------------------
void EngineShutdown()
{
	AudioSystem::GetInstance()->Shutdown();
	CommandRegistration::DeleteAllRegisteredCommands();

	delete Console::GetInstance();

	delete g_theMasterClock;
	g_theMasterClock = nullptr;

	delete g_theInputSystem;
	g_theInputSystem = nullptr;

	delete g_editorIconSpriteSheet;
	g_editorIconSpriteSheet = nullptr;

	delete Renderer::GetInstance();
	//g_theRenderer = nullptr;

	if (DebugRenderingSystem::IsInitialized())
	{
		delete DebugRenderingSystem::GetInstance();
	}
}
