#include "CurveEditor.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Geometry/Disc.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Math/Splines/CubicSpline.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include "Engine/Renderer/Systems/MeshBuilder.hpp"
#include "Engine/Core/Platform/File.hpp"
#include "Engine/Core/Utils/StrawberryFileUtils.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"
#include "Game/General/EditorMouse.hpp"


//===============================================================================================
float CurveData::Evaluate(float t)
{
	Vector2 pos = EvaluateCubicBezier(Vector2(0, 0), m_controlPointA, m_controlPointB, Vector2(1, 1), t);
	return pos.y;
}

//===============================================================================================
CurveEditor::CurveEditor()
{
	GenerateCurveData();

	m_controlPointANode.m_position = m_curves[m_selectedCurve].m_controlPointA;
	m_controlPointBNode.m_position = m_curves[m_selectedCurve].m_controlPointB;

	Renderer* r = Renderer::GetInstance();
	m_camera = new Camera();
	m_camera->SetColorTarget(r->m_defaultColorTarget);
	r->SetCamera(nullptr);

	m_colorOfCircleOnLine = Rgba::GetRandomColor();
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::Update()
{
	HandleInput();
	UpdateHover();

	m_camera->SetProjectionOrthoByAspect(m_zoomedInAmount);
	m_camera->GoToPosition2D(Vector2(.5f, .5f));
	m_cameraBounds = m_camera->GetOrthoBounds();
	GenerateBounds();

	if(GetFractionOf(g_theMasterClock->totalTime) < .025f)
	{
		m_colorOfCircleOnLine = Rgba::GetRandomColor();
	}
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::Render() const
{
	Renderer* r = Renderer::GetInstance();
	r->SetCamera(m_camera);

	r->DrawAABB2Filled(AABB2(-10.f, -10.f, 10.f, 10.f), Rgba::STRAWBERRY_RED);

	RenderGrid();
	RenderUIKnobs();
	RenderLine();

	// on curve progress ball
	float onCurveT = GetFractionOf(g_theMasterClock->totalTime);
	Vector2 pos = EvaluateCubicBezier(Vector2(0, 0), m_controlPointANode.m_position, m_controlPointBNode.m_position, Vector2(1, 1), onCurveT);
	r->DrawCircleFilled2D(pos, .025f, m_colorOfCircleOnLine);
	r->DrawCircleOutline2D(pos, .025f, Rgba::WHITE);

	r->DrawAABB2Outline(AABB2(0.f, 0.f, 1.f, 1.f), Rgba::WHITE);

	RenderLinePreview();
	RenderCurveSelector();

	// mouse
	//Vector2 mousePos = GetMousePosition(m_cameraBounds);
	//r->DrawCircleOutline2D(mousePos, .02f, Rgba(0, 255, 0, 255));
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

		for (uint i = 0; i < MAX_AMOUNT_OF_CURVES; i++)
		{
			if (m_splineButtonsBounds[i].IsPointInBox(mousePos))
			{
				SwitchToCurve(i);
			}
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

	if(WasKeyJustPressed(KEYBOARD_UP_ARROW))
	{
		int newIndex = (m_selectedCurve - 1 + MAX_AMOUNT_OF_CURVES) % MAX_AMOUNT_OF_CURVES;
		SwitchToCurve(newIndex);
	}

	if(WasKeyJustPressed(KEYBOARD_DOWN_ARROW))
	{
		int newIndex = (m_selectedCurve + 1) % MAX_AMOUNT_OF_CURVES;
		SwitchToCurve(newIndex);
	}

	if(IsKeyPressed(KEYBOARD_CTRL) && WasKeyJustPressed('s'))
	{
		Save();
	}
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::UpdateHover()
{
	Vector2 mousePos = GetMousePosition(m_camera->GetOrthoBounds());

	for (uint i = 0; i < MAX_AMOUNT_OF_CURVES; i++)
	{
		if (m_splineButtonsBounds[i].IsPointInBox(mousePos))
		{
			EditorMouse::GetInstance()->SetOnHoverable();
		}
	}

	if (Disc2(m_controlPointANode.m_position, m_controlPointANode.m_radius * m_zoomedInAmount).IsPointInside(mousePos))
	{
		EditorMouse::GetInstance()->SetOnHoverable();
	}

	if (Disc2(m_controlPointBNode.m_position, m_controlPointBNode.m_radius * m_zoomedInAmount).IsPointInside(mousePos))
	{
		EditorMouse::GetInstance()->SetOnHoverable();
	}
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::RenderLinePreview() const
{
	Renderer* r = Renderer::GetInstance();
	
	float onCurveT = GetFractionOf(g_theMasterClock->totalTime);
	Vector2 pos = EvaluateCubicBezier(Vector2(0, 0), m_controlPointANode.m_position, m_controlPointBNode.m_position, Vector2(1, 1), onCurveT);

	Vector2 topPoint = m_linePreviewBounds.GetPositionWithinBox(Vector2(.5f, .9f));
	Vector2 bottomPoint = m_linePreviewBounds.GetPositionWithinBox(Vector2(.5f, .1f));
	Vector2 circlePos = Interpolate(bottomPoint, topPoint, pos.y);
	
	r->DrawAABB2Filled(m_linePreviewBounds, Rgba::STRAWBERRY_RED);
	r->DrawAABB2Outline(m_linePreviewBounds, Rgba::WHITE);
	r->DrawLine2D(bottomPoint, topPoint, Rgba::WHITE);
	
	r->DrawCircleFilled2D(circlePos, .025f * m_zoomedInAmount, m_colorOfCircleOnLine);
	r->DrawCircleOutline2D(circlePos, .025f * m_zoomedInAmount, Rgba::WHITE);
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::RenderCurveSelector() const
{
	Renderer* r = Renderer::GetInstance();

	r->DrawAABB2Filled(m_splineButtonOverallBounds, Rgba::STRAWBERRY_RED);
	r->DrawAABB2Outline(m_splineButtonOverallBounds, Rgba::WHITE);

	for(uint i = 0; i < MAX_AMOUNT_OF_CURVES; i++)
	{
		AABB2 currentBounds = m_splineButtonsBounds[i];

		r->DrawAABB2Outline(currentBounds, Rgba::WHITE);
		r->DrawTextInBox(std::to_string(i), currentBounds, 8, 1.f, DRAW_TEXT_MODE_SHRINKED, Vector2(.5f, .5f));
	}

	r->DrawAABB2Outline(m_splineButtonsBounds[m_selectedCurve], Rgba::GREEN);
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::RenderLine() const
{
	std::vector<Vector3> positions;
	positions.reserve(100);
	for (int i = 0; i < 100; i++)
	{
		float t = (float)i / (float)100;
		Vector2 pos = EvaluateCubicBezier(Vector2(0, 0), m_controlPointANode.m_position, m_controlPointBNode.m_position, Vector2(1, 1), t);

		positions.push_back(Vector3(pos.x, pos.y, 0));
	}

	MeshBuilder mb;
	mb.AddLines(positions, Rgba::BLACK);
	Renderer::GetInstance()->DrawMesh(mb.CreateMesh<Vertex3D_PCU>(), true);
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::RenderGrid() const
{
	Renderer* r = Renderer::GetInstance();
	
	std::vector<Vector3> horizontalLines;
	std::vector<Vector3> verticalLines;
	float tenth = -5.f;
	for (int i = -50; i < 50; i++)
	{
		horizontalLines.push_back(Vector3(-100.f, tenth, 0.f));
		horizontalLines.push_back(Vector3(100, tenth, 0.f));

		verticalLines.push_back(Vector3(tenth, -100.f, 0.f));
		verticalLines.push_back(Vector3(tenth, 100, 0.f));

		tenth += .2;
	}

	Rgba lineColor = Rgba(200, 200, 200, 50);
	MeshBuilder mb;

	mb.AddLines(horizontalLines, lineColor);
	r->DrawMesh(mb.CreateMesh<Vertex3D_PCU>(), true);
	
	mb.AddLines(verticalLines, lineColor);
	r->DrawMesh(mb.CreateMesh<Vertex3D_PCU>(), true);
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::RenderUIKnobs() const
{
	Renderer* r = Renderer::GetInstance();

	r->DrawCircleFilled2D(m_controlPointBNode.m_position, m_controlPointBNode.m_radius * m_zoomedInAmount, Rgba::RAINBOW_BLUE);
	r->DrawCircleOutline2D(m_controlPointBNode.m_position, m_controlPointBNode.m_radius * m_zoomedInAmount, Rgba::BLUE);
	r->DrawLine2D(Vector2(1, 1), m_controlPointBNode.m_position, Rgba::BLUE);

	r->DrawCircleFilled2D(m_controlPointANode.m_position, m_controlPointANode.m_radius * m_zoomedInAmount, Rgba::RAINBOW_RED);
	r->DrawCircleOutline2D(m_controlPointANode.m_position, m_controlPointANode.m_radius * m_zoomedInAmount, Rgba::RED);
	r->DrawLine2D(Vector2(0, 0), m_controlPointANode.m_position, Rgba::RED);
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::GenerateCurveData()
{
	std::string filePath = GetCurveDataFilePath();

	if (DoesDirectoryExist(filePath.c_str()))
	{
		LoadCurveDataFromFile(m_curves);
	}
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::GenerateBounds()
{
	m_linePreviewBounds = GetAABB2FromAABB2(Vector2(.95f, .2f), Vector2(1.f, .8f), m_cameraBounds);

	m_splineButtonOverallBounds = GetAABB2FromAABB2(Vector2(.0f, .2f), Vector2(.05f, .8f), m_cameraBounds);

	float amount = 1.f;
	for(uint i = 0; i < MAX_AMOUNT_OF_CURVES; i++)
	{
		m_splineButtonsBounds[i] = GetAABB2FromAABB2(Vector2(.0f, amount - .1f), Vector2(1.f, amount), m_splineButtonOverallBounds);
		amount -= .1f;
	}
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::SwitchToCurve(int index)
{
	// save old positions
	m_curves[m_selectedCurve].m_controlPointA = m_controlPointANode.m_position;
	m_curves[m_selectedCurve].m_controlPointB = m_controlPointBNode.m_position;
	
	m_selectedCurve = index;
	
	// put the nodes on the new curves pos
	m_controlPointANode.m_position = m_curves[m_selectedCurve].m_controlPointA;
	m_controlPointBNode.m_position = m_curves[m_selectedCurve].m_controlPointB;
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::Save()
{
	m_curves[m_selectedCurve].m_controlPointA = m_controlPointANode.m_position;
	m_curves[m_selectedCurve].m_controlPointB = m_controlPointBNode.m_position;

	SaveCurveDataToFile();
	DebugRenderLog("Saved! :)", 3.f);
}

//-----------------------------------------------------------------------------------------------
void CurveEditor::SaveCurveDataToFile()
{
	// controlPointA.x,controlPointA.y,controlPointB.x,controlPointB.y
	std::string content;
	for (uint i = 0; i < MAX_AMOUNT_OF_CURVES; i++)
	{
		content += std::to_string(m_curves[i].m_controlPointA.x) + ",";
		content += std::to_string(m_curves[i].m_controlPointA.y) + ",";
		
		content += std::to_string(m_curves[i].m_controlPointB.x) + ",";
		content += std::to_string(m_curves[i].m_controlPointB.y) + " \n";
	}

	std::string filePath = GetCurveDataFilePath();
	if (DoesDirectoryExist(filePath.c_str()))
	{
		CreateADirectory(filePath.c_str());
	}

	LogStringToFile(filePath.c_str(), content.c_str(), true);
}

//===============================================================================================
void LoadCurveDataFromFile(CurveData* outArray)
{
	String filePath = GetCurveDataFilePath();
	Strings data = GetAllLinesFromFile(filePath.c_str());

	for (uint i = 0; i < data.size(); i++)
	{
		String currentLine = data.at(i);
		Strings brokenUpLine = SplitString(currentLine, ",");
		
		outArray[i].m_controlPointA.x = ParseString(brokenUpLine.at(0), 0.f);
		outArray[i].m_controlPointA.y = ParseString(brokenUpLine.at(1), 0.f);

		outArray[i].m_controlPointB.x = ParseString(brokenUpLine.at(2), 1.f);
		outArray[i].m_controlPointB.y = ParseString(brokenUpLine.at(3), 1.f);
	}
}