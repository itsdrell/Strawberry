#include "EngineConsoleCommands.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Engine/Core/Tools/Console.hpp"

//===============================================================================================
void BindAllEngineCommands()
{
	CommandRegister("clear", "clear", "clears screen", ClearConsole, false);
	CommandRegister("help", "help", "shows all commands", PrintHelp, false);
	CommandRegister("quit", "quit", "quit app", QuitApp, false);

}

//-----------------------------------------------------------------------------------------------
void ClearConsole(Command & command)
{
	UNUSED(command);
	Console::GetInstance()->ClearHistory();
}

//-----------------------------------------------------------------------------------------------
void PrintHelp(Command & command)
{
	UNUSED(command);
	Console::GetInstance()->AddConsoleDialogue("H E L P", Rgba::GetRandomColor());
}

//-----------------------------------------------------------------------------------------------
void QuitApp(Command & command)
{
	UNUSED(command);
	InputSystem::GetInstance()->CallQuitFunction();
}


