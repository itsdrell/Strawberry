#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class Clock;

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
class StopWatch
{
public:
	// should default to Master if ref_clock is set to nullptr
	StopWatch(Clock *ref_clock = nullptr);
	~StopWatch() { m_reference = nullptr; }

	// change our reference clock. 
	// should maintain elapsed time 
	void SetClock(Clock* ref_clock);

	// sets the "interval time" for
	// this stopwtach.
	// takes seconds to be easy to use
	// but under the hood it will use hpc
	bool SetTimer(float seconds);

	// if we really just want to use it as a timer
	// this returns how long since the last reset
	float GetElapsedTime() const;

	// elapsed time divided by the interval
	float GetNormalizedElapsedTime();

	// returns true if the interval has elapsed;
	bool HasElapsed() const;

	// resets the interval
	// (get_elapsed_time() should return 0.0f);
	void Reset();

	// checks the interval, if it has elapsed, will
	// return true and reset.  If not, will just return false (no reset)
	bool CheckAndReset();

public:
	// clock to use as reference
	Clock *m_reference;

	// I switched to seconds because it makes more sense
	// and shouldn't matter since im using the reference clock
	// to determine seconds and thats based off hpc soooo
	// its basically the same but less gross
	float m_startTime;
	float m_length;
};
typedef StopWatch	Timer;

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [5/16/2020]
//====================================================================================