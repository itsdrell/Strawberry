#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

//===============================================================================================
//										NOTE:
// These are helpers for strawberry, not generic file utils
// Ex. Trying to get all loaded project names doesn't belong in File.hpp, so i made a seperate
// utils for these kind of things 
// 
// I also put them in engine, because engine classes might use them as well :l 
//===============================================================================================


//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// Standalone C Functions
//====================================================================================
void GetAllProjectNames(Strings* out);
bool DoesProjectHaveCoverImage(const String& projectName);

//====================================================================================
// Written by Zachary Bracken : [5/24/2020]
//====================================================================================