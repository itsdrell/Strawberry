#pragma once
#include "Game/States/AppState.hpp"
#include "Engine/Core/General/EngineCommon.hpp"

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
	void CreateStates();
	void RunFrame(); // this tells everyone else to do a frame
	
public:
	void Update();
	void HandleInput();
	void Render() const;
	
public:
	void LoadAndRunGame(const String& projectName);
	void ReloadAndRunGame();
	float GetTimeSinceProgramStart() const {return m_timeSinceStart;} 

private:
	void HandleCaptures() const;
	void TakeScreenshot() const;
	void TakeGameCover() const;

	void TestTexture();

public:
	void TransitionToState(AppState stateToGoTo);

public:
	AppState m_currentState = NUM_OF_APP_STATES;
	IAppState* m_states[NUM_OF_APP_STATES];

public:
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