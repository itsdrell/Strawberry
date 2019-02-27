#include "Renderer.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Renderer/GLFunctions.hpp"
#include "Engine/Renderer/RenderTypes.hpp"
#include "Engine/Core/General/Rgba.hpp"
#include "Engine/Core/General/Camera.hpp"
#include "Engine/Renderer/Components/Shader.hpp"
#include "Engine/Renderer/Components/ShaderProgram.hpp"
#include "Engine/Renderer/Images/Image.hpp"
#include "Engine/Renderer/Images/Texture.hpp"
#include "Engine/Renderer/BuiltInShaders.hpp"
#include "Engine/Renderer/Pipeline/Sampler.hpp"
#include "Engine/Renderer/Pipeline/FrameBuffer.hpp"
#include "Engine/Internal/EmscriptenCommon.hpp"



//-----------------------------------------------------------------------------------------------
int g_openGlPrimitiveTypes[ NUM_PRIMITIVE_TYPES ] =
{

	GL_POINTS,			// called PRIMITIVE_POINTS		in our engine
	GL_LINES,			// called PRIMITIVE_LINES		in our engine
	GL_TRIANGLES,		// called PRIMITIVE_TRIANGES	in our engine
						//GL_POLYGON,			// called PRIMITIVE_POLYGON
						//GL_QUADS			// called PRIMITIVE_QUADS		in our engine
};

//-----------------------------------------------------------------------------------------------
Renderer* Renderer::s_theRenderer = nullptr;

//===============================================================================================
Renderer::Renderer()
{
	s_theRenderer = this;
}

//-----------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
	
}

//-----------------------------------------------------------------------------------------------
void Renderer::RenderStartupForWindows(void* hwnd)
{
	
#ifndef EMSCRIPTEN_PORT


#endif
}

//-----------------------------------------------------------------------------------------------
void Renderer::RenderStartupForWeb(const Vector2& windowSize)
{
	m_windowSize = windowSize;

	BindGLFunctions(); 
	RenderPostStartUp();
}

//-----------------------------------------------------------------------------------------------
void Renderer::RenderPostStartUp()
{
	//BuiltInShaders::CreateAllBuiltInShaders();

	// default_vao is a GLuint member variable
	//glGenVertexArrays( 1, &m_defaultVAO ); 
	//glBindVertexArray( m_defaultVAO );  

	m_immediateBuffer = new RenderBuffer();

	m_defaultSampler = new Sampler();
	m_defaultSampler->CreateDefault();
	m_currentSampler = m_defaultSampler;

	// default white texture
	m_defaultTexture = new Texture(); //CreateOrGetTexture("Data/Images/defaultTexture.png");
	m_defaultTexture =  m_defaultTexture->CreateFromImage(Image("defaultTexture", IntVector2(8,8), Rgba(255, 255, 255, 255)));
	m_currentTexture = m_defaultTexture;

	// default font 
	//m_defaultFont = CreateOrGetBitmapFont("SquirrelFixedFont");

	//m_defaultShader = Shader::CreateOrGetShader("default");
	m_defaultShader = BuiltInShaders::CreateDefaultShader();
	m_currentShader = m_defaultShader;

	// Frame buffer stuff
	// the default color and depth should match our output window
	// so get width/height however you need to.
	int window_width =  m_windowSize.x;
	int window_height = m_windowSize.y;

	// create our output textures
	m_defaultColorTarget = CreateRenderTarget( window_width, 
		window_height );
	m_defaultDepthTarget = CreateRenderTarget( window_width, 
		window_height, 
		TEXTURE_FORMAT_D24S8 ); 

	// setup the initial camera
	m_defaultCamera = new Camera();
	m_defaultCamera->SetColorTarget( m_defaultColorTarget ); 
	//m_defaultCamera->SetDepthStencilTarget( m_defaultDepthTarget ); 

	// set our default camera to be our current camera
	SetCamera(nullptr); 

}

//-----------------------------------------------------------------------------------------------
void Renderer::BeginFrame()
{
	GL_CHECK_ERROR();
	ClearScreen(Rgba(0,0,0,0));
}

//-----------------------------------------------------------------------------------------------
void Renderer::EndFrame()
{
	GL_CHECK_ERROR();
	FrameBuffer temp = m_defaultCamera->GetFramebuffer();
	CopyFrameBuffer(nullptr, &temp);
	//CopyFrameBuffer( nullptr, &m_defaultCamera->GetFramebuffer() ); 

#ifdef EMSCRIPTEN_PORT
#else
	HWND hWnd = GetActiveWindow();
	HDC hDC = GetDC( hWnd );
	SwapBuffers( hDC );
#endif
}

//-----------------------------------------------------------------------------------------------
bool Renderer::CopyFrameBuffer(FrameBuffer* dst, FrameBuffer* src)
{
	GL_CHECK_ERROR();

	// we need at least the src.
	if (src == nullptr) {
		return false; 
	}

	// Get the handles - NULL refers to the "default" or back buffer FBO
	GLuint src_fbo = src->GetID();
	GLuint dst_fbo = NULL; 
	if (dst != nullptr) {
		dst_fbo = dst->GetID(); 
	}

	// can't copy onto ourselves
	if (dst_fbo == src_fbo) {
		return false; 
	}

	// the GL_READ_FRAMEBUFFER is where we copy from
	glBindFramebuffer( GL_READ_FRAMEBUFFER, src_fbo );						GL_CHECK_ERROR();

	// what are we copying to?
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, dst_fbo );						GL_CHECK_ERROR();

	// blit it over - get the size
	// (we'll assume dst matches for now - but to be safe,
	// you should get dst_width and dst_height using either
	// dst or the window depending if dst was nullptr or not
	int width = 0;
	int height = 0;

	if(src->m_depthStencilTarget != nullptr)
	{
		width = src->GetDepthStencilTargetWidth();     
		height = src->GetDepthStencilTargetHeight(); 
	}
	else
	{
		//Window* theWindow = Window::GetInstance();

		// might want to make em floats but w/e
		width = (int)m_windowSize.x;
		height = (int)m_windowSize.y;
	}

	// Copy it over
	//glBlitFramebuffer( 0, 0, // src start pixel
	//	width, height,        // src size
	//	0, 0,                 // dst start pixel
	//	width, height,        // dst size
	//	GL_COLOR_BUFFER_BIT,  // what are we copying (just colour)
	//	GL_NEAREST );         // resize filtering rule (in case src/dst don't match)

							  // Make sure it succeeded
	GL_CHECK_ERROR(); 

	// cleanup after ourselves
	glBindFramebuffer( GL_READ_FRAMEBUFFER, NULL );						GL_CHECK_ERROR();
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, NULL );						GL_CHECK_ERROR();

	return true;
}

//-----------------------------------------------------------------------------------------------
void Renderer::ClearScreen(const Rgba & clearColor)
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetCamera(Camera* theCamera /*= nullptr */)
{
	GL_CHECK_ERROR();

	if (theCamera == nullptr) {
		theCamera = m_defaultCamera; 
	}

	// Forseth had this as camera->Finalize but i think he meant to call the FB finalize
	bool check = theCamera->m_output.Finalize(); // make sure the framebuffer is finished being setup; 

	if(check == false)
		DebuggerPrintf("Framebuffer wasnt finalized");

	m_currentCamera = theCamera;

	BindCameraToShader(*m_currentCamera);
}

//-----------------------------------------------------------------------------------------------
void Renderer::BindCameraToShader(const Camera& theCamera)
{
	GL_CHECK_ERROR();

	// Set the variables so that we can use them in the uniform shader
	m_cameraMatrixData.view = theCamera.m_viewMatrix;
	m_cameraMatrixData.projection = theCamera.m_projectionMatrix;

	//TODO("viewProjection was breaking stuff pls fix");
	m_cameraMatrixData.viewProjection = Matrix44(); 

	// inverses
	//m_cameraMatrixData.inverseView = theCamera.m_cameraMatrix;
	//m_cameraMatrixData.inverseProjection = Invert(m_cameraMatrixData.projection);
	//m_cameraMatrixData.inverseViewProjection = Invert(m_cameraMatrixData.inverseViewProjection);

	m_modelMatrixData.model = theCamera.m_cameraMatrix;

	// bind to the shader
	m_cameraMatrixBuffer.CopyToGPU(sizeof(m_cameraMatrixData), &m_cameraMatrixData);
	//glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_BUFFER_BINDING, m_cameraMatrixBuffer.m_handle);			GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetShader(Shader* shader /*= nullptr */)
{
	GL_CHECK_ERROR();

	// Be sure to that this eventually does call glUseProgram,
	// as all your SetUniform* calls requires it 

	if(nullptr == shader)
	{
		shader = m_defaultShader;
	}

	m_currentShader = shader;

	glUseProgram(m_currentShader->m_program->m_programHandle);					GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetCurrentTexture(int bindIndex /*= 0*/, Texture* texture /*= nullptr*/)
{
	GL_CHECK_ERROR();

	if(texture == nullptr)
	{
		texture = m_defaultTexture;
	}

	m_currentTexture = texture;

	// Bind the texture
	//glActiveTexture( GL_TEXTURE0 + bindIndex );						GL_CHECK_ERROR();

	glBindTexture( GL_TEXTURE_2D, m_currentTexture->GetID() );		GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
Texture* Renderer::CreateRenderTarget(int width, int height, eTextureFormat format)
{
	Texture *tex = new Texture();
	tex->CreateRenderTarget( width, height, format );

	return tex;
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawAABB2(const AABB2 & bounds, const Rgba & color, bool filled)
{
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawMeshImmediate(PrimitiveType primitiveType, Vertex3D_PCU* vertices, int numOfVertices)
{
	GL_CHECK_ERROR();

	// first, copy the memory to the buffer
	m_immediateBuffer->CopyToGPU( sizeof(Vertex3D_PCU) * numOfVertices, vertices ); 

	// Tell GL what shader program to use.
	GLuint program_handle = m_currentShader->m_program->m_programHandle; 


	// Bind the Position
	GLint pos_bind = glGetAttribLocation(program_handle, "POSITION");		GL_CHECK_ERROR();

	// Next, bind the buffer we want to use; 
	glBindBuffer( GL_ARRAY_BUFFER, m_immediateBuffer->m_handle );				GL_CHECK_ERROR();

	// next, bind where position is in our buffer to that location; 
	if (pos_bind >= 0) {
		// enable this location
		glEnableVertexAttribArray(pos_bind);								GL_CHECK_ERROR();

		// describe the data
		glVertexAttribPointer(pos_bind, // where?
			3,                           // how many (vec3 has 3 floats)
			GL_FLOAT,                    // type? (vec3 is 3 floats)
			GL_FALSE,                    // Should data be normalized
			sizeof(Vertex3D_PCU),              // stride (how far between each vertex)
			(GLvoid*)offsetof(Vertex3D_PCU, m_position)); // From the start of a vertex, where is this data?
	}
	GL_CHECK_ERROR();

	// Now that it is described and bound, draw using our program
	glUseProgram( program_handle );											GL_CHECK_ERROR(); 

	// Bind the UV
	GLint uv_bind = glGetAttribLocation(program_handle, "UV");				GL_CHECK_ERROR();

	// Next, bind the buffer we want to use; 
	glBindBuffer( GL_ARRAY_BUFFER, m_immediateBuffer->m_handle );				GL_CHECK_ERROR();

	// next, bind where position is in our buffer to that location; 
	if (uv_bind >= 0) {
		// enable this location
		glEnableVertexAttribArray(uv_bind);									GL_CHECK_ERROR();

		// describe the data
		glVertexAttribPointer(uv_bind, // where?
			2,										// how many (vec2 has 2 floats)
			GL_FLOAT,								// type? (vec2 is 2 floats)
			GL_FALSE,								// Should data be normalized
			sizeof(Vertex3D_PCU),					// stride (how far between each vertex)
			(GLvoid*)offsetof(Vertex3D_PCU, m_uvTexCoords)); // From the start of a vertex, where is this data?
	}
	GL_CHECK_ERROR();

	// binding frame buffer
	glBindFramebuffer( GL_FRAMEBUFFER, 
		m_currentCamera->GetFrameBufferID() );								GL_CHECK_ERROR();

	// Now that it is described and bound, draw using our program
	glUseProgram( program_handle ); 										GL_CHECK_ERROR();

	// Next, bind the buffer we want to use;
	glBindBuffer( GL_ARRAY_BUFFER, m_immediateBuffer->m_handle );				GL_CHECK_ERROR();

	// next, bind where position is in our buffer to that location;
	GLint bind = glGetAttribLocation(program_handle, "COLOR");				GL_CHECK_ERROR();
	if (bind >= 0) {
		// enable this location
		glEnableVertexAttribArray(bind);									GL_CHECK_ERROR();

		// describe the data
		glVertexAttribPointer(bind, // where?
			4,                           // how many (RGBA is 4 unsigned chars)
			GL_UNSIGNED_BYTE,            // type? (RGBA is 4 unsigned chars)
			GL_TRUE,                     // Normalize components, maps 0-255 to 0-1.
			sizeof(Vertex3D_PCU),              // stride (how far between each vertex)
			(GLvoid*)offsetof(Vertex3D_PCU, m_color)); // From the start of a vertex, where is this data?
	}
	GL_CHECK_ERROR();


	GLenum glPrimitiveType = g_openGlPrimitiveTypes[ primitiveType ];
	glDrawArrays(glPrimitiveType, 0, numOfVertices );							GL_CHECK_ERROR();

}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawMeshImmediate(PrimitiveType thePrimitive, uint vertexCount, Vertex3D_PCU * vertices, uint indicesCount, uint * indices)
{
}

//-----------------------------------------------------------------------------------------------
Renderer * Renderer::GetInstance()
{
	return s_theRenderer;
}
