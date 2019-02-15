#pragma once
#include "Engine/Math/Matrices/Matrix44.hpp"
#include "Engine/Renderer/Pipeline/FrameBuffer.hpp"
#include "Engine/Core/General/EngineCommon.hpp"


//====================================================================================
// Forward Declare
//====================================================================================
class Texture;

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
class Camera
{
public:
	Camera();
	Camera( const Matrix44& cameraMatrix, const Matrix44& view, const Matrix44& projection );

	~Camera();

public:
	void SetColorTarget( Texture* colorTarget ) { m_output.SetColorTarget(colorTarget); }
	void SetProjectionOrthoByAspect( float height, float theNear = -10.f, float theFar = 100.f );

public:
	FrameBuffer GetFramebuffer() { return m_output; }
	uint GetFrameBufferID() { return m_output.m_ID; }

public:
	Matrix44		m_cameraMatrix; // where is the camera in the world
	Matrix44		m_viewMatrix; // inverse of camera
	Matrix44		m_projectionMatrix;

public:
	FrameBuffer		m_output;
	Vector2			m_orthoDimensions;

};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/31/2019]
//====================================================================================