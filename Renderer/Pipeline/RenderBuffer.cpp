#include "RenderBuffer.hpp"

//===============================================================================================
RenderBuffer::RenderBuffer()
{
	m_handle = 0;       
	m_bufferSize = 0;
}

RenderBuffer::~RenderBuffer()
{
	if (m_handle != NULL) 
	{
		glDeleteBuffers( 1, &m_handle );											GL_CHECK_ERROR();
		m_handle = NULL; 
	}
}

bool RenderBuffer::CopyToGPU(size_t const byteCount, void const * data)
{
	GL_CHECK_ERROR();

	// handle is a GLuint member - used by OpenGL to identify this buffer
	// if we don't have one, make one when we first need it [lazy instantiation]
	if (m_handle == NULL) {
		glGenBuffers( 1, &m_handle );												GL_CHECK_ERROR(); 
	}

	// Bind the buffer to a slot, and copy memory
	// GL_DYNAMIC_DRAW means the memory is likely going to change a lot (we'll get
	// during the second project)
	glBindBuffer( GL_ARRAY_BUFFER, m_handle );										GL_CHECK_ERROR();
	glBufferData( GL_ARRAY_BUFFER, byteCount, data, GL_DYNAMIC_DRAW );				GL_CHECK_ERROR();

	// buffer_size is a size_t member variable I keep around for 
	// convenience
	m_bufferSize = byteCount; 
	return true; 
}

//===============================================================================================
IndexBuffer::IndexBuffer(uint indexCount, uint indexStride)
{
	m_indexCount = indexCount;
	m_indexStride = indexStride;
}

//===============================================================================================
VertexBuffer::VertexBuffer(uint vertexCount, uint vertexStride)
{
	m_vertexCount = vertexCount;
	m_vertexStride = vertexStride;
}