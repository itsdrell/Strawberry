#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"


#ifdef EMSCRIPTEN_PORT
#else
#include "Engine/Core/Platform/Window.hpp"
#endif

//===============================================================================================
void EngineStartUp()
{
	//AudioSystem* audio = new AudioSystem();
	//InputSystem* input = new InputSystem();
	Renderer* renderer = new Renderer();
#ifdef EMSCRIPTEN_PORT
	renderer->RenderStartupForWeb(Vector2(320.f, 220.f));
#else
	renderer->RenderStartupForWindows(Window::GetInstance()->GetHandle()); // call the static variable

#endif
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
