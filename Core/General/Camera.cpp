#include <Engine/Core/General/Camera.hpp>
#include "Engine/Math/Matrices/Matrix44.hpp"
#include "Engine/Math/Vectors/Vector3.hpp"

#ifdef EMSCRIPTEN_PORT
#else
#include "Engine/Core/Platform/Window.hpp"
#endif
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

#ifdef EMSCRIPTEN_PORT
	float ar = 1.77f;
#else
	float ar = Window::GetInstance()->GetAspect();
#endif
	m_orthoDimensions = Vector2(ar * height, height);

	m_projectionMatrix = Matrix44::MakeOrtho3D(
		Vector3(-m_orthoDimensions.x *.5f , -m_orthoDimensions.y *.5f, theNear),
		Vector3(m_orthoDimensions.x * .5f , m_orthoDimensions.y *.5f,  theFar));

}
