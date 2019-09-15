#include "Playground.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Core/General/BlackBoard.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Renderer/Images/Sprite.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Core/General/Camera.hpp"

// turn off unused variable warnings
#pragma warning( disable : 4189 )

//===============================================================================================
void Playground::RunTestOnce()
{
	//Command newCommand = Command("help idk how to help you_duh");


}

//-----------------------------------------------------------------------------------------------
void Playground::RunTestOnUpdate()
{
	//PrintLog("MouseX: " + )
	Renderer* r = Renderer::GetInstance();
	AABB2 cameraBounds = r->m_defaultUICamera->GetOrthoBounds();
	Vector2 mousepos = GetMousePosition(cameraBounds);
	PrintLog("MouseX: " + std::to_string(mousepos.x) + " MouseY: " + std::to_string(mousepos.y) + "\n");
}

//-----------------------------------------------------------------------------------------------
void Playground::RenderTest()
{
	Renderer* r = Renderer::GetInstance();
}

