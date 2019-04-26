#include "EngineConsoleCommands.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "Engine/Core/Platform/File.hpp"
#include "Engine/Core/General/BuiltInAssets.hpp"
#include "Engine/Renderer/Images/Image.hpp"


#ifndef EMSCRIPTEN_PORT
	#include <stdlib.h>
#endif

//===============================================================================================
void BindAllEngineCommands()
{
	CommandRegister("cls", "clear", "clears screen", ClearConsole, false);
	CommandRegister("help", "help", "shows all commands", PrintHelp, false);
	CommandRegister("quit", "quit", "quit app", QuitApp, false);
	CommandRegister("create", "create <projectName>", "create new project", CreateNewProject, false);
	CommandRegister("load", "load <projectName>", "loads project", LoadProject, false);
	CommandRegister("projects", "projects", "shows all projects", ShowAllProjectNames, false);
	CommandRegister("folder", "folder", "opens folder to project in explorer", OpenFolder, false);
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
	
	//Strings textToShow = CommandRegistration::GetAllCommandsAndTheirHelp();
	Strings textToShow = CommandRegistration::GetAllTheCommandsNames();

	Console::GetInstance()->AddConsoleDialogue("", Rgba::WHITE);

	for (uint i = 0; i < textToShow.size(); i++)
	{
		String current = textToShow.at(i);

		if(current != "help")
			Console::GetInstance()->AddConsoleDialogue(current, Rgba::WHITE);
	}
}

//-----------------------------------------------------------------------------------------------
void QuitApp(Command & command)
{
	UNUSED(command);
	InputSystem::GetInstance()->CallQuitFunction();
}

//-----------------------------------------------------------------------------------------------
void CreateNewProject(Command& command)
{
	String name = command.GetNextString();
	g_currentProjectName = name;

	// lua
	String luaMainPath = "Projects/" + name + "/Scripts/Main.lua";
	String luaGameCommonPath = "Projects/" + name + "/Scripts/GameConfig.lua";
	
	String luaFile = GetMainLuaFileAsString();
	String gameConfigFile = GetLuaGameConfigString();

	CreateAndLogStringToFile(luaMainPath.c_str(), luaFile.c_str());
	CreateAndLogStringToFile(luaGameCommonPath.c_str(), gameConfigFile.c_str());

	// spritesheet
	String imagePath = "Projects/" + name + "/Images";
	CreateADirectory(imagePath.c_str());
	imagePath += "/SpriteSheet.png";
	Image theSpriteSheet = Image("spriteSheet", IntVector2(256, 256), Rgba(0, 0, 0, 0));
	SaveImageAsPng(theSpriteSheet, imagePath);

	// audio folders
	String musicPath = "Projects/" + name + "/Audio/Music/MusicHelperText.txt";
	String sfxPath = "Projects/" + name + "/Audio/SFX/SFXHelperText.txt";

	String musicText = GetMusicTextString();
	String sfxText = GetSFXTextString();

	CreateAndLogStringToFile(musicPath.c_str(), musicText.c_str());
	CreateAndLogStringToFile(sfxPath.c_str(), sfxText.c_str());

}

//-----------------------------------------------------------------------------------------------
void LoadProject(Command & command)
{
	String projectNameToLoad = command.GetNextString();
	String pathToCheck = "Projects/" + projectNameToLoad;

	// need to see if it exists
	if (DoesDirectoryExist(pathToCheck.c_str()))
	{
		g_currentProjectName = projectNameToLoad;
		AddConsoleDialogue("Loaded: " + projectNameToLoad, Rgba(0, 255, 0));
	}
	else
	{
		AddConsoleErrorMessage("Could not find: " + projectNameToLoad);
	}
}

//-----------------------------------------------------------------------------------------------
void ShowAllProjectNames(Command& command)
{
	UNUSED(command);
	Strings allFolders = GetAllFoldersInADirectory("Projects");

	for (uint i = 0; i < allFolders.size(); i++)
	{
		String current = allFolders.at(i);
		String theText = String(current, 9);
		
		AddConsoleDialogue(theText);
	}
}

//-----------------------------------------------------------------------------------------------
void OpenFolder(Command& command)
{
	UNUSED(command);
	String currentPath = GetWorkingDirectoryPath() + "\\Projects\\";

	// this is a check to see if a project is loaded. We would rather go there!
	if (g_currentProjectName != "")
		currentPath += (g_currentProjectName + "\\");

	String theTextCommand = "explorer /e," + currentPath;

#ifndef EMSCRIPTEN_PORT
	
	// this creates a command line which is shady... but it works!
	// squirrel says you can use a "." to get the current directory
	system(theTextCommand.c_str());

#endif
}

