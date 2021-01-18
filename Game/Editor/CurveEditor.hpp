#pragma once
#include "EditorMode.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Rgba.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class Camera;

//====================================================================================
// Type Defs + Defines
//====================================================================================
constexpr int MAX_AMOUNT_OF_CURVES = 10;

constexpr float MAX_ZOOMED_OUT_LEVEL_CURVE_EDITOR = 4.f;
constexpr float MIN_ZOOMED_IN_LEVEL_CURVE_EDITOR = 2.f;

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct CurveData
{
	CurveData(const Vector2& controlPointA, const Vector2& controlPointB)
		: m_controlPointA(controlPointA)
		, m_controlPointB(controlPointB) {}

	CurveData() {}

	float Evaluate(float t);

	Vector2 m_controlPointA = Vector2(.25f, .75f);
	Vector2 m_controlPointB = Vector2(.75f, .25f);
};

//-----------------------------------------------------------------------------------------------
struct ControlPointUINode
{
	ControlPointUINode(const Vector2& position)
		: m_position(position) {}

	ControlPointUINode() {}

	Vector2		m_position = Vector2(.5f, .5f);
	float		m_radius = .025f;
	bool		m_isSelected = false;
};

//====================================================================================
// Classes
//====================================================================================
class CurveEditor : public EditorMode
{
public:
	CurveEditor();

public:
	virtual void Update() override;
	virtual void Render() const override;
	virtual void HandleInput() override;
	
private:
	void UpdateHover();

private:
	void RenderLinePreview() const;
	void RenderCurveSelector() const;
	void RenderLine() const;
	void RenderGrid() const;
	void RenderUIKnobs() const;

public:
	virtual void Enter() override {}
	virtual void Exit() override {}

private:
	void GenerateCurveData();
	void GenerateBounds();
	void SwitchToCurve(int index);

private:
	void Save();
	void SaveCurveDataToFile();

private:
	int					m_selectedCurve = 0;
	CurveData			m_curves[MAX_AMOUNT_OF_CURVES];

private:
	ControlPointUINode	m_controlPointANode;
	ControlPointUINode	m_controlPointBNode;

	Camera*				m_camera = nullptr;

	float				m_zoomedInAmount = MIN_ZOOMED_IN_LEVEL_CURVE_EDITOR;

private:
	Rgba				m_colorOfCircleOnLine;

	AABB2				m_cameraBounds;
	AABB2				m_linePreviewBounds;

	AABB2				m_splineButtonOverallBounds;
	AABB2				m_splineButtonsBounds[MAX_AMOUNT_OF_CURVES];
};

//====================================================================================
// Standalone C Functions
//====================================================================================
void LoadCurveDataFromFile(CurveData* outArray);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [6/1/2020]
//====================================================================================