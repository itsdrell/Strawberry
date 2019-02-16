#pragma once
#include <string>
#include <vector>

//====================================================================================
// Forward Declare
//====================================================================================

//===============================================================================================
// Defines
//===============================================================================================
#define UNUSED(x) (void)(x);

//====================================================================================
// Type Defs 
//====================================================================================
typedef std::string					String;
typedef std::vector<std::string>	Strings;
typedef std::vector<int>			Ints;
typedef unsigned int				uint;
typedef unsigned short				uint16;
typedef unsigned char				Byte;

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================


//====================================================================================
// Standalone C Functions
//====================================================================================
void EngineStartUp();
void EngineShutdown();

//===============================================================================================
// Templates
//===============================================================================================
template< typename T>
void RemoveFast( uint& idx, std::vector<T>& theVector )
{
	T endThing = theVector.at(theVector.size() - 1);

	if(theVector.size() == 1)
	{
		theVector.pop_back();
		idx--;
		return;
	}

	theVector.at(theVector.size() - 1) = theVector.at(idx);
	theVector.at(idx) = endThing;
	theVector.pop_back();
	idx--; // so we don't have to do it outside of the function
}


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================