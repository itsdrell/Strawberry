#include "Stopwatch.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include "Engine/Core/Platform/Time.hpp"


//===============================================================================================
StopWatch::StopWatch(Clock* ref_clock)
{
	if (ref_clock == nullptr)
	{
		m_reference = g_theMasterClock;
	}
	else
	{
		m_reference = ref_clock;
	}

	m_startTime = m_reference->total.seconds;
}

//-----------------------------------------------------------------------------------------------
void StopWatch::SetClock(Clock* ref_clock)
{
	m_reference = ref_clock;
}

//-----------------------------------------------------------------------------------------------
bool StopWatch::SetTimer(float seconds)
{
	m_startTime = m_reference->total.seconds;
	m_length = seconds;
	return true;
}

//-----------------------------------------------------------------------------------------------
float StopWatch::GetElapsedTime() const
{
	return (m_startTime + m_length) - m_reference->total.seconds;
}

//-----------------------------------------------------------------------------------------------
float StopWatch::GetNormalizedElapsedTime()
{
	if (HasElapsed())
		return 1.f;

	float percentDone = 1.f - (GetElapsedTime() / (float)m_length);

	return percentDone;
}

//-----------------------------------------------------------------------------------------------
bool StopWatch::HasElapsed() const
{
	float endTime = m_startTime + m_length;

	if (m_reference->total.seconds > endTime)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
void StopWatch::Reset()
{
	m_startTime = m_reference->total.seconds;
}

//-----------------------------------------------------------------------------------------------
bool StopWatch::CheckAndReset()
{
	if (HasElapsed())
	{
		Reset();
		return true;
	}

	return false;
}