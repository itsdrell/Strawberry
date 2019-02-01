#include "EngineCommon.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Renderer.hpp"

//===============================================================================================
void EngineStartUp()
{
	//AudioSystem* audio = new AudioSystem();
	//InputSystem* input = new InputSystem();
	Renderer* renderer = new Renderer();

	renderer->RenderStartup(Window::GetInstance()->GetHandle()); // call the static variable

	//audio = nullptr;
	//input = nullptr;
	renderer = nullptr;
}

//-----------------------------------------------------------------------------------------------
void EngineShutdown()
{
	//delete g_theAudioSystem;
	//g_theAudioSystem = nullptr;
	//
	//delete g_theInputSystem;
	//g_theInputSystem = nullptr;

	delete Renderer::GetInstance();
	//g_theRenderer = nullptr;
}
