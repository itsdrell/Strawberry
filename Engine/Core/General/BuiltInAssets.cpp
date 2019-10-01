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

