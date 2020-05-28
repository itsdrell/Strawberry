#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Game/States/AppState.hpp"
#include "Engine/Core/Tools/StopWatch.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class LuaScript;
class Map;

//====================================================================================
// Type Defs + Defines
//====================================================================================
constexpr int CAMERA_SCREEN_SIZE = 256; // how big is the cameras view

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class Game : IAppState
{
public:
	Game();
	~Game();

public:
	void		StartUp();
	void		CleanUp();

public:
	virtual void Update() override;
	virtual void Render() const override;

	virtual void OnEnter() {};
	virtual void OnExit();

public:
	void		RenderGame() const;
	void		RenderGameShell(float padding, float size) const;
	void		RenderError() const;

public:
	static void LoadOrReloadSpriteSheet();
	static Game* GetInstance();

public:
	void ShakeCamera();

public:
	LuaScript*		m_mainLuaScript;
	String			m_texturePath;

	Map*			m_map;
	bool			m_drawMap;

	Vector2			m_cameraPos;
	float			m_cameraAngle = 0.f;
	float			m_screenshakeAmount = 0.f; // [0-1]

	float			m_timeScale = 1.f;
	float			m_sleepTimer = 0.f;

	float			m_lengthBeforeTakingScreenshot = 10.f; // 10 seconds
	StopWatch		m_takeDefaultCoverScreenshotTimer;
	
private:
	// I had to make this a bool because the engine config was getting corrupted(?)
	// and crashing the game if I checked for it constantly :l 
	bool			m_showBorder = true;

};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================
extern Game* g_theGame;

//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================