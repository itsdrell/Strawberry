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
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/Math/Geometry/AABB2.hpp"
#include "Engine/ThirdParty/SDL2/SDL.h"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Images/Sprite.hpp"
#include "Engine/Renderer/Images/SpriteSheet.hpp"
#include "Engine/Renderer/Images/BitmapFont.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"



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

	RenderStartup();
}

//-----------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
	Sprite::DeleteSprites();
	//SDL_GL_DeleteContext(m_glContext);
}

//-----------------------------------------------------------------------------------------------
void Renderer::RenderStartup()
{	
	// https://wiki.libsdl.org/SDL_GL_SetAttribute
	if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES) == -1)
		printf("Error with attribute \n");
	
	if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2) == -1) // sets the max version (need this for render doc)
		printf("Error with attribute \n");

	if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0) == -1) // set mins
		printf("Error with attribute \n");

	if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == -1)
		printf("Error with attribute \n");

	if(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)== -1)
		printf("Error with attribute \n");

	//result = SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 0);

	// this doesn't work on the web for some reason? :l
	//0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for adaptive vsync;
	//if(SDL_GL_SetSwapInterval(-1) == -1)
	//	printf("Error swap interval \n");

	Window* theWindow = Window::GetInstance();

	// this is important and has to be after all the attributes
	SDL_GLContext theContext = SDL_GL_CreateContext(theWindow->GetWindowReference());
	if(!theContext)
		printf("Error creating context \n");

	//SDL_GL_MakeCurrent(theWindow->GetWindowReference(),theContext);
	
	m_windowSize = theWindow->GetDimensions();

	// if you create the SDL_Renderer it will break renderdoc, so don't do it unless needed
	//m_theSDLRenderer = SDL_CreateRenderer(theWindow->GetWindowReference(), -1, SDL_RENDERER_ACCELERATED);
	
	BindGLFunctions(); 
	RenderPostStartUp();

	printf("renderer finished startup \n");
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
	int window_width =  (int) m_windowSize.x;
	int window_height = (int) m_windowSize.y;

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

	m_defaultUICamera = new Camera();
	m_defaultUICamera->SetColorTarget( m_defaultColorTarget );
	m_defaultUICamera->SetProjectionOrthoByAspect(1.f);
	m_defaultUICamera->SetProjectionOrtho2D(Vector2(0.f, 0.f), Vector2(Window::GetInstance()->GetAspect(), 1.f));
	//m_defaultUICamera->m_projectionMatrix = Matrix44::MakeOrtho2D(Vector2(0.f, 0.f), Vector2(Window::GetInstance()->GetAspect(), 1));

	// set our default camera to be our current camera
	SetCamera(nullptr); 

	// Hard setting the width of the lines we use to be a certain size
	SetLineWidth(2.f);

	m_defaultDrawColor = Rgba(0, 0, 0, 255);
	m_clearScreenColor = m_defaultDrawColor;

	//m_defaultFont = CreateOrGetBitmapFont("Data/StrawberryFont.png");
	m_defaultFont = CreateOrGetBitmapFont("Data/font.png");

}

//-----------------------------------------------------------------------------------------------
void Renderer::BeginFrame()
{
	GL_CHECK_ERROR();

	// this fixes colors to not be weird in openGL ES default framebuffer
	// if we had our own we could just use the shader and it would be fine 
	// but nooooooooooo we gotta do this cause we are't using our own
	glDisable(GL_FRAMEBUFFER_SRGB);

	if (m_clearScreen)
	{
		ClearScreen(m_clearScreenColor);
		m_clearScreen = false;
	}
}

//-----------------------------------------------------------------------------------------------
void Renderer::EndFrame()
{
	GL_CHECK_ERROR();
	// emscripten gets mad if you combine these
	//FrameBuffer temp = m_defaultCamera->GetFramebuffer();
	//CopyFrameBuffer( nullptr, &temp ); 

	SDL_Window* theWindow = Window::GetWindowReference();
	SDL_GL_SwapWindow(theWindow);

	//ClearScreen(Rgba(0,0, 0, 255));
}

//-----------------------------------------------------------------------------------------------
bool Renderer::CopyFrameBuffer(FrameBuffer* dst, FrameBuffer* src)
{
	GL_CHECK_ERROR();

	// we need at least the src.
	//if (src == nullptr) {
	//	return false; 
	//}
	//
	//// Get the handles - NULL refers to the "default" or back buffer FBO
	//GLuint src_fbo = src->GetID();
	//GLuint dst_fbo = NULL; 
	//if (dst != nullptr) {
	//	dst_fbo = dst->GetID(); 
	//}
	//
	//// can't copy onto ourselves
	//if (dst_fbo == src_fbo) {
	//	return false; 
	//}
	
	// the GL_READ_FRAMEBUFFER is where we copy from
	//glBindFramebuffer( GL_READ_FRAMEBUFFER, src_fbo );						GL_CHECK_ERROR();
	
	// what are we copying to?
	//glBindFramebuffer( GL_DRAW_FRAMEBUFFER, dst_fbo );						GL_CHECK_ERROR();
	
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
	
		width =	(int) Window::GetInstance()->GetWidth();
		height = (int)Window::GetInstance()->GetHeight();
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
	//glBindFramebuffer( GL_READ_FRAMEBUFFER, NULL );						GL_CHECK_ERROR();
	//glBindFramebuffer( GL_DRAW_FRAMEBUFFER, NULL );						GL_CHECK_ERROR();

	// set simple ortho (draw in 2d)
	//glOrtho(0.f, 1.f, 0.f, 1.f, 0.f, 10.f);
	//SetCamera(m_defaultUICamera);

	// bind color target texture
	//float theWidth = width * .5f;
	//float theHeight = height * .5f;

	// draw quad window size
	//DrawTexturedAABB2(
	//	AABB2(-theWidth,-theHeight, theWidth, theHeight), 
	//	*src->m_colorTarget, 
	//	Vector2(0.f, 0.f), 
	//	Vector2(1.f, 1.f), 
	//	Rgba(255,255,255,255));

	AABB2 bounds = AABB2(0.f, 0.f, 1.f, 1.f);
	Vector2 texCoordsAtMins = Vector2(0.f, 0.f);
	Vector2 texCoordsAtMaxs = Vector2(1.f, 1.f);
	Rgba tint = Rgba(255,255,255,255);

	glEnable(GL_BLEND);												GL_CHECK_ERROR();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				GL_CHECK_ERROR();

	m_currentTexture = (Texture*) m_defaultCamera->GetFramebuffer().m_colorTarget;
	SetCurrentTexture(0, m_currentTexture);

	Vertex3D_PCU vertices [6];

	vertices[0] = Vertex3D_PCU(Vector3(bounds.mins.x,bounds.mins.y, 0.f),tint,Vector2(texCoordsAtMins.x,texCoordsAtMins.y)); // bottom left
	vertices[1] = Vertex3D_PCU(Vector3(bounds.maxs.x,bounds.mins.y, 0.f),tint,Vector2(texCoordsAtMaxs.x,texCoordsAtMins.y)); // bottom right
	vertices[2] = Vertex3D_PCU(Vector3(bounds.maxs.x,bounds.maxs.y, 0.f),tint,Vector2(texCoordsAtMaxs.x,texCoordsAtMaxs.y)); // top right

	vertices[3] = Vertex3D_PCU(Vector3(bounds.mins.x,bounds.mins.y, 0.f),tint,Vector2(texCoordsAtMins.x,texCoordsAtMins.y)); // bottom left
	vertices[4] = Vertex3D_PCU(Vector3(bounds.maxs.x,bounds.maxs.y, 0.f),tint,Vector2(texCoordsAtMaxs.x,texCoordsAtMaxs.y)); // top right
	vertices[5] = Vertex3D_PCU(Vector3(bounds.mins.x,bounds.maxs.y, 0.f),tint,Vector2(texCoordsAtMins.x,texCoordsAtMaxs.y)); // top left

	DrawMeshImmediateWithoutFramebuffer(PRIMITIVE_TRIANGLES, vertices, 6);
	

	return true;
}

//-----------------------------------------------------------------------------------------------
void Renderer::ClearScreen(const Rgba & clearColor)
{
	Vector4 normalizedFloatColor = clearColor.GetAsNormalizedVector4();
	glClearColor(normalizedFloatColor.x, normalizedFloatColor.y, normalizedFloatColor.z, normalizedFloatColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

//-----------------------------------------------------------------------------------------------
void Renderer::EnableWireframe(bool check)
{
	//if (check)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetLineWidth(float width)
{
	glLineWidth(width);
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
void Renderer::SetDefaultDrawColor(const Rgba& theColor)
{
	m_defaultDrawColor = theColor;
}

//-----------------------------------------------------------------------------------------------
void Renderer::BindCameraToShader(const Camera& theCamera)
{
	GL_CHECK_ERROR();

	SetUniform("VIEW", theCamera.m_viewMatrix);
	SetUniform("PROJECTION", theCamera.m_projectionMatrix);
	SetUniform("VIEW_PROJECTION", Matrix44());
	SetUniform("MODEL", theCamera.m_cameraMatrix);
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
void Renderer::SetCurrentTexture(int bindIndex /*= 0*/, const Texture* texture /*= nullptr*/)
{
	GL_CHECK_ERROR();

	if(texture == nullptr)
	{
		texture = m_defaultTexture;
	}

	m_currentTexture = (Texture*) texture;

	// Bind the texture
#ifdef EMSCRIPTEN_PORT
	glActiveTexture(GL_TEXTURE0 + bindIndex);
#else
	glActiveTexturePls( GL_TEXTURE0 + bindIndex );						GL_CHECK_ERROR();
#endif

	glBindTexture( GL_TEXTURE_2D, m_currentTexture->GetID() );		GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetUniform(const String& name, const Matrix44& uniform)
{
	GL_CHECK_ERROR();

	glUseProgram(m_currentShader->m_program->m_programHandle ); // but very redundant O WELL

	// https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes
	float asArray[16]; 
	uniform.GetValuesAsArray(asArray);

	int bind_idx = glGetUniformLocation( m_currentShader->m_program->m_programHandle, name.c_str() ); 
	if (bind_idx >= 0) {
		glUniformMatrix4fv( bind_idx, 1, GL_FALSE, asArray );
	}

	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetUniform(const String & name, float uniform)
{
	GL_CHECK_ERROR();

	// but very redundant O WELL
	glUseProgram(m_currentShader->m_program->m_programHandle );											GL_CHECK_ERROR();

	int bind_idx = glGetUniformLocation( m_currentShader->m_program->m_programHandle, name.c_str() );	GL_CHECK_ERROR();
	if (bind_idx >= 0) {
		glUniform1fv( bind_idx, 1, &uniform );															GL_CHECK_ERROR();
	}


	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetUniform(const String & name, const Vector3 & uniform)
{
	GL_CHECK_ERROR();

	glUseProgram(m_currentShader->m_program->m_programHandle ); // but very redundant O WELL

															   // https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes
	float value[3] 
	{
		uniform.x,
		uniform.y,
		uniform.z
	};

	int bind_idx = glGetUniformLocation( m_currentShader->m_program->m_programHandle, name.c_str() ); 
	if (bind_idx >= 0) {
		glUniform3fv( bind_idx, 1, value );
	}

	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetUniform(const String & name, const Vector4 & uniform)
{
	GL_CHECK_ERROR();

	glUseProgram(m_currentShader->m_program->m_programHandle ); // but very redundant O WELL

															   // https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes
	float value[4] 
	{
		uniform.x,
		uniform.y,
		uniform.z,
		uniform.w
	};

	int bind_idx = glGetUniformLocation( m_currentShader->m_program->m_programHandle, name.c_str() ); 
	if (bind_idx >= 0) {
		glUniform4fv( bind_idx, 1, value );
	}

	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
void Renderer::SetUniform(const String& name, const Rgba& uniform)
{
	GL_CHECK_ERROR();

	glUseProgram(m_currentShader->m_program->m_programHandle ); // but very redundant O WELL

															   // https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes
	Vector4 asFloat = uniform.GetAsNormalizedVector4();
	float value[4] 
	{
		asFloat.x,
		asFloat.y,
		asFloat.z,
		asFloat.w
	};

	int bind_idx = glGetUniformLocation( m_currentShader->m_program->m_programHandle, name.c_str() ); 
	if (bind_idx >= 0) {
		glUniform4fv( bind_idx, 1, value );
	}

	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawLine2D(const Vector2& start, const Vector2& end, const Rgba& color /*= Rgba(0, 0, 0, 255)*/)
{
	SetCurrentTexture(0, nullptr);

	// Create the array
	Vertex3D_PCU vertices[2];

	// Create the index
	vertices[0] = Vertex3D_PCU(start, color, Vector2(0.f, 0.f));
	vertices[1] = Vertex3D_PCU(end, color, Vector2(0.f, 0.f));

	DrawMeshImmediate(PRIMITIVE_LINES, vertices, 2);
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawLines2D(Vector2* arrayPointer, int numberOfSides, const Rgba& theColor)
{
	std::vector<Vertex3D_PCU> points;
	for (int i = 0; i < numberOfSides; i++)
	{
		Vertex3D_PCU current = Vertex3D_PCU(arrayPointer[i], theColor, Vector2(0.f, 0.f));
		points.push_back(current); 
	}

	DrawMeshImmediate(PRIMITIVE_LINES, points.data(), numberOfSides);
}

//-----------------------------------------------------------------------------------------------
Texture* Renderer::CreateRenderTarget(int width, int height, eTextureFormat format)
{
	Texture *tex = new Texture();
	tex->CreateRenderTarget( width, height, format );

	return tex;
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawAABB2Outline(const AABB2 & bounds, const Rgba & color)
{
	glEnable(GL_BLEND);												GL_CHECK_ERROR();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				GL_CHECK_ERROR();

	SetCurrentTexture(0, m_defaultTexture);

	Vertex3D_PCU vertices[8];

	vertices[0] = Vertex3D_PCU(Vector3(bounds.mins.x, bounds.mins.y, 0.f), color, Vector2(0.f, 0.f)); // bottom left
	vertices[1] = Vertex3D_PCU(Vector3(bounds.maxs.x, bounds.mins.y, 0.f), color, Vector2(1.f, 0.f)); // bottom right

	vertices[2] = Vertex3D_PCU(Vector3(bounds.maxs.x, bounds.mins.y, 0.f), color, Vector2(1.f, 0.f)); // bottom right
	vertices[3] = Vertex3D_PCU(Vector3(bounds.maxs.x, bounds.maxs.y, 0.f), color, Vector2(1.f, 1.f)); // top right

	vertices[4] = Vertex3D_PCU(Vector3(bounds.maxs.x, bounds.maxs.y, 0.f), color, Vector2(1.f, 1.f)); // top right
	vertices[5] = Vertex3D_PCU(Vector3(bounds.mins.x, bounds.maxs.y, 0.f), color, Vector2(0.0f, 0.f)); // bottom left

	vertices[6] = Vertex3D_PCU(Vector3(bounds.mins.x, bounds.maxs.y, 0.f), color, Vector2(0.0f, 0.f)); // bottom left
	vertices[7] = Vertex3D_PCU(Vector3(bounds.mins.x, bounds.mins.y, 0.f), color, Vector2(1.0f, 1.f)); // top right

	DrawMeshImmediate(PRIMITIVE_LINES, vertices, 8);
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawAABB2Filled(const AABB2& bounds, const Rgba& color /*= Rgba(255, 255, 255, 255)*/)
{
	glEnable(GL_BLEND);												GL_CHECK_ERROR();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				GL_CHECK_ERROR();

	SetCurrentTexture(0, m_defaultTexture);

	Vertex3D_PCU vertices[6];

	vertices[0] = Vertex3D_PCU(Vector3(bounds.mins.x, bounds.mins.y, 0.f), color, Vector2(0.f, 0.f)); // bottom left
	vertices[1] = Vertex3D_PCU(Vector3(bounds.maxs.x, bounds.mins.y, 0.f), color, Vector2(1.f, 0.f)); // bottom right
	vertices[2] = Vertex3D_PCU(Vector3(bounds.maxs.x, bounds.maxs.y, 0.f), color, Vector2(1.f, 1.f)); // top right

	vertices[3] = Vertex3D_PCU(Vector3(bounds.mins.x, bounds.mins.y, 0.f), color, Vector2(0.0f, 0.f)); // bottom left
	vertices[4] = Vertex3D_PCU(Vector3(bounds.maxs.x, bounds.maxs.y, 0.f), color, Vector2(1.0f, 1.f)); // top right
	vertices[5] = Vertex3D_PCU(Vector3(bounds.mins.x, bounds.maxs.y, 0.f), color, Vector2(0.0f, 1.f)); // top left

	DrawMeshImmediate(PRIMITIVE_TRIANGLES, vertices, 6);
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawTexturedAABB2(const AABB2& bounds, const Texture& texture, const Vector2& texCoordsAtMins, const Vector2& texCoordsAtMaxs, const Rgba& tint)
{
	GL_CHECK_ERROR();

	// THIS IS IMPORTANT
	glEnable(GL_BLEND);												GL_CHECK_ERROR();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				GL_CHECK_ERROR();

	m_currentTexture = (Texture*) &texture;
	SetCurrentTexture(0, m_currentTexture);

	Vertex3D_PCU vertices [6];

	vertices[0] = Vertex3D_PCU(Vector3(bounds.mins.x,bounds.mins.y, 0.f),tint,Vector2(texCoordsAtMins.x,texCoordsAtMins.y)); // bottom left
	vertices[1] = Vertex3D_PCU(Vector3(bounds.maxs.x,bounds.mins.y, 0.f),tint,Vector2(texCoordsAtMaxs.x,texCoordsAtMins.y)); // bottom right
	vertices[2] = Vertex3D_PCU(Vector3(bounds.maxs.x,bounds.maxs.y, 0.f),tint,Vector2(texCoordsAtMaxs.x,texCoordsAtMaxs.y)); // top right
									 							
	vertices[3] = Vertex3D_PCU(Vector3(bounds.mins.x,bounds.mins.y, 0.f),tint,Vector2(texCoordsAtMins.x,texCoordsAtMins.y)); // bottom left
	vertices[4] = Vertex3D_PCU(Vector3(bounds.maxs.x,bounds.maxs.y, 0.f),tint,Vector2(texCoordsAtMaxs.x,texCoordsAtMaxs.y)); // top right
	vertices[5] = Vertex3D_PCU(Vector3(bounds.mins.x,bounds.maxs.y, 0.f),tint,Vector2(texCoordsAtMins.x,texCoordsAtMaxs.y)); // top left

	DrawMeshImmediate(PRIMITIVE_TRIANGLES, vertices, 6);
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawSpriteRotated2D(const Vector3& position, const Sprite& theSprite, 
	float rotation /*= Vector2(0.f, 0.f)*/, bool flipX, bool flipY)
{
	Matrix44 theRotationMatrix = Matrix44::MakeRotationDegrees2D(rotation);
	Vector3 right = theRotationMatrix.GetRight();
	Vector3 up = theRotationMatrix.GetUp();

	DrawSprite(position, theSprite, flipX, flipY, right, up);
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawSprite(const Vector3& position, const Sprite& theSprite, bool flipX, bool flipY,
	const Vector3& right /*= Vector3::RIGHT*/, const Vector3& up /*= Vector3::UP*/)
{
	//m_currentTexture = theSprite.m_texture;
	SetCurrentTexture(0, theSprite.m_texture);

	// calculating offsets from the pivot point
	float leftOffset = -1.0f * ((theSprite.m_pivot.x) * theSprite.m_dimensions.x);
	float rightOffset = leftOffset + theSprite.m_dimensions.x;
	float bottomOffset = -1.0f * ((theSprite.m_pivot.y) * theSprite.m_dimensions.y);
	float topOffset = bottomOffset + theSprite.m_dimensions.y;

	// calculating the vertex points 
	Vector3 p0 = position + (right * leftOffset) +  (up * bottomOffset); // bottom left
	Vector3 p1 = position + (right * rightOffset) + (up * bottomOffset); // bottom right
	Vector3 p2 = position + (right * rightOffset) + (up * topOffset); // top right
	Vector3 p3 = position + (right * leftOffset) +  (up * topOffset); // top left
	
	// Get the UVs
	AABB2 uvs = theSprite.m_uvs;

	if (flipY)
	{
		uvs.mins.y = theSprite.m_uvs.maxs.y;
		uvs.maxs.y = theSprite.m_uvs.mins.y;
	}

	if (flipX)
	{
		uvs.mins.x = theSprite.m_uvs.maxs.x;
		uvs.maxs.x = theSprite.m_uvs.mins.x;
	}

	Vector2 bl = uvs.mins;
	Vector2 br = Vector2(uvs.maxs.x, uvs.mins.y);
	Vector2 tl = Vector2(uvs.mins.x, uvs.maxs.y);
	Vector2 tr = uvs.maxs;

	Vertex3D_PCU vertices[] =
	{
		Vertex3D_PCU(p0, Rgba::WHITE, bl),
		Vertex3D_PCU(p1, Rgba::WHITE, br),
		Vertex3D_PCU(p2, Rgba::WHITE, tr),
							 
		Vertex3D_PCU(p0, Rgba::WHITE, bl),
		Vertex3D_PCU(p2, Rgba::WHITE, tr),
		Vertex3D_PCU(p3, Rgba::WHITE, tl)
	};

	DrawMeshImmediate(PRIMITIVE_TRIANGLES, vertices, 6);
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawText2D(const Vector2& startPos, const String& text, float cellHeight, const Rgba& tint /*= Rgba::WHITE*/, float aspectScale /*= 1.7f*/, BitmapFont* font /*= nullptr*/)
{
	if (font == nullptr)
	{
		font = m_defaultFont;
	}

	SetCurrentTexture(0, font->m_spriteSheet->m_texture);

	int length = (int)text.size();
	Vector2 startPoint = startPos;
	std::vector<Vertex3D_PCU>	vertices;

	// Draw
	for (int i = 0; i < length; i++)
	{
		// Get Current Letter
		char currentLetter = text.at(i);

		// calculate cell width
		float cellWidth = font->GetGlyphAspect() * cellHeight * aspectScale;

		AABB2 posBox = AABB2(startPoint, Vector2(startPoint.x + cellWidth, startPoint.y + cellHeight));
		AABB2 uvBox = AABB2(font->GetUVsForGlyph(currentLetter));

		vertices.push_back(Vertex3D_PCU(
			Vector3(posBox.mins.x, posBox.mins.y, 0.01f), tint, uvBox.mins));

		vertices.push_back(Vertex3D_PCU(
			Vector3(posBox.maxs.x, posBox.mins.y, 0.01f), tint, Vector2(uvBox.maxs.x, uvBox.mins.y)));

		vertices.push_back(Vertex3D_PCU(
			Vector3(posBox.maxs.x, posBox.maxs.y, 0.01f), tint, Vector2(uvBox.maxs.x, uvBox.maxs.y)));

		vertices.push_back(Vertex3D_PCU(
			Vector3(posBox.mins.x, posBox.mins.y, 0.01f), tint, Vector2(uvBox.mins.x, uvBox.mins.y)));

		vertices.push_back(Vertex3D_PCU(
			Vector3(posBox.maxs.x, posBox.maxs.y, 0.01f), tint, Vector2(uvBox.maxs.x, uvBox.maxs.y)));
		
		vertices.push_back(Vertex3D_PCU(
			Vector3(posBox.mins.x, posBox.maxs.y, 0.01f), tint, Vector2(uvBox.mins.x, uvBox.maxs.y)));

		startPoint.x += cellWidth;
	}

	DrawMeshImmediate(PRIMITIVE_TRIANGLES, vertices.data(), (int) vertices.size());
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawWrappedTextInBox2D(const String& text, const AABB2& boxSize, float cellHeight , float aspectScale, 
	const Rgba& textColor, BitmapFont* font)
{
	if (font == nullptr)
	{
		font = m_defaultFont;
	}

	SetCurrentTexture(0, font->m_spriteSheet->m_texture);

	float width = boxSize.GetWidth();
	float fontWidth = cellHeight * aspectScale;
	float fontHeightSpace = cellHeight * 2.f;
	Vector2 currentPosition = Vector2(boxSize.mins.x + fontWidth, boxSize.maxs.y - (fontHeightSpace));

	// this gets the \n character
	Strings vectorOfWords = SplitString(text, " ");

	for (int i = 0; i < (int)vectorOfWords.size(); i++)
	{
		// Get the length of the word
		String currentWord = vectorOfWords.at(i);
		float wordSize = ((float)currentWord.size() * fontWidth);

		if (currentWord == "\n")
		{
			currentPosition.x = (boxSize.mins.x + fontWidth);
			currentPosition.y -= fontHeightSpace;
		}
		else
		{
			float endPos = currentPosition.x + wordSize;

			// WRAP the x
			if (endPos >= (boxSize.mins.x + width))
			{
				currentPosition.x = (boxSize.mins.x + fontWidth);
				currentPosition.y -= fontHeightSpace;
			}

			DrawText2D(currentPosition, currentWord, cellHeight, textColor, aspectScale, font);

			currentPosition.x += (wordSize + fontWidth);
		}
	}
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawCircleFilled2D(const Vector2 & center, float radius, const Rgba & color, int numberOfEdges)
{
	float distanceInDegrees = 360.f / ((float) numberOfEdges * .3f);
	float degrees = 0.f;

	SetCurrentTexture(0, m_defaultTexture);

	std::vector<Vertex3D_PCU> vertices;
	for (int i = 0; i < numberOfEdges; i += 3)
	{
		// Starting point
		float startX = center.x + (radius * (CosDegrees(degrees)));
		float startY = center.y + (radius * (SinDegrees(degrees)));

		// Increase degrees so that we can find the next point
		degrees += distanceInDegrees;

		// End point
		float endX = center.x + (radius * (CosDegrees(degrees)));
		float endY = center.y + (radius * (SinDegrees(degrees)));

		vertices.push_back(Vertex3D_PCU(center, color, Vector2(0.f, 0.f)));
		vertices.push_back(Vertex3D_PCU(Vector2(startX, startY), color, Vector2(0.f, 0.f)));
		vertices.push_back(Vertex3D_PCU(Vector2(endX, endY), color, Vector2(0.f, 0.f)));
	}

	DrawMeshImmediate(PRIMITIVE_TRIANGLES, vertices.data(), numberOfEdges);
}

//-----------------------------------------------------------------------------------------------
void Renderer::DrawCircleOutline2D(const Vector2 & center, float radius, const Rgba & color, int numberOfEdges)
{
	float distanceInDegrees = 360.f / ((float)numberOfEdges * .5f);
	float degrees = 0.f;

	SetCurrentTexture(0, m_defaultTexture);

	std::vector<Vertex3D_PCU> vertices;
	for (int i = 0; i < numberOfEdges; i += 2)
	{
		// Starting point
		float startX = center.x + (radius * (CosDegrees(degrees)));
		float startY = center.y + (radius * (SinDegrees(degrees)));

		// Increase degrees so that we can find the next point
		degrees += distanceInDegrees;

		// End point
		float endX = center.x + (radius * (CosDegrees(degrees)));
		float endY = center.y + (radius * (SinDegrees(degrees)));

		vertices.push_back(Vertex3D_PCU(Vector2(startX, startY), color, Vector2(0.f, 0.f)));
		vertices.push_back(Vertex3D_PCU(Vector2(endX, endY), color, Vector2(0.f, 0.f)));
	}

	DrawMeshImmediate(PRIMITIVE_LINES, vertices.data(), numberOfEdges);
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
		0 );								GL_CHECK_ERROR();

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
void Renderer::DrawMeshImmediateWithoutFramebuffer(PrimitiveType primitiveType, Vertex3D_PCU* vertices, int numOfVertices)
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
		0 );								GL_CHECK_ERROR();

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
Texture* Renderer::CreateOrGetTexture(const String& path, bool flip /*= true*/)
{
	String fullPath = path;
#ifdef EMSCRIPTEN_PORT
	fullPath = "Run_Win32/" + path;
#endif
	
	std::map<String, Texture*>::iterator textureIterator;

	for (textureIterator = m_createdTextures.begin(); textureIterator != m_createdTextures.end(); textureIterator++)
	{
		if (textureIterator->first == fullPath)
			return textureIterator->second;
	}

	// need to make it
	PrintLog("Created a texture at address: " + fullPath);
	Texture* newTexture = new Texture(fullPath, flip);
	m_createdTextures.insert(std::pair<String, Texture*>(fullPath, newTexture));

	return newTexture;
}

//-----------------------------------------------------------------------------------------------
BitmapFont* Renderer::CreateOrGetBitmapFont(const String& path)
{
	String fullPath = path;

	std::map<String, BitmapFont*>::iterator theIterator;

	for (theIterator = m_createdFonts.begin(); theIterator != m_createdFonts.end(); theIterator++)
	{
		if (theIterator->first == fullPath)
			return theIterator->second;
	}

	// create or get texture adds the full path for us 
	Texture* fontTexture = CreateOrGetTexture(path);
	SpriteSheet* fontSpriteSheet = new SpriteSheet(fontTexture, 16, 16);

	BitmapFont* newFont = new BitmapFont(fullPath, *fontSpriteSheet, 1.f);

	m_createdFonts.insert(std::pair<String, BitmapFont*>(fullPath, newFont));

	return newFont;
}

//-----------------------------------------------------------------------------------------------
Renderer * Renderer::GetInstance()
{
	return s_theRenderer;
}
