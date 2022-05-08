#include "DebugRendering.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Renderer/Images/BitmapFont.hpp"

//===============================================================================================
DebugRenderingSystem* DebugRenderingSystem::s_instance = nullptr;

//===============================================================================================
DebugRenderingSystem::DebugRenderingSystem()
{
	Renderer* r = Renderer::GetInstance();

	m_consoleWindow = AABB2(Vector2(0.f, 0.f), Vector2(Window::GetInstance()->GetAspect(), 1));
	m_fontSize = m_consoleWindow.GetHeight() * (1.f / 50);

	m_fontWidth = r->m_defaultFont->GetWidthOfAGlyph(m_fontSize, Window::GetInstance()->GetAspect());
	m_maxLengthOfTextToShow = ((m_consoleWindow.GetWidth() / m_fontWidth) - 2.f);
	m_maxRowsOfText = ((m_consoleWindow.GetHeight() / m_fontSize) - 4.f);

	m_camera = new Camera();
	m_camera->SetColorTarget(r->m_defaultColorTarget);
	m_camera->m_projectionMatrix = Matrix44::MakeOrtho2D(m_consoleWindow.mins, m_consoleWindow.maxs);
	Matrix44 view = Matrix44::MakeView(Vector3(0.f, 0.f, -10.f), Vector3::ZERO);

	r->SetCamera();
}

//-----------------------------------------------------------------------------------------------
DebugRenderingSystem::~DebugRenderingSystem()
{
	for (uint i = 0; i < m_tasks.size(); i++)
	{
		DebugRenderingTask* current = m_tasks.at(i);

		delete current;
		current = nullptr;
	}

	m_tasks.clear();

	delete m_camera;
	m_camera = nullptr;
}

//-----------------------------------------------------------------------------------------------
DebugRenderingSystem* DebugRenderingSystem::GetInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new DebugRenderingSystem();
	}
	
	return s_instance;
}

//-----------------------------------------------------------------------------------------------
void DebugRenderingSystem::AddTask(DebugRenderingTask * taskToAdd)
{
	m_tasks.push_back(taskToAdd);
}

//-----------------------------------------------------------------------------------------------
void DebugRenderingSystem::UpdateAndRender()
{
	for (uint i = 0; i < m_tasks.size(); i++)
	{
		DebugRenderingTask* current = m_tasks.at(i);

		if (current->m_isReadyForDeletion)
		{
			delete current;
			current = nullptr;
			
			RemoveFast(i, m_tasks);
		}
		else
		{
			current->Execute();
		}
	}

	RenderMessages();
}

//-----------------------------------------------------------------------------------------------
void DebugRenderingSystem::RenderMessages()
{
	Renderer* r = Renderer::GetInstance();
	r->SetCamera(m_camera);
	
	Vector2 startPos = Vector2(.02f, .95f);
	for (uint i = 0; i < m_messagesToShow.size(); i++)
	{
		DebugMessage currentText = m_messagesToShow.at(i);
		
		r->DrawText2D(startPos, currentText.m_message, m_fontSize, currentText.m_color);

		startPos.y -= (m_fontSize + (m_fontSize * .1f));
	}

	m_messagesToShow.clear();

	r->SetCamera();
}

//===============================================================================================
void DebugUpdateAndRender()
{
	if(DebugRenderingSystem::IsInitialized())
		DebugRenderingSystem::GetInstance()->UpdateAndRender();
}

//-----------------------------------------------------------------------------------------------
void DebugRenderLog(const std::string& message, float length, const Rgba& color)
{
	DebugRenderingLogTask* newTask = new DebugRenderingLogTask(length, message, color);
	DebugRenderingSystem::GetInstance()->AddTask(newTask);
}

//=============================================================================================== 
void DebugRenderingTask::Execute()
{
	if (m_timeToLive > 0.f)
	{
		m_timeToLive -= g_theMasterClock->deltaTime;
	}
	else
	{
		m_isReadyForDeletion = true;
	}
}

//===============================================================================================
DebugRenderingLogTask::DebugRenderingLogTask(float length, const std::string& message, const Rgba& color)
	: DebugRenderingTask(length, color)
{
	m_message = message;
}

//-----------------------------------------------------------------------------------------------
void DebugRenderingLogTask::Execute()
{
	DebugRenderingTask::Execute();
	DebugRenderingSystem::GetInstance()->AddLogMessage(m_message, m_color);
}
