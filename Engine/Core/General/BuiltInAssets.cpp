#include "BuiltInAssets.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"

String GetMainLuaFileAsString()
{
	const char* mainLuaFile = 
R"(

-------------------------------------------------------------
-- Called once at the start
function StartUp()

end


------------------------------------------------------------
-- called every frame
function Update(ds)


end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
	Cls()

end


-- Good luck <3


)";
	
	return String(mainLuaFile);
}

//-----------------------------------------------------------------------------------------------
String GetLuaGameConfigString()
{
	const char* gameConfigLuaFile =
		R"(
------------------------------------------------------------------------
-- Game Config
------------------------------------------------------------------------
name = "strawberry engine";
version = 1.2;
web = true;

-- These are the physics channels in the collision editor
-- You can change what they equal -v to show up in the editor
channelOneName      = "Channel One";
channelTwoName      = "Channel Two";
channelThreeName    = "Channel Three";
channelFourName     = "Channel Four";
channelFiveName     = "Channel Five";

----------------------------------------------------------------------
-- default colors are from SuperFuture25 by @ENDESGA
-- https://twitter.com/ENDESGA
colors =
{
    black = { 16, 8, 32 },
    navy = { 24, 30, 51 },
    medBlue = { 96, 111, 171 },
    lightBlue = { 183, 217, 238 },
    white = { 255, 255, 255 },

    darkBrown = { 44, 25, 35 },
    brown = { 150, 80, 57 },
    lightBrown = { 219, 147, 87 },
    tan = { 248, 218, 172 },
    grey = { 192, 192, 129 },

    darkGreen = { 28, 51, 45 },
    green = { 54, 105, 67 },
    lightGreen = { 112, 195, 59 },
    yellow = { 255, 255, 0 },
    darkGrey = { 77, 77, 77 },

    darkRed = { 97, 27, 50 },
    orangeRed = { 210, 53, 47 },
    orange = { 255, 128, 0 },
    mustard = { 228, 187, 64 },
    red = { 255, 0, 64 },

    cyan = { 0, 255, 255 },
    blue = { 0, 128, 255 },
    indigo = { 69, 16, 126 },
    purple = { 156, 9, 204 },
    magenta = { 255, 0, 255 },
}
)";

	return String(gameConfigLuaFile);
}

//-----------------------------------------------------------------------------------------------
String GetMusicTextString()
{
	const char* musicText =
R"(
Music supports all audio types but .wav and .mp3 are ideal! 
This is for songs you want to have looped!

You can create subfolders in here but in lua you will have to 
add the folders to the path when using the lua functions

Ex. You made a folder call PlayerSounds
Music/PlayerSounds/theCoolSound.wav

In Lua you would call the sounds like:
PlayMusic("PlayerSounds/theCoolSound.wav")

instead of 
PlayMusic("theCoolSound.wav") -- DOES NOT WORK


)";

	return String(musicText);
}

//-----------------------------------------------------------------------------------------------
String GetSFXTextString()
{
	const char* sfxText =
R"(
SFX supports all audio types but .wav and .mp3 are ideal! 
This is for sfx that are just played once!

You can create subfolders in here but in lua you will have to 
add the folders to the path when using the lua functions

Ex. You made a folder call PlayerSounds
Music/PlayerSounds/theCoolSound.wav

In Lua you would call the sounds like:
PlayMusic("PlayerSounds/theCoolSound.wav")

instead of 
PlayMusic("theCoolSound.wav") -- DOES NOT WORK



)";

	return String(sfxText);
}

//-----------------------------------------------------------------------------------------------
String GetWebBatchFile()
{
	const char* webBatchFile =
		R"(

REM ---------------------------------------------
REM Set Variables 
REM ---------------------------------------------
SET myPath=%cd%

SET engineBinaries="Data\Web\Resources\Engine.bc"
SET whereToPutIt=WebBuilds\%nameOfGame%\
REM SET nameOfGameTextFile="%cd%/Data/Web/NameOfGame.txt"
SET nameOfGameFile="%cd%/Data/Web/NameOfGame.lua"

SET htmlFileLocation="%myPath%/Data/Web/Resources/WebPage/index.html"


REM ---------------------------------------------

echo webName="%nameOfGame%" > %nameOfGameFile%

REM ---------------------------------------------

REM Set the paths for all assets.
SET imageFiles= --preload-file Projects/%nameOfGame%/Images
SET scriptFiles= --preload-file Projects/%nameOfGame%/Scripts
SET audioFiles= --preload-file Projects/%nameOfGame%/Audio
SET engineResources= --preload-file Data
SET allAssetFiles= %imageFiles% %scriptFiles% %audioFiles% %engineResources%

REM ---------------------------------------------
REM |	Building time
REM ---------------------------------------------

REM ---------------------------------------------
REM |	Moving index.html over
REM ---------------------------------------------
COPY "Data/Web/Resources/WebPage/" "Builds\%nameOfGame%\"

@echo on
REM ---------------------------------------------
REM |	Delete old files to prevent garbage (trust nothing)
REM ---------------------------------------------
cd %myPath%
del %whereToPutIt%"%nameOfGame%.html"
del %whereToPutIt%"%nameOfGame%.js"
del %whereToPutIt%"%nameOfGame%.wasm"
del %whereToPutIt%"%nameOfGame%.wasm.map"
del %whereToPutIt%"%nameOfGame%.wast"
del %whereToPutIt%"%nameOfGame%.data"


REM ---------------------------------------------
REM |	Delete old files to prevent garbage (trust nothing)
REM ---------------------------------------------
if not exist %whereToPutIt% mkdir %whereToPutIt%

REM ---------------------------------------------
REM |	Compile the Game and the Engine
REM ---------------------------------------------
cmd.exe /c em++ -std=c++14 %engineBinaries% %allAssetFiles% -s FULL_ES2=1 --memory-init-file 0 -O0 -o %whereToPutIt%Game.js -v -g -I%myPath% -s ASSERTIONS=2 -s USE_SDL=2 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 -s USE_WEBGL2=1


REM ---------------------------------------------
REM | Make a server and open browser
REM ---------------------------------------------
REM cd "Web"
REM call "OpenInBrowser.bat"

REM ---------------------------------------------
REM |	Delete text lua and myself
REM ---------------------------------------------
del %nameOfGameFile%

REM ---------------------------------------------
REM Finished
REM ---------------------------------------------

DEL "%~f0"

pause 

@echo off
REM cmd.exe /c em++ -std=c++14 %allEngineAndGameCpps% %allAssetFiles% -s FULL_ES2=1 -DEMSCRIPTEN_PORT=1 --memory-init-file 0 -O0 -o Web/ProtoGame.js -v -g -I%myPath% -s ASSERTIONS=2 -s USE_SDL=2 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 -s USE_WEBGL2=1
REM if you are having weird linker errors (for your functions), make sure you didn't forget to include the cpp
REM using -s FULL_ES2=1 instead of -s FULL_ES3=1 because i am targetting 2
REM -s ERROR_ON_UNDEFINED_SYMBOLS=0 disables erros from linker erros so you can open it in the web and see whats causing them
REM -s DEMANGLE_SUPPORT=1 fixes a crash in chrome? :l 
REM -s ALLOW_MEMORY_GROWTH=1 may need to be used since audio? May not be efficient tho (see dev console in chrome)
REM this is for fmod -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']"
REM use USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 if using those libraries
REM cmd.exe /c before the em++ makes runs the last successful build if there is an error
REM GL_PREINITIALIZED_CONTEXT=1
REM -O3 is good for release, -O0 good for debug
REM type emsdk and enter and itll show you some helpers
REM -s WASM=0 is bad don't use it
REM didnt use GLEW -s USE_GLFW=3



)";

#ifndef EMSCRIPTEN_PORT
	return Stringf("SET nameOfGame=%s\n", g_currentProjectName.c_str()) + String(webBatchFile);
#else
	return "error";
#endif
}

//-----------------------------------------------------------------------------------------------
String GetWebPowershellBuildString()
{
	const char* webPowershellFile =
		R"(
#---------------------------------------------------------------
# Parameters
#---------------------------------------------------------------
param 
(
    $NameOfGame,
	$BuildName
)

#---------------------------------------------------------------
# Functions
#---------------------------------------------------------------
function Write-Banner()
{
    param
    (
        $Message
    )

    Write-Host "-------------------------------------"
    Write-Host $Message
    Write-Host "-------------------------------------"
}

#---------------------------------------------------------------
# Start of script
#---------------------------------------------------------------
Write-Banner "See if buildName is null and replace with timestamp if so"

Write-Host "BuildName is: " $BuildName
if($BuildName -eq $null)
{
	$BuildName = Get-Date -Format o | ForEach-Object { $_ -replace ":", "." }
}

#---------------------------------------------------------------
Write-Banner "Setting variables!"
$Env:SCRIPT_ROOT = $PSScriptRoot
$Env:ENGINE_BINARIES = $Env:SCRIPT_ROOT + "\Data\Web\Resources\Engine.bc"
$Env:WHERE_TO_PUT_IT = $Env:SCRIPT_ROOT + "\Builds\WebBuilds\$NameOfGame\$BuildName\" 
$NAME_OF_GAME_FILE = $Env:SCRIPT_ROOT + "\Data\Web\NameOfGame.lua"
$HTML_FILE_LOCATION = $Env:SCRIPT_ROOT + "\Data\Web\Resources\WebPage\index.html"

Write-Host "Engine Binary paths: " $Env:ENGINE_BINARIES
Write-Host "Where to put the build: " $Env:WHERE_TO_PUT_IT
Write-Host "Name of the game file: " $NAME_OF_GAME_FILE
Write-Host "HTML file location: " $HTML_FILE_LOCATION


#---------------------------------------------------------------
# emscripten commands
#---------------------------------------------------------------
$IMAGE_FILE_COMMAND = "--preload-file Projects/$NameOfGame/Images "
$SCRIPT_FILE_COMMAND = "--preload-file  Projects/$NameOfGame/Scripts "
$AUDIO_FILE_COMMAND = "--preload-file  Projects/$NameOfGame/Audio "
$ENGINE_RESOURCES_COMMAND = "--preload-file  Data "
$MAP_DATA_COMMAND = ("--preload-file  Projects/{0}/{0}.mapdata " -f $NameOfGame)
$Env:ALL_RESOURCE_COMMANDS = $IMAGE_FILE_COMMAND + $SCRIPT_FILE_COMMAND + $AUDIO_FILE_COMMAND + $ENGINE_RESOURCES_COMMAND + $MAP_DATA_COMMAND

Write-Host "Emscripten Resource command: " $Env:ALL_RESOURCE_COMMANDS

#---------------------------------------------------------------
Write-Banner 'Deleting old files if they exist to prevent garbage (trust nothing)'

if(Test-Path $Env:WHERE_TO_PUT_IT)
{
	Write-Host "Found files. Deleting them"
	Get-ChildItem -Path $Env:WHERE_TO_PUT_IT -Recurse | Remove-Item
}

#---------------------------------------------------------------
Write-Banner "Moving index.html over"
Write-Host "Index location: " $HTML_FILE_LOCATION
Write-Host "Destination: " $Env:WHERE_TO_PUT_IT

if(-not (Test-Path $Env:WHERE_TO_PUT_IT))
{
    New-Item -Path $Env:WHERE_TO_PUT_IT -ItemType Directory
}
Copy-Item -Force -Path $HTML_FILE_LOCATION -Destination $Env:WHERE_TO_PUT_IT

#---------------------------------------------------------------
# content of file is: webName="jump" 
Write-Banner "Create GameName.lua"

$content = ('gameName="{0}"' -f $NameOfGame)
Write-Host ("Creating a file: {0} with contents: {1}" -f $NAME_OF_GAME_FILE, $content)
New-Item -Path $NAME_OF_GAME_FILE -ItemType "file" -Value $content -Force

#---------------------------------------------------------------
Write-Banner "Compiling the Game, Engine, and resources"
Set-Location $PSScriptRoot

cmd.exe --% /c echo %cd% & em++ -std=c++14 %ENGINE_BINARIES% %ALL_RESOURCE_COMMANDS% -s FULL_ES2=1 --memory-init-file 0 -O0 -o %WHERE_TO_PUT_IT%\Game.js -v -g -I %SCRIPT_ROOT% -s ASSERTIONS=2 -s USE_SDL=2 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 -s USE_WEBGL2=1

#---------------------------------------------------------------
Write-Banner "Delete GameName.lua"

if(-not (Test-Path -Path $NAME_OF_GAME_FILE))
{
    Write-Error -Message "NameOfGame.lua did not exist so we couldn't delete :("
}
Remove-Item -Path $NAME_OF_GAME_FILE

#---------------------------------------------------------------
# Open in explorer
explorer.exe $Env:WHERE_TO_PUT_IT

#---------------------------------------------------------------
#Write-Banner "Open and Run in browser"
#Start-Process "http://localhost:8080/Index.html"
#python -m SimpleHTTPServer 8080
 
#---------------------------------------------------------------
Write-Banner "Delete self"
Remove-Item -LiteralPath $MyInvocation.MyCommand.Path -Force

#---------------------------------------------------------------
# Notes
#---------------------------------------------------------------
<#
@echo off
REM cmd.exe /c em++ -std=c++14 %allEngineAndGameCpps% %allAssetFiles% -s FULL_ES2=1 -DEMSCRIPTEN_PORT=1 --memory-init-file 0 -O0 -o Web/ProtoGame.js -v -g -I%myPath% -s ASSERTIONS=2 -s USE_SDL=2 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 -s USE_WEBGL2=1
REM if you are having weird linker errors (for your functions), make sure you didn't forget to include the cpp
REM using -s FULL_ES2=1 instead of -s FULL_ES3=1 because i am targetting 2
REM -s ERROR_ON_UNDEFINED_SYMBOLS=0 disables erros from linker erros so you can open it in the web and see whats causing them
REM -s DEMANGLE_SUPPORT=1 fixes a crash in chrome? :l 
REM -s ALLOW_MEMORY_GROWTH=1 may need to be used since audio? May not be efficient tho (see dev console in chrome)
REM this is for fmod -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']"
REM use USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 if using those libraries
REM cmd.exe /c before the em++ makes runs the last successful build if there is an error
REM GL_PREINITIALIZED_CONTEXT=1
REM -O3 is good for release, -O0 good for debug
REM type emsdk and enter and itll show you some helpers
REM -s WASM=0 is bad don't use it
REM didnt use GLEW -s USE_GLFW=3
#>
)";

	return String(webPowershellFile);
}

//-----------------------------------------------------------------------------------------------
String GetNewLuaFileString()
{
	const char* fileContent =
		R"(-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

)";

	return String(fileContent);
}

//-----------------------------------------------------------------------------------------------
String GetPCPowershellBuildString()
{
	const char* pcPowershellFile =
		R"(
param
(
    $GameName,
	$BuildName
)

#---------------------------------------------------------------
# Functions
#---------------------------------------------------------------
function Write-Banner()
{
    param
    (
        $Message
    )

    Write-Host "-------------------------------------"
    Write-Host $Message
    Write-Host "-------------------------------------"
}

#---------------------------------------------------------------
# Start of script
#---------------------------------------------------------------
Write-Banner "Setting variables!"
$Env:SCRIPT_ROOT = $PSScriptRoot

if($BuildName -eq $null)
{
	$BuildName = Get-Date -Format o | ForEach-Object { $_ -replace ":", "." }
}

$PCBuildDestinationPath = $Env:SCRIPT_ROOT + "\Builds\PCBuilds\" + $GameName + "\" + $BuildName

Write-Host "NameOfBuild: " $BuildName
Write-Host "PCBuildDestination: " $PCBuildDestinationPath

#---------------------------------------------------------------
# Need to move the Project/GameName folder
Write-Banner "Moving the resources"
$resourcesPath = $Env:SCRIPT_ROOT + "\Projects\" + $GameName
$resourcesDestination = $PCBuildDestinationPath + "\Projects\" + $GameName 

Write-Host "Resource Path: " $resourcesPath
Write-Host "Resource Destination: " $resourcesDestination

Copy-Item -Path $resourcesPath -Destination $resourcesDestination -Recurse

#---------------------------------------------------------------
# Need to move all of Data
Write-Banner "Moving all of data"
$dataLocation = $Env:SCRIPT_ROOT + "\Data"
$dataDestination = $PCBuildDestinationPath + "\Data\" 

Copy-Item -Path $dataLocation -Destination $dataDestination -Recurse

#---------------------------------------------------------------
# Need to create the gamename.lua
Write-Banner "Creating NameOfGame.lua"
$nameOfGamePath = $dataDestination + "\NameOfGame.lua"
$contentOfNameOfGame = ('gameName="{0}"' -f $GameName)

New-Item -Path $nameOfGamePath -Value $contentOfNameOfGame -Force

#---------------------------------------------------------------
# Need to add the release flag to the appconfig
Write-Banner "Adding release flag to app config"

Add-Content -Path ($dataDestination + "/AppConfig.lua") -Value "release=true"

#---------------------------------------------------------------
# Move all the dependencies (fmod.dll, SDL2.dll, Strawberry.exe)
# Rename Strawberry to be gamename.exe
Write-Banner "Moving c++ Dependencies"

Copy-Item -Path "$Env:SCRIPT_ROOT\fmod.dll" -Destination $PCBuildDestinationPath
Copy-Item -Path "$Env:SCRIPT_ROOT\fmod64.dll" -Destination $PCBuildDestinationPath
Copy-Item -Path "$Env:SCRIPT_ROOT\SDL2.dll" -Destination $PCBuildDestinationPath
Copy-Item -Path "$Env:SCRIPT_ROOT\Strawberry.exe" -Destination $PCBuildDestinationPath

Rename-Item -Path ($PCBuildDestinationPath + "\Strawberry.exe") -NewName ($GameName + ".exe")

#---------------------------------------------------------------
# Open in explorer
explorer.exe $PCBuildDestinationPath

#---------------------------------------------------------------
Write-Banner "Delete self"
Remove-Item -LiteralPath $MyInvocation.MyCommand.Path -Force

)";

	return String(pcPowershellFile);
}

