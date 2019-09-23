#include "Playground.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"


// turn off unused variable warnings
#pragma warning( disable : 4189 )

//===============================================================================================
void Playground::RunTestOnce()
{
	//Command newCommand = Command("help idk how to help you_duh");
	AABB2  test = AABB2(0, 0, 3, 6);
	test.ShrinkToSquare();

	AABB2  test1 = AABB2(0, 0, 6, 3);
	test1.ShrinkToSquare();

}

//-----------------------------------------------------------------------------------------------
void Playground::RunTestOnUpdate()
{
}

//-----------------------------------------------------------------------------------------------
void Playground::RenderTest()
{
	Renderer* r = Renderer::GetInstance();
}

