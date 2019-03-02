@echo off

REM ---------------------------------------------
REM Set Variables For ThirdParty Batch
REM ---------------------------------------------

REM LuaFiles
SET luaSources= Lua/lapi.c Lua/lauxlib.c Lua/lbaselib.c Lua/lbitlib.c Lua/lcode.c Lua/lcorolib.c Lua/lctype.c Lua/ldblib.c Lua/ldebug.c Lua/ldo.c Lua/ldump.c Lua/lfunc.c Lua/lgc.c Lua/linit.c Lua/liolib.c Lua/llex.c Lua/lmathlib.c Lua/lmem.c Lua/loadlib.c Lua/lobject.c Lua/lopcodes.c Lua/loslib.c Lua/lparser.c Lua/lstate.c Lua/lstring.c Lua/lstrlib.c Lua/ltable.c Lua/ltablib.c Lua/ltm.c Lua/lua.c Lua/luac.c Lua/lundump.c Lua/lutf8lib.c Lua/lvm.c Lua/lzio.c

REM StbiFiles
SET stbiFiles=stbi/stb_image.c stbi/stb_image_write.c

REM Fmod binaries
SET fmodSource=fmod/fmod.bc

REM sdl2 binaries
SET sdl2=SDL2/libSDL2.bc

REM ---------------------------------------------
REM Time to build
REM ---------------------------------------------

@echo on
REM ---------------------------------------------
REM | 	Make sure we are in the right directory
REM ---------------------------------------------
cd "D:\Github\Strawberry\Games\ProtoGame\Engine\ThirdParty"

REM ---------------------------------------------
REM |	The command to build the files to bc type which is like a static library
REM ---------------------------------------------
emcc -DFT2_BUILD_LIBRARY %luaSources% %stbiFiles% %fmodSource% %sdl2% --memory-init-file 0 -O3 -o thirdparty.bc 
REM ---------------------------------------------
REM | 		DONE with 3rd party stuff
REM ---------------------------------------------
