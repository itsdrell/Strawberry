#include "EngineConsoleCommands.hpp"
#include "Engine/Core/Tools/Command.hpp"
#include "Engine/Core/Tools/Console.hpp"
#include "Engine/Core/Platform/File.hpp"
#include "Engine/Core/General/BuiltInAssets.hpp"
#include "Engine/Renderer/Images/Image.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"
#include "Engine/Core/General/ScreenCaptures.hpp"
#include "Engine/Lua/LuaUtils.hpp"


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
	CommandRegister("lua", "lua", "execute lua code", ExecuteLuaCommand, false);
	CommandRegister("luac", "luac  <filename>", "", CreateLuaFile, false);
	CommandRegister("vsc", "vsc <filename>", "", OpenVisualStudioCode, false);
	CommandRegister("export", "export", "export either <web or PC>", ExportGame, false);
	CommandRegister("startup", "startup", "set the default project to load", StartupProject);
	CommandRegister("gifLength", "gifLength", "overload the gif record legnth", SetGifLength);
	//CommandRegister("web", "", "", BuildForWeb);
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
	imagePath += "/SpriteSheet_0.png";
	Image theSpriteSheet = Image("spriteSheet", IntVector2(256, 256), Rgba(0, 0, 0, 0));
	SaveImageAsPng(theSpriteSheet, imagePath);

	// audio folders
	String musicPath = "Projects/" + name + "/Audio/Music/MusicHelperText.txt";
	String sfxPath = "Projects/" + name + "/Audio/SFX/SFXHelperText.txt";

	String musicText = GetMusicTextString();
	String sfxText = GetSFXTextString();

	CreateAndLogStringToFile(musicPath.c_str(), musicText.c_str());
	CreateAndLogStringToFile(sfxPath.c_str(), sfxText.c_str());

	SetStartupProject(name);
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

//-----------------------------------------------------------------------------------------------
void ExecuteLuaCommand(Command& command)
{
	// todo : this doesn't work but it seems to be the right step???
	
	int result = LuaExecute(command.GetNextString());

	if (result != 0)
	{
		AddConsoleErrorMessage("Error running lua script");
	}
}

//-----------------------------------------------------------------------------------------------
void CreateLuaFile(Command & command)
{
	if (g_currentProjectName == "")
		return;

	String currentPath = GetWorkingDirectoryPath() + "\\Projects\\";
	String scriptPath = currentPath + (g_currentProjectName + "\\Scripts\\");
	String luaFilePath = scriptPath + command.GetNextString() + ".lua";

	String theTextCommand = "code -r " + luaFilePath + " -a " + scriptPath;

#ifndef EMSCRIPTEN_PORT
	CreateAndLogStringToFile(luaFilePath.c_str(), GetNewLuaFileString().c_str());
	system(theTextCommand.c_str());
#endif
}

//-----------------------------------------------------------------------------------------------
void OpenVisualStudioCode(Command & command)
{
	UNUSED(command);

	if (g_currentProjectName == "")
		return;

	String currentPath = GetWorkingDirectoryPath() + "\\Projects\\";
	String scriptPath = currentPath + (g_currentProjectName + "\\Scripts\\");
	String luaFilePath = scriptPath + "Main.lua";

	String theTextCommand = "code -r " + luaFilePath + " -a " + scriptPath;

#ifndef EMSCRIPTEN_PORT
	system(theTextCommand.c_str());
#endif
}

//-----------------------------------------------------------------------------------------------
void StartupProject(Command& command)
{
	String requestedProjectName = command.GetNextString();
	Strings allFolders = GetAllFoldersInADirectory("Projects");

	bool found = false;
	for (String project : allFolders)
	{
		String currentProjectName = String(project, 9); // it included the path Project in name
		if (currentProjectName == requestedProjectName)
		{
			found = true;
			break;
		}
	}

	if (found == false)
	{
		// Can't set it to a project that doesn't exist
		AddConsoleErrorMessage("Unable to find project: " + requestedProjectName);
		return;
	}

	SetStartupProject(requestedProjectName);
}

//-----------------------------------------------------------------------------------------------
void SetGifLength(Command& command)
{
	String input = command.GetNextString();
	float length = ParseString(input, GIF_DEFAULT_LENGTH_IN_SECONDS);

	GifRecorder* recorder = GifRecorder::Instance();
	recorder->SetLength(length);

	AddConsoleDialogue("Set gif length to:" + input + " seconds!");
}

//-----------------------------------------------------------------------------------------------
// Export <Web or PC>
void ExportGame(Command& command)
{
	String platformType = command.GetNextString();

	// you need a name!
	if (g_currentProjectName == "")
		return;

	if(platformType == "web")
	{
		BuildForWeb(command);
	}
	else
	{
		BuildForWin32(command);
	}
}

//===============================================================================================
// Standalone functions
//===============================================================================================
void BuildForWeb(Command& command)
{
#ifndef EMSCRIPTEN_PORT 
	//if (command.m_commandArguements.size() != 0)
	//	g_currentProjectName = command.m_commandArguements.at(0);

	// you need a name!
	if (g_currentProjectName == "")
		return;

	String workingDirectory = GetWorkingDirectoryPath();
	String fullPath = workingDirectory + "/" + "Projects/" + g_currentProjectName;

	if (!DoesDirectoryExist(fullPath.c_str()))
		return;

	// Do the batch file
	LogStringToFile("BuildWeb.ps1", GetWebPowershellBuildString().c_str(), true);

	// make sure you can run scripts this may be needed?
	//system("start powershell.exe Set-ExecutionPolicy RemoteSigned \n");
	// You can add -NoExit after powershell.exe if you need it to not close
	String cmd = Stringf("start /WAIT powershell.exe %s\\BuildWeb.ps1 -NameOfGame %s",
		workingDirectory.c_str(), g_currentProjectName.c_str());

	String buildName = command.GetNextString();
	if (buildName != "")
	{
		cmd += (" -BuildName " + buildName);
	}

	LogStringToFile("BuildForWeb.bat", cmd.c_str(), true);

	int exitCode = system("BuildForWeb.bat");
	if (exitCode == 0)
	{
		Console::GetInstance()->AddConsoleDialogue("Web Build Completed");
	}
	else
	{
		Console::GetInstance()->AddErrorMessage("Web Build Failed");
	}
	system("\n DEL BuildForWeb.bat");
#endif
}

//-----------------------------------------------------------------------------------------------
void SetStartupProject(const String& projectName)
{
	String workingDirectory = GetWorkingDirectoryPath();
	String configPath = workingDirectory + "\\Data\\AppConfig.lua";

	Strings content = GetAllLinesFromFile(configPath.c_str());

	for (uint i = 0; i < content.size(); i++)
	{
		std::string line = content.at(i);
		if(line.find("startupGame") != std::string::npos)
		{
			line = "startupGame='" + projectName + "'";
			content[i] = line;
			break;
		}
	}

	LogStringsToFile(configPath.c_str(), content, true);
}

//-----------------------------------------------------------------------------------------------
void BuildForWin32(Command& command)
{
#ifndef EMSCRIPTEN_PORT 
	// you need a name!
	if (g_currentProjectName == "")
		return;

	String workingDirectory = GetWorkingDirectoryPath();
	String fullPath = workingDirectory + "/" + "Projects/" + g_currentProjectName;

	if (!DoesDirectoryExist(fullPath.c_str()))
		return;

	// Do the batch file
	LogStringToFile("BuildPC.ps1", GetPCPowershellBuildString().c_str(), true);

	// make sure you can run scripts this may be needed?
	//system("start powershell.exe Set-ExecutionPolicy RemoteSigned \n");
	// You can add -NoExit after powershell.exe if you need it to not close
	String cmd = Stringf("start /WAIT powershell.exe %s\\BuildPC.ps1 -GameName %s",
		workingDirectory.c_str(), g_currentProjectName.c_str());

	String buildName = command.GetNextString();
	if(buildName != "")
	{
		cmd += (" -BuildName " + buildName);
	}

	LogStringToFile("BuildForPC.bat", cmd.c_str(), true);

	int exitCode = system("BuildForPC.bat");
	if (exitCode == 0)
	{
		Console::GetInstance()->AddConsoleDialogue("PC Build Completed");
	}
	else
	{
		Console::GetInstance()->AddErrorMessage("PC Build Failed");
	}
	system("\n DEL BuildForPC.bat");
#endif
}
