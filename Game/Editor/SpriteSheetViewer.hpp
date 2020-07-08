#pragma once
#include "Game/Editor/EditorMode.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include <vector>

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================
enum SpriteSheetViewMode
{
	SPRITE_VIEW_MODE,
	COLOR_VIEW_MODE,
	NUM_OF_VIEW_MODES
};

//====================================================================================
// Structs
//====================================================================================
struct DisplayColor 
{
	DisplayColor(const String& theName, const Rgba& color)
		: m_name(theName), m_color(color) {} 

	String	m_name;
	Rgba m_color;
};

//====================================================================================
// Classes
//====================================================================================
class SpriteSheetView : public EditorMode
{
public:
	SpriteSheetView();
	~SpriteSheetView() {}

public:
	virtual void Update() override;
	virtual void Render() const override;

	virtual void Enter() override;
	virtual void Exit() override {}

private:
	virtual void HandleInput() override;
	void CalculateSpritePositions();
	void CreateDisplayColors();

private:
	void RenderTexture() const;
	void RenderCursor() const;
	void RenderGrid() const;
	void RenderSpriteSheetButtons() const;

private:
	void RenderColors() const;

private:
	void RenderIndex() const;
	void RenderSwitchButtons() const;

private:
	AABB2				m_cameraBounds;
	AABB2				m_textureBounds;

	AABB2				m_switchBoxBounds;
	AABB2				m_spriteButtonBounds;
	AABB2				m_colorButtonBounds;

	AABB2				m_spriteSheetButtonBounds[MAX_AMOUNT_OF_SPRITE_SHEETS];

private:
	IntVector2			m_spriteCoords;
	int					m_spriteIndex = 0;

	uint				m_currentSpriteSheet = 0;

	bool				m_showGrid = true;

private:
	std::vector<DisplayColor>	m_displayColors;
	int							m_colorIndex = 0;
	int							m_maxAmountOfColors = 4;

	SpriteSheetViewMode			m_currentMode = SPRITE_VIEW_MODE;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/15/2019]
//====================================================================================