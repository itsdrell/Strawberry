#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"


#ifdef EMSCRIPTEN_PORT
#else
#include "Engine/Core/Platform/Window.hpp"
#endif

//===============================================================================================
void EngineStartUp()
{
	AudioSystem* audio = new AudioSystem();
	//InputSystem* input = new InputSystem();
	Renderer* renderer = new Renderer();

	audio = nullptr;
	//input = nullptr;
	renderer = nullptr;
}

//-----------------------------------------------------------------------------------------------
void EngineShutdown()
{
	AudioSystem::Shutdown();

	//delete g_theInputSystem;
	//g_theInputSystem = nullptr;

	delete Renderer::GetInstance();
	//g_theRenderer = nullptr;
}
