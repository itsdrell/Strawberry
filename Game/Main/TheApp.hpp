#pragma once

//====================================================================================
// Forward Declare
//====================================================================================
class Command;
class GifRecorder;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================
enum AppState
{
	APPSTATE_EDITOR,
	APPSTATE_GAME,
	APPSTATE_CONSOLE,
	NUM_OF_APP_STATES
};

//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class App
{

public:
	App();
	~App();
	
public:
	void StartUp(); 
	void RunFrame(); // this tells everyone else to do a frame
	
public:
	void Update();
	void HandleInput();
	void Render() const;
	
public:
	void ReloadAndRunGame();

	float GetTimeSinceProgramStart() const {return m_timeSinceStart;} 

private:
	void HandleCaptures() const;
	void TakeScreenshot() const;
	void TakeGameCover() const;

	void TestTexture();

public:
	AppState m_currentState = APPSTATE_CONSOLE;
	bool m_isQuitting;
	bool m_isReleaseVersion = false;
	bool m_takeScreenshot = false;
	bool m_takeGameCoverScreenshot = false;
	float m_timeSinceStart;

private:
	GifRecorder*	m_recorder = nullptr;

};

//====================================================================================
// Standalone C Functions
//====================================================================================
void Quit();
void RunProject(Command& command);

//====================================================================================
// Externs
//====================================================================================
extern App* g_theApp;

//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================