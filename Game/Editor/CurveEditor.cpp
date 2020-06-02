#include "CurveEditor.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/Disc.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Math/Splines/CubicSpline.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Core/Tools/Clock.hpp"

//===============================================================================================
CurveEditor::CurveEditor()
{
	GenerateCurveData();
	GenerateBounds();

	m_controlPointANode.m_position = m_curves[m_selectedCurve].m_controlPointA;
	m_controlPointBNode.m_position = m_curves[m_selectedCurve].m_controlPointB;

	Renderer* r = Renderer::GetInstance();
	m_camera = new Camera();
	m_camera->SetColorTarget(r->m_defaultColorTarget);
	r->SetCamera(nullptr);
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::Update()
{
	HandleInput();
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::Render() const
{
	Renderer* r = Renderer::GetInstance();
	r->SetCamera(m_camera);
	m_camera->SetProjectionOrthoByAspect(m_zoomedInAmount);
	m_camera->GoToPosition2D(Vector2(.5f, .5f));

	Vector2 mousePos = GetMousePosition(m_camera->GetOrthoBounds());

	r->DrawAABB2Filled(AABB2(-10.f, -10.f, 10.f, 10.f), Rgba::STRAWBERRY_RED);
	r->DrawAABB2Outline(AABB2(0, 0, 1, 1), Rgba::WHITE);

	float tenth = -5.f;
	for (int i = -50; i < 50; i++)
	{
		r->DrawLine2D(Vector2(-100, tenth), Vector2(100, tenth), Rgba(200,200,200,150));
		r->DrawLine2D(Vector2(tenth, -100), Vector2(tenth, 100), Rgba(200, 200, 200, 150));

		tenth += .1;
	}

	for (int i = 0; i < 100; i++)
	{
		float t = (float)i / (float)100;
		Vector2 pos = EvaluateCubicBezier(Vector2(0, 0), m_controlPointANode.m_position, m_controlPointBNode.m_position, Vector2(1, 1), t);

		r->DrawCircleFilled2D(pos, .005f, Rgba::BLACK);

		r->DrawCircleOutline2D(m_controlPointANode.m_position, m_controlPointANode.m_radius * m_zoomedInAmount, Rgba::YELLOW);
		r->DrawLine2D(Vector2(0, 0), m_controlPointANode.m_position, Rgba::YELLOW);

		r->DrawCircleOutline2D(m_controlPointBNode.m_position, m_controlPointBNode.m_radius * m_zoomedInAmount, Rgba::GREEN);
		r->DrawLine2D(Vector2(1, 1), m_controlPointBNode.m_position, Rgba::GREEN);
	}

	// on curve progress ball
	float onCurveT = GetFractionOf(g_theMasterClock->totalTime);
	Vector2 pos = EvaluateCubicBezier(Vector2(0, 0), m_controlPointANode.m_position, m_controlPointBNode.m_position, Vector2(1, 1), onCurveT);
	r->DrawCircleFilled2D(pos, .025f, Rgba::GetRandomColor());

	r->DrawAABB2Outline(AABB2(0.f, 0.f, 1.f, 1.f), Rgba::WHITE);

	// mouse
	r->DrawCircleOutline2D(mousePos, .02f, Rgba(0, 255, 0, 255));
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::HandleInput()
{
	Vector2 mousePos = GetMousePosition(m_camera->GetOrthoBounds());

	if (WasMouseButtonJustReleased(LEFT_MOUSE_BUTTON))
	{
		m_controlPointANode.m_isSelected = false;
		m_controlPointBNode.m_isSelected = false;
	}

	if (WasMouseButtonJustPressed(LEFT_MOUSE_BUTTON))
	{
		if (Disc2(m_controlPointANode.m_position, m_controlPointANode.m_radius * m_zoomedInAmount).IsPointInside(mousePos))
		{
			m_controlPointANode.m_isSelected = true;
		}

		if (Disc2(m_controlPointBNode.m_position, m_controlPointBNode.m_radius * m_zoomedInAmount).IsPointInside(mousePos))
		{
			m_controlPointBNode.m_isSelected = true;
		}
	}

	if(DidMouseWheelScrollUp())
	{
		m_zoomedInAmount = ClampFloat(m_zoomedInAmount - .2f, MIN_ZOOMED_IN_LEVEL_CURVE_EDITOR, MAX_ZOOMED_OUT_LEVEL_CURVE_EDITOR);
	}

	if (DidMouseWheelScrollDown())
	{
		m_zoomedInAmount = ClampFloat(m_zoomedInAmount + .2f, MIN_ZOOMED_IN_LEVEL_CURVE_EDITOR, MAX_ZOOMED_OUT_LEVEL_CURVE_EDITOR);
	}

	if (m_controlPointANode.m_isSelected)
	{
		m_controlPointANode.m_position = mousePos;
	}

	if (m_controlPointBNode.m_isSelected && 
		m_controlPointANode.m_isSelected == false) // A gets priority if they are stacked
	{
		m_controlPointBNode.m_position = mousePos;
	}
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::GenerateCurveData()
{

}

//-----------------------------------------------------------------------------------------------
void CurveEditor::GenerateBounds()
{

}
