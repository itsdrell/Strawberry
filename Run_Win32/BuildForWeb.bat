@echo off

REM ---------------------------------------------
REM Set Variables 
REM ---------------------------------------------
SET myPath=%cd%

REM Location of third party stuff
SET thirdPartySource="%myPath%\Engine\ThirdParty\thirdparty.bc"

REM List of all of the game files
REM SET gameFiles=Game/Main/Game.cpp Game/Main/TheApp.cpp Game/Main/Main_Win32.cpp

@echo on
setlocal ENABLEDELAYEDEXPANSION
SET allEngineAndGameCpps=
for /f "tokens=1* delims=\" %%A in (
  'forfiles /s /m *.cpp /c "cmd /c echo @relpath"'
) do for %%F in (^"%%B) do (set allEngineAndGameCpps=!allEngineAndGameCpps! %%~F)

REM ---------------------------------------------
REM echo List of all Files: %allEngineAndGameCpps%
REM ---------------------------------------------

REM List of all of the engine files
REM Set engineFiles=Engine/Core/General/Camera.cpp Engine/Core/General/EngineCommon.cpp Engine/Core/General/Rgba.cpp Engine/Core/Tools/ErrorWarningAssert.cpp Engine/Core/Utils/StringUtils.cpp Engine/Lua/EngineLuaFunctionBindings.cpp Engine/Lua/LuaScript.cpp Engine/Math/Geometry/AABB2.cpp Engine/Math/Vectors/IntVector2.cpp Engine/Math/Vectors/Vector2.cpp Engine/Math/Vectors/Vector3.cpp Engine/Math/Matrices/Matrix44.cpp Engine/Renderer/Components/Shader.cpp Engine/Renderer/Components/ShaderProgram.cpp Engine/Renderer/Images/Image.cpp Engine/Renderer/Images/Texture.cpp Engine/Renderer/Pipeline/FrameBuffer.cpp Engine/Renderer/Pipeline/RenderBuffer.cpp Engine/Renderer/Pipeline/Sampler.cpp Engine/Renderer/Pipeline/VertexLayout.cpp Engine/Renderer/BuiltInShaders.cpp Engine/Renderer/GLFunctions.cpp Engine/Renderer/Renderer.cpp Engine/Renderer/RenderTypes.cpp Engine/Core/Platform/Window.cpp Engine/Audio/AudioSystem.cpp

REM Set the paths for all assets. Engine programmer will need to include Run_Win32 in path when loading
SET imageFiles= --preload-file Run_Win32/Images
SET scriptFiles= --preload-file Run_Win32/Scripts
SET audioFiles= --preload-file Run_Win32/Audio
SET allAssetFiles= %imageFiles% %scriptFiles% %audioFiles%
REM ---------------------------------------------
REM |	Building time
REM ---------------------------------------------

@echo on
REM ---------------------------------------------
REM | 	Running the thirdParty batch files
REM ---------------------------------------------
call "%myPath%\Engine\ThirdParty\ThirdPartyCBatch.bat"

@echo on
REM ---------------------------------------------
REM |	Delete old files to prevent garbage (trust nothing)
REM ---------------------------------------------
cd %myPath%
REM del Web\ProtoGame.html 
del Web\ProtoGame.js 
del Web\ProtoGame.wasm 
del Web\ProtoGame.wasm.map
del Web\ProtoGame.wast
del Web\ProtoGame.data

REM ---------------------------------------------
REM |	Compile the Game and the Engine
REM ---------------------------------------------
cmd.exe /c em++ -std=c++14 %thirdPartySource% %allEngineAndGameCpps% %allAssetFiles% -s FULL_ES2=1 -DEMSCRIPTEN_PORT=1 --memory-init-file 0 -O0 -o Web/ProtoGame.js -v -g -I%myPath% -s ASSERTIONS=2 -s USE_SDL=2 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 -s USE_WEBGL2=1

REM ---------------------------------------------
REM | Make a server and open browser
REM ---------------------------------------------
cd "Web"
call "OpenInBrowser.bat"

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
