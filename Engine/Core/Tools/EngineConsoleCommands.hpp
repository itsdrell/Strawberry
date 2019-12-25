#pragma once

//====================================================================================
// Forward Declare
//====================================================================================
class Command;

//====================================================================================
// Standalone C Functions
//====================================================================================
void BindAllEngineCommands();

//-----------------------------------------------------------------------------------------------
void ClearConsole(Command& command);
void PrintHelp(Command& command);
void QuitApp(Command& command);
void CreateNewProject(Command& command);
void LoadProject(Command& command);
void ShowAllProjectNames(Command& command);
void OpenFolder(Command& command);
void BuildForWeb(Command& command);
void ExecuteLuaCommand(Command& command);

void CreateLuaFile(Command& command);
void OpenVisualStudioCode(Command& command);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [4/3/2019]
//====================================================================================