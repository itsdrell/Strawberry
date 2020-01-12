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


//====================================================================================
// Structs
//====================================================================================
struct VertexAttributeT
{

public:
	VertexAttributeT() { name = "END";} // null terminator
	VertexAttributeT(const String & theName, int theType, uint theCount, bool isNormalized, size_t theMemberOffset);

public:
	String						name; // because we have to search for it
	int							type; // had to do this for web to work. Enum eRenderDataType
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
	~VertexLayout();

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