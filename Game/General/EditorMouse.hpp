#pragma once
#include "Engine/Math/Vectors/Vector2.hpp"

//====================================================================================
// Forward Declare
//====================================================================================


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
class EditorMouse
{
public:
	EditorMouse();
	~EditorMouse();

public:
	static EditorMouse* GetInstance() { return s_theMouse; }
	static void SetOnHoverable(bool value = true) { GetInstance()->m_onClickable = value; }

public:
	void BeginFrame();
	void Update();
	void Render() const;

private:
	Vector2 m_postion;
	bool m_onClickable = false;

private:
	static EditorMouse* s_theMouse;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/16/2021]
//====================================================================================