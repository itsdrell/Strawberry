#include "BuiltInAssets.hpp"

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
