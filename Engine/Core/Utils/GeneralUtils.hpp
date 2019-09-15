#pragma once
#include <map>

//====================================================================================
// Forward Declare
//====================================================================================
//template <typename S, typename T>
//void DeleteAndClearMap(std::map<S, T>& themap);


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Templates
//====================================================================================
// emscripten threw errors for this :(
//template <typename S, typename T>
//void DeleteAndClearMap(std::map<S, T>& themap)
//{
//	std::map<typename S, typename T>::iterator theIterator;
//
//	for (theIterator = themap.begin(); theIterator != themap.end(); theIterator++)
//	{
//		delete theIterator->second;
//		theIterator->second = nullptr;
//	}
//
//	themap.clear();
//}


//====================================================================================
// Written by Zachary Bracken : [4/3/2019]
//====================================================================================