#include "Playground.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Game/General/Map/Tile.hpp"
#include "Engine/Core/Tools/DebugRendering.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Renderer/Systems/MeshBuilder.hpp"
#include <vector>
#include "Engine/Renderer/Systems/MeshBuilderStrawberry.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Tools/Clock.hpp"


// turn off unused variable warnings
#pragma warning( disable : 4189 )

//===============================================================================================
void Playground::RunTestOnce()
{
}

//-----------------------------------------------------------------------------------------------
void Playground::RunTestOnUpdate()
{
	
}

//-----------------------------------------------------------------------------------------------
void Playground::RenderTest()
{
	Renderer* r = Renderer::GetInstance();

	//r->DrawAABB2Filled(AABB2(-100, -100, 100, 100), Rgba::STRAWBERRY_RED);

	AABB2 boxA = AABB2(.0,.0,.8,.8);
	AABB2 boxB = AABB2(.5, .5, .7, .7);
	AABB2 boxC = AABB2(.55, .4, .8, .6);


	float degree = Interpolate(0.f, 360.f, GetFractionOf(g_theMasterClock->totalTime));
	float oppositeDegree = degree - 180;

	Vector2 start = Vector2(CosDegrees(degree), SinDegrees(degree));
	Vector2 end = Vector2(CosDegrees(oppositeDegree), SinDegrees(oppositeDegree));

	//g_theMeshBuilder.AppendCircleOutline2D(Vector2(0.f, 0.f), 1.f, Rgba::BLUE, .1f);
	//g_theMeshBuilder.AppendAABB2Outline(boxA, Rgba::WHITE, .1f);
	//g_theMeshBuilder.AppendLine(start, end, Rgba::WHITE, .05f);
	
	//r->DrawMesh(g_theMeshBuilder.CreateMesh(true), true);
	//r->DrawAABB2Outline(boxA, Rgba::BLACK);
	//g_theMeshBuilder.AppendCircleFilled2D(Vector2(.5f, .5f), .4f);

	//r->DrawMesh(g_theMeshBuilder.CreateMesh(true), true);
}

