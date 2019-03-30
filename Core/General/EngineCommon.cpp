#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"


#ifdef EMSCRIPTEN_PORT
#else
#include "Engine/Core/Platform/Window.hpp"
#endif
#include "../Tools/Clock.hpp"

//===============================================================================================
void EngineStartUp()
{
	g_theMasterClock = new Clock();
	
	AudioSystem* audio = new AudioSystem();
	InputSystem* input = new InputSystem();
	Renderer* renderer = new Renderer();

	audio = nullptr;
	input = nullptr;
	renderer = nullptr;
}

//-----------------------------------------------------------------------------------------------
void EngineShutdown()
{
	AudioSystem::GetInstance()->Shutdown();

	delete g_theMasterClock;
	g_theMasterClock = nullptr;

	delete g_theInputSystem;
	g_theInputSystem = nullptr;

	delete Renderer::GetInstance();
	//g_theRenderer = nullptr;
}
