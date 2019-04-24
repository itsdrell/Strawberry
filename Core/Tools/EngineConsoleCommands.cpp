#include "EngineConsoleCommands.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "Engine/Core/Platform/File.hpp"
#include "Engine/Core/General/BuiltInAssets.hpp"
#include "Engine/Renderer/Images/Image.hpp"

//===============================================================================================
void BindAllEngineCommands()
{
	CommandRegister("cls", "clear", "clears screen", ClearConsole, false);
	CommandRegister("help", "help", "shows all commands", PrintHelp, false);
	CommandRegister("quit", "quit", "quit app", QuitApp, false);
	CommandRegister("create", "create <projectName>", "create new project", CreateNewProject, false);
	CommandRegister("load", "load <projectName>", "loads project", LoadProject, false);
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

