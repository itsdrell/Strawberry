#include "Playground.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Game/General/Map/Tile.hpp"


// turn off unused variable warnings
#pragma warning( disable : 4189 )

//===============================================================================================
void Playground::RunTestOnce()
{
	//Command newCommand = Command("help idk how to help you_duh");
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

