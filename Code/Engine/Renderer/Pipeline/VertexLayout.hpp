#pragma once
#include "Engine/Core/General/EngineCommon.hpp"

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================
enum eRenderDataType;

//====================================================================================
// Structs
//====================================================================================
struct VertexAttributeT
{

public:
	VertexAttributeT() { name = "END";} // null terminator
	VertexAttributeT(const String & theName, eRenderDataType theType, uint theCount, bool isNormalized, size_t theMemberOffset);

public:
	String						name; // because we have to search for it
	eRenderDataType				type; // what? (abstract away GL from the header)
	uint						elem_count; // how many?
	bool						normalized; 
	uint						vertex_stride;
	size_t						member_offset; 
};

//====================================================================================
// Classes
//====================================================================================
class VertexLayout 
{
public:
	VertexLayout(uint stride, const VertexAttributeT data[]);

	uint						GetAttributeCount(); 
	VertexAttributeT&			GetAttribute( int const idx );  

public:
	// what are the members
	std::vector<VertexAttributeT>		m_attributes;

	// how large is the vertex in bytes
	uint m_stride; 
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================