SET nameOfGame=Demo
REM ---------------------------------------------

REM ---------------------------------------------
REM Set Variables 
REM ---------------------------------------------
SET myPath=%cd%

SET engineBinaries="Data\Web\Resources\Engine.bc"
SET whereToPutIt=Data\Web\Builds\%nameOfGame%\
REM SET nameOfGameTextFile="%cd%/Data/Web/NameOfGame.txt"
SET nameOfGameFile="%cd%/Data/Web/NameOfGame.lua"


REM ---------------------------------------------

echo webName="%nameOfGame%" > %nameOfGameFile%

REM ---------------------------------------------

REM Set the paths for all assets. Engine programmer will need to include Run_Win32 in path when loading
SET imageFiles= --preload-file Projects/%nameOfGame%/Images
SET scriptFiles= --preload-file Projects/%nameOfGame%/Scripts
SET audioFiles= --preload-file Projects/%nameOfGame%/Audio
SET engineResources= --preload-file Data
SET allAssetFiles= %imageFiles% %scriptFiles% %audioFiles% %engineResources%

REM ---------------------------------------------
REM |	Building time
REM ---------------------------------------------


@echo on
REM ---------------------------------------------
REM |	Delete old files to prevent garbage (trust nothing)
REM ---------------------------------------------
cd %myPath%
del %whereToPutIt%%nameOfGame%.html 
del %whereToPutIt%%nameOfGame%.js 
del %whereToPutIt%%nameOfGame%.wasm 
del %whereToPutIt%%nameOfGame%.wasm.map
del %whereToPutIt%%nameOfGame%.wast
del %whereToPutIt%%nameOfGame%.data


REM ---------------------------------------------
REM |	Delete old files to prevent garbage (trust nothing)
REM ---------------------------------------------
if not exist %whereToPutIt% mkdir %whereToPutIt%

REM ---------------------------------------------
REM |	Compile the Game and the Engine
REM ---------------------------------------------
REM cmd.exe /c em++ -std=c++14 %thirdPartySource% %allEngineAndGameCpps% %allAssetFiles% -s FULL_ES2=1 -DEMSCRIPTEN_PORT=1 --memory-init-file 0 -O0 -o Web/ProtoGame.js -v -g -I%myPath% -s ASSERTIONS=2 -s USE_SDL=2 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 -s USE_WEBGL2=1

cmd.exe /c em++ -std=c++14 %engineBinaries% %allAssetFiles% -s FULL_ES2=1 --memory-init-file 0 -O0 -o %whereToPutIt%%nameOfGame%.js -v -g -I%myPath% -s ASSERTIONS=2 -s USE_SDL=2 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 -s USE_WEBGL2=1


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

pause 

@echo off
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
