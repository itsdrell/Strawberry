#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/ThirdParty/SDL2/SDL_config.h"

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct time_unit_t
{
	time_unit_t(){}

	uint64_t	hpc = 0;
	double		hpcSeconds = 0.0;  // high precision seconds 
	uint		ms = 0;

	float		seconds = 0.f;      // convenience float seconds 
};

//====================================================================================
// Classes
//====================================================================================
class Clock
{
public:
	Clock( Clock* parent = nullptr);
	~Clock();

public:
	void Reset();
	void BeginFrame();
	void Advance(uint64_t elapsed);

	void AddChild(Clock *child);

public:
	time_unit_t			frame;
	time_unit_t			total;

	// for game clock so i can just say g_theGameClock.deltaTime like unity
	float				deltaTime = 0.f;
	float				totalTime = 0.f;
	uint				m_frameCount = 0;

private:
	uint64_t			m_startHpc = 0;      // hpc when the clock was last reset (made)
	uint64_t			m_lastFrameHpc = 0; // hpc during last begin_frame call

private:
	Clock* m_parent;

	std::vector<Clock*> m_children;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
// convenience - calls begin frame on the master clock
void ClockSystemBeginFrame();

Clock* GetMasterClock();
float GetDeltaTime();
float GetFPS();

//====================================================================================
// Externs
//====================================================================================
extern Clock* g_theMasterClock;
extern Clock* g_theGameClock;

//====================================================================================
// Written by Zachary Bracken : [3/29/2019]
//====================================================================================