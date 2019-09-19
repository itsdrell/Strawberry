#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
struct SDL_Window;

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
class Window
{
public: 
	Window( const char* name, float width, float height);
	~Window();

	void CreateSDLWindow();

public:
	float GetWidth() const {return m_width;}
	float GetHeight() const {return m_height;}
	float GetAspect() const { return m_aspect; } // aspect is width / height
	Vector2 GetDimensions() const { return Vector2(m_width, m_height); }

public:
	void SetWindowSize(int x, int y);
	void ToggleFullscreenMode();

public:
	static Window* GetInstance();
	static SDL_Window* GetWindowReference();

private:
	float m_width;
	float m_height;
	float m_aspect;

	Vector2 m_originalResolution;
	bool  m_isFullscreen = false;

	const char* m_appName = "Made with the Strawberry Engine :p ";

	SDL_Window*		m_windowReference = nullptr;
};



//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================