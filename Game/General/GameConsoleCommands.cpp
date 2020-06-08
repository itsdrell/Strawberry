#include "GameConsoleCommands.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Game/Main/TheApp.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Core/Tools/Console.hpp"


//===============================================================================================
void BindAllGameSideCommands()
{
	CommandRegister("home", "home", "go back to home", GoBackToHome);
}

//-----------------------------------------------------------------------------------------------
void GoBackToHome(Command & command)
{	
	UNUSED(command);
	
	g_theApp->TransitionToState(APPSTATE_HOME);

	Console::GetInstance()->Toggle();
}
