#pragma once
#include <string>
#include <vector>

//====================================================================================
// Forward Declare
//====================================================================================
class SpriteSheet;

//===============================================================================================
// Const expr
//===============================================================================================
constexpr float SPRITE_DIMENSION = 16.f;
constexpr int MAX_AMOUNT_OF_SPRITE_SHEETS = 4; // if you change this, you have to edit the Shader 

//===============================================================================================
// Defines
//===============================================================================================
#define UNUSED(x) (void)(x);
#define STATIC
#define EPSILON (FLT_EPSILON)
#define UINFINITY ((float)INT_MAX);

//-----------------------------------------------------------------------------------------------
// Source from http://www.flipcode.com/archives/FIXME_TODO_Notes_As_Warnings_In_Compiler_Output.shtml
#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define PRAGMA(p)  __pragma( p )
#define NOTE( x )  PRAGMA( message(x) )
#define FILE_LINE  NOTE( __FILE__LINE__ )

// THE IMPORANT BITS
#define TODO( x )  NOTE( __FILE__LINE__"\n"           \
        " --------------------------------------------------------------------------------------\n" \
        "|  TODO :   " ##x "\n" \
        " --------------------------------------------------------------------------------------\n" )

// Unimplemented (Todo comment on steroids)
#define UNIMPLEMENTED()  TODO( "IMPLEMENT: " QUOTE(__FILE__) " (" QUOTE(__LINE__) ")" ); ASSERT_RECOVERABLE(1,"")


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
extern String				g_currentProjectName;

extern SpriteSheet*			g_theSpriteSheet;
extern SpriteSheet*			g_allSpriteSheets[MAX_AMOUNT_OF_SPRITE_SHEETS];

//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================
