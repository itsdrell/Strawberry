#pragma once
#include "Game/Editor/EditorMode.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Core/General/Rgba.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class MapEditor;
class Mesh;
class MeshBuilder;

//====================================================================================
// Type Defs + Defines
//====================================================================================
constexpr uint MAX_AMOUNT_OF_COLLISION_CHANNELS = 5;

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================
struct CollisionChannelButton
{
public:
	CollisionChannelButton() {}
	CollisionChannelButton(const String& name, const AABB2& textBox, const AABB2& buttonBounds)
		: m_nameOfChannel(name)
		, m_textBounds(textBox)
		, m_buttonBounds(buttonBounds) {}

public:
	bool IsMousePositionOnButton(const Vector2& mousePos);
	bool OnLeftClick(const Vector2& mousePos);
	void UpdateBounds(const AABB2& textBox, const AABB2& buttonBounds);


public:
	void Render() const;

public:
	String	m_nameOfChannel = "idk";
	bool	m_isOn = false;

private: 
	AABB2 m_textBounds;
	AABB2 m_buttonBounds;
};

//====================================================================================
// Classes
//====================================================================================
class CollisionEditor : public EditorMode
{
public:
	CollisionEditor( MapEditor* theMapEditor);
	~CollisionEditor();

public:
	virtual void Update() override;
	virtual void Render() const override;
	virtual void HandleInput() override;
	virtual void Enter() override;
	virtual void Exit() override {}

private:
	void GenerateAllBounds();
	void GenerateCurrentCollisionChannelMesh();
	void UpdateCollisionMesh(int index);
	void UpdateNames();
	void LeftClick();

public:
	MapEditor*			m_mapEditor;

	CollisionChannelButton	m_channelButtons[MAX_AMOUNT_OF_COLLISION_CHANNELS] = { CollisionChannelButton() };

private:
	AABB2			m_channelSelectBounds;
	Byte			m_channelBits = 0;
	Rgba			m_tileColor = Rgba(255, 0, 255, 100);

	MeshBuilder*	m_meshBuilder;
	Mesh*			m_mesh;

};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [9/29/2019]
//====================================================================================