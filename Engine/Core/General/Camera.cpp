#include <Engine/Core/General/Camera.hpp>
#include "Engine/Math/Matrices/Matrix44.hpp"
#include "Engine/Math/Vectors/Vector3.hpp"


#ifdef EMSCRIPTEN_PORT
#else
#include "Engine/Core/Platform/Window.hpp"
#endif

//===============================================================================================
Camera* g_theGameCamera = nullptr;

//===============================================================================================
Camera::Camera()
{
	m_cameraMatrix = Matrix44();
	m_viewMatrix = Matrix44();
	m_projectionMatrix = Matrix44();
}

//-----------------------------------------------------------------------------------------------
Camera::Camera(const Matrix44 & cameraMatrix, const Matrix44 & view, const Matrix44 & projection)
{
	m_cameraMatrix = cameraMatrix;
	m_viewMatrix = view;
	m_projectionMatrix = projection;
}

//-----------------------------------------------------------------------------------------------
Camera::~Camera()
{
}

//-----------------------------------------------------------------------------------------------
void Camera::SetProjectionOrthoByAspect(float height, float theNear /*= -10.f*/, float theFar /*= 100.f */)
{
	float ar = 1.77f;
#ifndef EMSCRIPTEN_PORT
	 ar = Window::GetInstance()->GetAspect();
#endif
	m_orthoDimensions = Vector2(ar * height, height);

	m_projectionMatrix = Matrix44::MakeOrtho3D(
		Vector3(-m_orthoDimensions.x *.5f , -m_orthoDimensions.y *.5f, theNear),
		Vector3(m_orthoDimensions.x * .5f , m_orthoDimensions.y *.5f,  theFar));

	float halfWidth = m_orthoDimensions.x * .5f;
	float halfHeight = m_orthoDimensions.y * .5f;
	m_orthoBounds = AABB2(-halfWidth, -halfHeight, halfWidth, halfHeight);
}

//-----------------------------------------------------------------------------------------------
void Camera::SetProjectionOrtho2D(const Vector2& mins, const Vector2& maxs)
{
	m_orthoBounds = AABB2(mins, maxs);
	m_projectionMatrix = Matrix44::MakeOrtho2D(mins, maxs);
}

//-----------------------------------------------------------------------------------------------
void Camera::LookAt(Vector3 position, Vector3 target, Vector3 up /*= Vector3(0.f, 1.f, 0.f)*/)
{
	// Look first creates the camera position
	m_cameraMatrix = Matrix44::LookAt(position, target, up);

	m_viewMatrix = Matrix44::Invert(m_cameraMatrix);

	// Using the make view function its look at + inverse in one call
	//m_viewMatrix = Matrix44::MakeView(position, target, up);
}

//-----------------------------------------------------------------------------------------------
void Camera::GoToPosition2D(const Vector2& translation)
{
	m_cameraMatrix = Matrix44::MakeTranslation2D(translation);

	m_viewMatrix = Matrix44::Invert(m_cameraMatrix);

	float halfWidth = m_orthoDimensions.x * .5f;
	float halfHeight = m_orthoDimensions.y * .5f;
	m_orthoBounds = AABB2(-halfWidth + translation.x, -halfHeight + translation.y, 
		translation.x + halfWidth, translation.y + halfHeight);
}

//-----------------------------------------------------------------------------------------------
AABB2 Camera::GetBounds() const
{
	AABB2 bounds = GetOrthoBounds();

	Vector3 pos = m_cameraMatrix.GetPosition();
	bounds.Translate(pos.x, pos.y);

	return bounds;
}
