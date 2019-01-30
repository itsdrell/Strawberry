#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================
typedef bool (*windows_message_handler_cb)( unsigned int msg, size_t wparam, size_t lparam ); 

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
	// Do all the window creation work is currently in App
	// Could use Get Modular Handle instead of passing it
	Window( const char* name, float aspectRatio); 
	~Window();


	// Register a function callback to the Window.  Any time Windows processing a 
	// message, this callback should forwarded the information, along with the
	// supplied user argument. 
	void RegisterHandler( windows_message_handler_cb cb ); 

	// Allow users to unregister (not needed for this Assignment, but I 
	// like having cleanup methods). 
	void UnregisterHandler( windows_message_handler_cb cb ); 

	// This is safely castable to an HWND
	void* GetHandle() const { return m_hwnd; }
	float GetWidth() const {return m_width;}
	float GetHeight() const {return m_height;}
	float GetAspect() const { return m_aspect; } // aspect is width / height
	void* GetCursor() { return m_cursor; }
	static Window* GetInstance();

	// This may need to be called during construction
	void SetTitle( char const* new_title ); 

private:
	void* m_hwnd = nullptr; 

	float m_width;
	float m_height;
	float m_aspect;

	void* m_cursor;

	const char* APP_NAME = "Made with the Strawberry Engine :p ";

public:

	// When the WindowsProcedure is called - let all listeners get first crack at the message
	// Giving us better code modularity. 
	std::vector<windows_message_handler_cb> listeners; 
};



//====================================================================================
// Standalone C Functions
//====================================================================================
//bool CheckIfWindowIsActive();
//String GetClipboardText();
//void SetClipboardsData(std::string data);
//void ShowTheCursor(bool value);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================