@echo off

REM ---------------------------------------------
REM Set Variables 
REM ---------------------------------------------
SET myPath=%cd%

REM Location of third party stuff
SET thirdPartySource="%myPath%\Engine\ThirdParty\thirdparty.bc"

@echo on
setlocal ENABLEDELAYEDEXPANSION
SET allEngineAndGameCpps=
for /f "tokens=1* delims=\" %%A in (
  'forfiles /s /m *.cpp /c "cmd /c echo @relpath"'
) do for %%F in (^"%%B) do (set allEngineAndGameCpps=!allEngineAndGameCpps! %%~F)

REM ---------------------------------------------
REM echo List of all Files: %allEngineAndGameCpps%
REM ---------------------------------------------

REM ---------------------------------------------
REM |	Building time
REM ---------------------------------------------

@echo on
REM ---------------------------------------------
REM | 	Running the thirdParty batch files
REM ---------------------------------------------
call "%myPath%\Engine\ThirdParty\ThirdPartyCBatch.bat"

REM ---------------------------------------------
REM | 	Delete old binary code
REM ---------------------------------------------
cd %myPath%
del Run_Win32\Data\Web\Resources\Engine.bc

REM ---------------------------------------------
REM |	Compile the Game and the Engine
REM ---------------------------------------------
cmd.exe /c em++ -std=c++14 -DFT2_BUILD_LIBRARY %thirdPartySource% %allEngineAndGameCpps% --memory-init-file 0 -O3 -o Run_Win32\Data\Web\Resources\Engine.bc -I%myPath% -s ASSERTIONS=2 -s USE_SDL=2 -s FULL_ES2=1 -DEMSCRIPTEN_PORT=1

REM ---------------------------------------------
REM Finished
REM ---------------------------------------------

pause 


REM cmd.exe /c em++ -std=c++14 %thirdPartySource% %allEngineAndGameCpps% -s FULL_ES2=1 -DEMSCRIPTEN_PORT=1 --memory-init-file 0 -O0 -o Web/ProtoGame.js -v -g -I%myPath% -s ASSERTIONS=2 -s USE_SDL=2 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 -s USE_WEBGL2=1