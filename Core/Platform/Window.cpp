#include "Window.hpp"
#include "Engine/Internal/EmscriptenCommon.hpp"
#include "Engine/ThirdParty/SDL2/SDL.h"

//===============================================================================================
static Window *g_theWindow = nullptr; // Instance Pointer; 

//===============================================================================================
Window::Window(const char* name, float width, float height)
{
	m_appName = name;
	m_width = width;
	m_height = height;

	m_aspect = width/height;

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
		m_width, 
		m_height, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if(m_windowReference == NULL)
		printf("Window failed to create \n");
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

