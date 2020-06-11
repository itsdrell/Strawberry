#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Game/States/AppState.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Core/Tools/StopWatch.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class CreateProjectPage;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct ProjectData
{
public:
	ProjectData(const String name, bool hasCover) 
	: m_name(name)
	, m_hasCover(hasCover) {}

public:
	String	m_name = "idk man";
	bool	m_hasCover = false;
};

//====================================================================================
// Classes
//====================================================================================
class Home : IAppState
{
public:
	Home();
	~Home();

public:
	virtual void Update() override;
	virtual void Render() const override;
	
private:
	void RenderUI() const;

public:
	virtual void OnEnter();
	virtual void OnExit();

private:
	void GenerateBounds();
	void GenerateAllProjectData();

private:
	void HandleInput();
	void HandleMouseClicks();

public:
	void ClosePopup();

private:
	uint m_currentIndex = 0;
	std::vector<ProjectData> m_allProjectsData;

private:
	CreateProjectPage*		m_createPagePopup = nullptr;

private:
	AABB2		m_coverImageBounds;
	AABB2		m_gameTitleBounds;
	AABB2		m_loadGameBounds;
	AABB2		m_leftArrowBounds;
	AABB2		m_rightArrowBounds;

	Rgba		m_randomBackgroundColor;

	StopWatch	m_fadeAwayTimer;
	float		m_fadeAwayLength = .5f;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [5/20/2020]
//====================================================================================