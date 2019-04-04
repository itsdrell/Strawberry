#include "Shader.hpp"

//===============================================================================================
RenderState::RenderState()
{
	m_cullMode = CULLMODE_BACK;			
	m_fillMode = FILLMODE_SOLID;			
	m_frontFace = WIND_COUNTER_CLOCKWISE;		

	m_depthCompare = COMPARE_LESS;		
	m_depthWrite = true;       

	m_colorBlendOp = COMPARE_ADD;	
	m_colorSrcFactor = BLEND_ONE;	
	m_colorDstFactor = BLEND_ZERO;	

	m_alphaBlendOp = COMPARE_ADD;	
	m_alphaSrcFactor = BLEND_ONE;	
	m_alphaDstFactor = BLEND_ONE;
}

//===============================================================================================
Shader::Shader(const String & path)
{
	UNUSED(path);
}

//-----------------------------------------------------------------------------------------------
void Shader::SetProgram(ShaderProgram * theProgram)
{
	m_program = theProgram;
}

//-----------------------------------------------------------------------------------------------
void Shader::EnableBlending(eBlendOperation op, eBlendFactor src, eBlendFactor dst)
{
	m_state.m_colorBlendOp = op;
	m_state.m_colorSrcFactor = src;
	m_state.m_colorDstFactor = dst;
}

//-----------------------------------------------------------------------------------------------
void Shader::DisableBlending()
{
	// Disabled is: 
	// Color: Compare Add  1, zero. 
	// Alpha: Compare ADD, 1, 1

	m_state.m_colorBlendOp = COMPARE_ADD;
	m_state.m_colorSrcFactor = BLEND_ONE;
	m_state.m_colorDstFactor = BLEND_ZERO;

	m_state.m_alphaBlendOp = COMPARE_ADD;
	m_state.m_colorSrcFactor = BLEND_ONE;
	m_state.m_colorDstFactor = BLEND_ONE;
}

//-----------------------------------------------------------------------------------------------
void Shader::SetDepth(DepthBufferComparisons compare, bool write)
{
	m_state.m_depthCompare = compare;
	m_state.m_depthWrite = write;
}

//-----------------------------------------------------------------------------------------------
void Shader::SetCullMode(eCullMode cullMode)
{
	m_state.m_cullMode = cullMode;
}

//-----------------------------------------------------------------------------------------------
void Shader::SetFillMode(eFillMode fillMode)
{
	m_state.m_fillMode = fillMode;
}

void Shader::SetFrontFace(eWindOrder windOrder)
{
	m_state.m_frontFace = windOrder;
}
