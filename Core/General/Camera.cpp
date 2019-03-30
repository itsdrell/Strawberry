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

}

//-----------------------------------------------------------------------------------------------
void Camera::LookAt(Vector3 position, Vector3 target, Vector3 up /*= Vector3(0.f, 1.f, 0.f)*/)
{
	// Look first creates the camera position
	m_cameraMatrix = Matrix44::LookAt(position, target, up);

	// Using the make view function its look at + inverse in one call
	m_viewMatrix = Matrix44::MakeView(position, target, up);
}

//-----------------------------------------------------------------------------------------------
void Camera::Translate2D(const Vector2& translation)
{
	m_cameraMatrix = Matrix44::MakeTranslation2D(translation);
}
