#pragma once
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include <string>
#include <vector>

//====================================================================================
// Forward Declare
//====================================================================================
class Camera;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
class DebugRenderingTask
{
public:
	DebugRenderingTask(float length, const Rgba& color)
		: m_timeToLive(length) 
		, m_color(color) {}

public: 
	virtual void Execute();

public:
	float	m_timeToLive;
	Rgba	m_color;
	bool	m_isReadyForDeletion = false;
};

//-----------------------------------------------------------------------------------------------
class DebugRenderingLogTask : public DebugRenderingTask
{
public:
	DebugRenderingLogTask(float length, const std::string& message, const Rgba& color = Rgba::WHITE);

public:
	virtual void Execute() override;

public:
	std::string m_message;
};

//-----------------------------------------------------------------------------------------------
struct DebugMessage
{
public:
	DebugMessage(const std::string& message, const Rgba& color = Rgba::WHITE)
		: m_message(message)
		, m_color(color) {}

public:
	std::string m_message;
	Rgba m_color;
};

//====================================================================================
// Classes
//====================================================================================
class DebugRenderingSystem
{
public:
	DebugRenderingSystem();
	~DebugRenderingSystem();

public:
	static DebugRenderingSystem* GetInstance(); 
	static bool IsInitialized() { return s_instance != nullptr; }

public:
	void AddTask(DebugRenderingTask* taskToAdd);
	void AddLogMessage(const std::string& message, const Rgba& color = Rgba::WHITE) { m_messagesToShow.push_back(DebugMessage(message, color)); }
	void UpdateAndRender(); 
	void RenderMessages();

private:
	std::vector<DebugRenderingTask*>		m_tasks;
	std::vector<DebugMessage>				m_messagesToShow;

private:
	Camera*			m_camera;
	float			m_windowWidth;
	float			m_windowHeight;

	AABB2			m_consoleWindow;

	float			m_fontSize;
	float			m_fontWidth;
	float			m_maxLengthOfTextToShow;
	float			m_maxRowsOfText;

private:
	static DebugRenderingSystem* s_instance;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
void DebugUpdateAndRender();

void DebugRenderLog(const std::string& message, float length = 1.f, const Rgba& color = Rgba::WHITE);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [10/26/2019]
//====================================================================================