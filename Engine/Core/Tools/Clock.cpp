#include "Clock.hpp"
#include "Engine/Core/Platform/Time.hpp"

//===============================================================================================
Clock* g_theMasterClock = nullptr;
Clock* g_theGameClock = nullptr;

//===============================================================================================
Clock::Clock(Clock * parent)
{
	m_parent = parent;

	Reset();
	m_frameCount = 0;

	m_lastFrameHpc = GetPerformanceCounter();

	if (parent != nullptr)
	{
		m_parent->AddChild(this);
	}

	if (g_theMasterClock == nullptr)
	{
		// if its null then this is the first constructor which is always master
		g_theMasterClock = this;
	}
}

//-----------------------------------------------------------------------------------------------
Clock::~Clock()
{
	if(m_parent)
		m_parent->RemoveChild(this);
	
	// you don't delete the parent, just the children
	for (uint i = 0; i < m_children.size(); i++)
	{
		Clock* current = m_children.at(i);

		delete current;
		current = nullptr;
	}

	m_children.clear();

	m_parent = nullptr;
}

//-----------------------------------------------------------------------------------------------
void Clock::Reset()
{
	frame = time_unit_t();
	total = time_unit_t();

	deltaTime = 0.f;
	totalTime = 0.f;
	m_frameCount = 0;

	m_startHpc = 0;
	m_lastFrameHpc = 0;
}

//-----------------------------------------------------------------------------------------------
void Clock::BeginFrame()
{
	uint64_t currentHpc = GetPerformanceCounter();
	uint64_t elapsed = currentHpc - m_lastFrameHpc;

	Advance(elapsed);
}

//-----------------------------------------------------------------------------------------------
void Clock::Advance(uint64_t elapsed)
{
	// frames always increase even when paused :)
	m_frameCount++;

	// so we can compares against the new frame hpc and get ds
	m_lastFrameHpc += elapsed;

	// Update the clocks member variables
	double elapsedSeconds = PerformanceCountToSeconds(elapsed);
	frame.hpcSeconds = elapsedSeconds;
	frame.seconds = (float)elapsedSeconds;
	deltaTime = frame.seconds; // storing it off for easier access
	frame.hpc = elapsed;
	frame.ms = (uint)(frame.hpcSeconds * 1000.f);

	total.hpcSeconds += frame.hpcSeconds;
	total.seconds += frame.seconds;
	totalTime += deltaTime;
	total.hpc += frame.hpc;
	total.ms += frame.ms;

	// update all children
	for (Clock* iterator : m_children)
	{
		iterator->Advance(elapsed);
	}
}

//-----------------------------------------------------------------------------------------------
void Clock::AddChild(Clock *child)
{
	m_children.push_back(child);
}

//-----------------------------------------------------------------------------------------------
void Clock::RemoveChild(Clock* child)
{
	for (uint i = 0; i < m_children.size(); i++)
	{
		if (child == m_children.at(i))
			RemoveFast(i, m_children);
	}
}

//===============================================================================================
void ClockSystemBeginFrame()
{
	g_theMasterClock->BeginFrame();
}

//-----------------------------------------------------------------------------------------------
Clock * GetMasterClock()
{
	return g_theMasterClock;
}

//-----------------------------------------------------------------------------------------------
float GetDeltaTime()
{
	return g_theMasterClock->frame.seconds;
}

//-----------------------------------------------------------------------------------------------
float GetFPS()
{
	return (1.f / g_theMasterClock->deltaTime);
}
