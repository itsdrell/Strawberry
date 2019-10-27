#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"

#ifdef EMSCRIPTEN_PORT
#else
#include "Engine/Core/Platform/Window.hpp"
#endif

//===============================================================================================
SpriteSheet*	g_theSpriteSheet = nullptr;
String			g_currentProjectName = "";

//===============================================================================================
void EngineStartUp()
{
	g_theMasterClock = new Clock();
	
	new AudioSystem();
	new InputSystem();
	new Renderer();
	new Console();

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

	delete Renderer::GetInstance();
	//g_theRenderer = nullptr;

	if (DebugRenderingSystem::IsInitialized())
	{
		delete DebugRenderingSystem::GetInstance();
	}
}
