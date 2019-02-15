#include "VertexLayout.hpp"

//===============================================================================================
VertexLayout::VertexLayout(uint stride, const VertexAttributeT data[])
{
	m_stride = stride;

	uint idx = 0;
	std::string current = data[idx].name;

	while(current != "END")
	{
		m_attributes.push_back(data[idx]);

		idx++;
		current = data[idx].name;
	}
}

//-----------------------------------------------------------------------------------------------
uint VertexLayout::GetAttributeCount()
{
	return (uint) m_attributes.size();
}

//===============================================================================================
VertexAttributeT & VertexLayout::GetAttribute(int const idx)
{
	return m_attributes[idx];
}

//-----------------------------------------------------------------------------------------------
VertexAttributeT::VertexAttributeT(const String & theName, int theType, uint theCount, bool isNormalized, size_t theMemberOffset)
{
	name = theName;
	type = theType;
	elem_count = theCount;
	normalized = isNormalized;
	member_offset = theMemberOffset;

	vertex_stride; //??????
}
