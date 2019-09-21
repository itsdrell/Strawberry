#include "Window.hpp"
#include "Engine/Internal/EmscriptenCommon.hpp"
#include "Engine/ThirdParty/SDL2/SDL.h"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/General/BlackBoard.hpp"

//===============================================================================================
static Window *g_theWindow = nullptr; // Instance Pointer; 

//===============================================================================================
Window::Window(const char* name, float width, float height)
{
	m_appName = name;
	m_width = width;
	m_height = height;
	m_originalResolution = Vector2(m_width, m_height);
	m_aspect = width/height;

	GetAllResolutions();

	CreateSDLWindow();

	g_theWindow = this;
}

//-----------------------------------------------------------------------------------------------
Window::Window(const char* name)
{
	GetAllResolutions();
	
	m_appName = name;
	m_width = (float)m_resolutions.at(0).x;
	m_height = (float)m_resolutions.at(0).y;
	m_originalResolution = Vector2(m_width, m_height);
	m_aspect = m_width / m_height;

	CreateSDLWindow();

	g_theWindow = this;
}

//-----------------------------------------------------------------------------------------------
Window::~Window()
{
	SDL_DestroyWindow(m_windowReference);
	g_theWindow = nullptr;
}

//-----------------------------------------------------------------------------------------------
void Window::CreateSDLWindow()
{
	m_windowReference = SDL_CreateWindow(
		m_appName, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		(int) m_width, 
		(int) m_height, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if(m_windowReference == NULL)
		printf("Window failed to create \n");

	// gross overlay thing
	//SDL_SetWindowBrightness(m_windowReference, 4.4);
}

//-----------------------------------------------------------------------------------------------
void Window::SetWindowSize(int x, int y)
{
	SDL_SetWindowSize(m_windowReference, x, y);
	SDL_SetWindowPosition(m_windowReference, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	
	m_width = (float) x;
	m_height = (float) y;
	m_aspect = m_width / m_height;

	Renderer::GetInstance()->ResizeViewport(x, y);
}

//-----------------------------------------------------------------------------------------------
void Window::ToggleFullscreenMode()
{
	m_isFullscreen = !m_isFullscreen;
	
	if (m_isFullscreen)
	{
		// These all had weird side effects I didn't like
		// SDL_WINDOW_FULLSCREEN_DESKTOP - Doesn't stretch just makes bigger
		// SDL_WINDOW_FULLSCREEN - This stretches the image resolution and makes fullscreen (and fucks your computer resolution up)
		// 0 for windowed mode
		//SDL_SetWindowFullscreen(m_windowReference, SDL_WINDOW_FULLSCREEN);

		// Gets the max resolution of the monitor and makes that the size of the window. 
		// Fake full screen
		SDL_DisplayMode current;
		SDL_GetCurrentDisplayMode(0, &current);
		SetWindowSize(current.w, current.h);
	}
	else
	{
		SDL_SetWindowFullscreen(m_windowReference, 0);
		SetWindowSize((int) m_originalResolution.x, (int) m_originalResolution.y);
	}
}

//-----------------------------------------------------------------------------------------------
STATIC Window* Window::GetInstance()
{
	return g_theWindow;
}

//-----------------------------------------------------------------------------------------------
STATIC SDL_Window* Window::GetWindowReference()
{
	return g_theWindow->m_windowReference;
}

//-----------------------------------------------------------------------------------------------
void Window::GetAllResolutions()
{
	IntVector2 resolution1 = g_theEngineBlackboard->GetValue("resolution1", IntVector2(600, 400));
	IntVector2 resolution2 = g_theEngineBlackboard->GetValue("resolution2", IntVector2(0, 0));
	IntVector2 resolution3 = g_theEngineBlackboard->GetValue("resolution3", IntVector2(0, 0));

	// I give this one an actual resolution so if the user deletes the option in config it will
	// always be able to make a window resolution
	m_resolutions.push_back(resolution1);

	if (resolution2 != IntVector2(0, 0))
		m_resolutions.push_back(resolution2);
	if (resolution3 != IntVector2(0, 0))
		m_resolutions.push_back(resolution3);
}

//-----------------------------------------------------------------------------------------------
void Window::ChangeResolutionByDirection(int y)
{
	if (y >= 0)
	{
		m_currentResolutionIndex++;

		if (m_currentResolutionIndex >= m_resolutions.size())
			m_currentResolutionIndex = 0;
	}

	if (y < 0)
	{
		m_currentResolutionIndex--;

		if (m_currentResolutionIndex < 0)
			m_currentResolutionIndex = (int) (m_resolutions.size() - 1);
	}

	IntVector2 currentResolution = m_resolutions.at(m_currentResolutionIndex);
	SetWindowSize(currentResolution.x, currentResolution.y);
}

