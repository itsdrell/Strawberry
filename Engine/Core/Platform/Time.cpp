#include "Time.hpp"
#include "Engine/ThirdParty/SDL2/SDL_timer.h"
#include <time.h>

#pragma warning(disable:4996) // for localtime

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

//-----------------------------------------------------------------------------------------------
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
// Source: https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
String CurrentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now); // this gives a warning

	// year_month_day_hour-min-sec
	strftime(buf, sizeof(buf), "%Y_%m_%d_%H-%M-%S", &tstruct);

	return buf;
}
