#include "FrameBuffer.hpp"
#include "Engine/Renderer/GLFunctions.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Internal/EmscriptenCommon.hpp"

//===============================================================================================
FrameBuffer::FrameBuffer()
{
	GL_CHECK_ERROR();
	glGenFramebuffers( 1, (GLuint*) &m_ID );
	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{
	//glDeleteFramebuffers( 1, (GLuint*) &m_ID ); 
	m_ID = NULL; 
}

//-----------------------------------------------------------------------------------------------
void FrameBuffer::SetColorTarget(Texture * color_target)
{
	m_colorTarget = color_target;
}

//-----------------------------------------------------------------------------------------------
int FrameBuffer::GetDepthStencilTargetWidth()
{
	return m_depthStencilTarget->GetDimensions().x;
}

//-----------------------------------------------------------------------------------------------
int FrameBuffer::GetDepthStencilTargetHeight()
{
	return m_depthStencilTarget->GetDimensions().y;
}

//-----------------------------------------------------------------------------------------------
bool FrameBuffer::Finalize()
{
	GL_CHECK_ERROR_AND_DIE();

	glBindFramebuffer( GL_FRAMEBUFFER, m_ID );						GL_CHECK_ERROR();
	//GL_CHECK_ERROR_AND_DIE();

	// keep track of which outputs go to which attachments; 
	GLenum targets[1]; 

	// Bind a color target to an attachment point
	// and keep track of which locations to to which attachments. 
	//glFramebufferTexture( GL_FRAMEBUFFER, 
	//	GL_COLOR_ATTACHMENT0 + 0, 
	//	m_colorTarget->GetID(), 
	//	0 ); 
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0 + 0,
		GL_TEXTURE_2D,
		m_colorTarget->GetID(),
		0);

	GL_CHECK_ERROR();

	// 0 to to attachment 0
	targets[0] = GL_COLOR_ATTACHMENT0 + 0; 

	// Update target bindings
	//glDrawBuffers( 1, targets );									GL_CHECK_ERROR();

	// Bind depth if available;
	if (m_depthStencilTarget == nullptr) 
	{
		//glFramebufferTexture( GL_FRAMEBUFFER, 
		//	GL_DEPTH_STENCIL_ATTACHMENT, // depth attatchment
		//	NULL, 
		//	0 ); 
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D,
			NULL,
			0);

		GL_CHECK_ERROR();
	} 
	else 
	{
		//glFramebufferTexture( GL_FRAMEBUFFER, 
		//	GL_DEPTH_STENCIL_ATTACHMENT, 
		//	m_depthStencilTarget->GetID(), 
		//	0 ); 
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D,
			m_depthStencilTarget->GetID(),
			0);

		GL_CHECK_ERROR();
	}

	// Error Check - recommend only doing in debug
#if defined(_DEBUG)
	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );					GL_CHECK_ERROR();
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		DebuggerPrintf( "Failed to create framebuffer:  %u", status );
		return false;
	}
#endif

	GL_CHECK_ERROR();
	return true;
}
