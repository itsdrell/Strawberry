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

#ifndef EMSCRIPTEN_PORT
#include "Engine/Core/Platform/Window.hpp"

//===============================================================================================
// Four needed variables for RenderStartup.
//static HMODULE gGLLibrary  = NULL; 
//HMODULE gGLLibrary  = NULL;				// this is externed in GLFunctions
HMODULE gGLLibrary  = NULL;		// this is externed in GLFunctions
static HWND gGLwnd         = NULL;    // window our context is attached to; 
static HDC gHDC            = NULL;    // our device context
static HGLRC gGLContext    = NULL;    // our rendering context; 

// these need to be declared here for linking!
static HGLRC CreateOldRenderContext( HDC hdc );
static HGLRC CreateRealRenderContext( HDC hdc, int major, int minor );
#endif


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
	GLShutdown();
}

//-----------------------------------------------------------------------------------------------
void Renderer::RenderStartupForWindows(void* hwnd)
{
	
#ifndef EMSCRIPTEN_PORT
	// load and get a handle to the opengl dll (dynamic link library)
	gGLLibrary = ::LoadLibraryA( "opengl32.dll" ); 

	// Get the Device Context (DC) - how windows handles the interface to rendering devices
	// This "acquires" the resource - to cleanup, you must have a ReleaseDC(hwnd, hdc) call. 
	HDC hdc = ::GetDC( (HWND) hwnd );       

	// use the DC to create a rendering context (handle for all OpenGL state - like a pointer)
	// This should be very similar to SD1
	HGLRC temp_context = CreateOldRenderContext( hdc ); 

	::wglMakeCurrent( hdc, temp_context ); 
	BindNewGLFunctions();  // find the functions we'll need to create the real context; 

	// create the real context, using opengl version 4.2
	HGLRC real_context = CreateRealRenderContext( hdc, 4, 2 ); 

	// Set and cleanup
	::wglMakeCurrent( hdc, real_context ); 
	::wglDeleteContext( temp_context ); 

	// Bind all our OpenGL functions we'll be using.
	BindGLFunctions(); 

	// set the globals
	gGLwnd = (HWND) hwnd;
	gHDC = hdc; 
	gGLContext = real_context; 

	m_windowSize = Vector2(Window::GetInstance()->GetWidth(), Window::GetInstance()->GetHeight());

	// Turn off VSync
	wglSwapIntervalEXT( 0 ); 

	RenderPostStartUp();

	//return true; 

#endif
}

//-----------------------------------------------------------------------------------------------
void Renderer::RenderStartupForWeb(const Vector2& windowSize)
{
#ifdef EMSCRIPTEN_PORT

	m_windowSize = windowSize;

	//if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0) 
	//{
	//	m_webScreen = SDL_SetVideoMode(m_windowSize.x, m_windowSize.y, 0, SDL_OPENGL);
	//	if (m_webScreen == NULL)
	//	{
	//		printf("Could not set video mode: %s", SDL_GetError());
	//		return;
	//	}
	//}
	//else 
	//{
	//	printf("Could not initialize SDL: %s", SDL_GetError());
	//	return;
	//}

	RenderPostStartUp();

#endif
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
	glActiveTexture( GL_TEXTURE0 + bindIndex );						GL_CHECK_ERROR();

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

//-----------------------------------------------------------------------------------------------
void* Renderer::GetGLLibrary()
{
#ifdef EMSCRIPTEN_PORT
	return NULL;
#else
	return (void*) gGLLibrary;
#endif
}

//===============================================================================================
// Creates a real context as a specific version (major.minor)
#ifdef EMSCRIPTEN_PORT
#else
static HGLRC CreateRealRenderContext( HDC hdc, int major, int minor ) 
{
	// So similar to creating the temp one - we want to define 
	// the style of surface we want to draw to.  But now, to support
	// extensions, it takes key_value pairs
	int const format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,    // The rc will be used to draw to a window
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,    // ...can be drawn to by GL
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,     // ...is double buffered
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, // ...uses a RGBA texture
		WGL_COLOR_BITS_ARB, 24,             // 24 bits for color (8 bits per channel)
											// WGL_DEPTH_BITS_ARB, 24,          // if you wanted depth a default depth buffer...
											// WGL_STENCIL_BITS_ARB, 8,         // ...you could set these to get a 24/8 Depth/Stencil.
											NULL, NULL,                         // Tell it we're done.
	};

	// Given the above criteria, we're going to search for formats
	// our device supports that give us it.  I'm allowing 128 max returns (which is overkill)
	size_t const MAX_PIXEL_FORMATS = 128;
	int formats[MAX_PIXEL_FORMATS];
	int pixel_format = 0;
	UINT format_count = 0;

	BOOL succeeded = wglChoosePixelFormatARB( hdc, 
		format_attribs, 
		nullptr, 
		MAX_PIXEL_FORMATS, 
		formats, 
		(UINT*)&format_count );

	if (!succeeded) {
		return NULL; 
	}

	// Loop through returned formats, till we find one that works
	for (UINT i = 0; i < format_count; ++i) {
		pixel_format = formats[i];
		succeeded = SetPixelFormat( hdc, pixel_format, NULL ); // same as the temp context; 
		if (succeeded) {
			break;
		} else {
			DWORD error = GetLastError();
			DebuggerPrintf( "Failed to set the format: %u", error ); 
		}
	}

	if (!succeeded) {
		return NULL; 
	}

	// Okay, HDC is setup to the rihgt format, now create our GL context

	// First, options for creating a debug context (potentially slower, but 
	// driver may report more useful errors). 
	int context_flags = 0; 
#if defined(_DEBUG)
	context_flags |= WGL_CONTEXT_DEBUG_BIT_ARB; 
#endif

	// describe the context
	int const attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,                             // Major GL Version
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,                             // Minor GL Version
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,   // Restrict to core (no compatibility)
		WGL_CONTEXT_FLAGS_ARB, context_flags,                             // Misc flags (used for debug above)
		0, 0
	};

	// Try to create context
	HGLRC context = wglCreateContextAttribsARB( hdc, NULL, attribs );
	if (context == NULL) {
		return NULL; 
	}

	return context;
}

//-----------------------------------------------------------------------------------------------
static HGLRC CreateOldRenderContext( HDC hdc ) 
{
	// Setup the output to be able to render how we want
	// (in our case, an RGBA (4 bytes per channel) output that supports OpenGL
	// and is double buffered
	PIXELFORMATDESCRIPTOR pfd;
	memset( &pfd, 0, sizeof(pfd) ); 
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 0; // 24; Depth/Stencil handled by FBO
	pfd.cStencilBits = 0; // 8; DepthStencil handled by FBO
	pfd.iLayerType = PFD_MAIN_PLANE; // ignored now according to MSDN

									 // Find a pixel format that matches our search criteria above. 
	int pixel_format = ::ChoosePixelFormat( hdc, &pfd );
	if ( pixel_format == NULL ) {
		return NULL; 
	}

	// Set our HDC to have this output. 
	if (!::SetPixelFormat( hdc, pixel_format, &pfd )) {
		return NULL; 
	}

	// Create the context for the HDC
	HGLRC context = wglCreateContext( hdc );
	if (context == NULL) {
		return NULL; 
	}

	// return the context; 
	return context; 
}

//-----------------------------------------------------------------------------------------------
void GLShutdown()
{
	wglMakeCurrent( gHDC, NULL ); 

	::wglDeleteContext( gGLContext ); 
	::ReleaseDC( gGLwnd, gHDC ); 

	gGLContext = NULL; 
	gHDC = NULL;
	gGLwnd = NULL; 

	::FreeLibrary( gGLLibrary );
}
#endif