#pragma once
#include "Engine/Math/Matrices/Matrix44.hpp"
#include "Engine/Renderer/Pipeline/FrameBuffer.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"

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
	void SetProjectionOrtho2D(const Vector2& mins, const Vector2& maxs);

public:
	void LookAt(Vector3 position, Vector3 target, Vector3 up = Vector3(0.f, 1.f, 0.f));  
	void GoToPosition2D(const Vector2& translation);

public:
	FrameBuffer GetFramebuffer() { return m_output; }
	uint GetFrameBufferID() { return m_output.m_ID; }
	AABB2 GetOrthoBounds() const { return m_orthoBounds; }
	AABB2 GetBounds() const;

public:
	Matrix44		m_cameraMatrix; // where is the camera in the world
	Matrix44		m_viewMatrix; // inverse of camera
	Matrix44		m_projectionMatrix;

public:
	FrameBuffer		m_output;
	Vector2			m_orthoDimensions;
	AABB2			m_orthoBounds;

};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================
extern Camera* g_theGameCamera;

//====================================================================================
// Written by Zachary Bracken : [1/31/2019]
//====================================================================================