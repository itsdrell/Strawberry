#include "Time.hpp"
#include "Engine/ThirdParty/SDL2/SDL_timer.h"

//===============================================================================================
// local to time.cpp (could also use static variables)
class LocalTimeData
{
public:
	LocalTimeData()
	{
		//::QueryPerformanceFrequency((LARGE_INTEGER*)&m_hpc_per_second);

		// do the divide now, to not pay the cost later

		//-----------------------------------------------------------------------------------------------
		// this may be wrong. It was originally 1 but it was giving too small a value
		m_seconds_per_hpc = (1.0 / (double)SDL_GetPerformanceFrequency());
		//-----------------------------------------------------------------------------------------------
	}

public:
	uint64_t m_hpc_per_second;
	double m_seconds_per_hpc; // frequency
};

//------------------------------------------------------------------------
// Declaring on the global scope - will 
// cause constructor to be called before 
// our entry function. 
static LocalTimeData g_localTimeData;


//===============================================================================================
uint64_t GetPerformanceCounter()
{
	return SDL_GetPerformanceCounter();
}

//-----------------------------------------------------------------------------------------------
double PerformanceCountToSeconds(uint64_t hpc)
{
	return (double)hpc * g_localTimeData.m_seconds_per_hpc;
}
