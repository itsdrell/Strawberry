#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Pipeline/RenderBuffer.hpp"
#include "Engine/Renderer/RenderTypes.hpp"


//====================================================================================
// Forward Declare
//====================================================================================
class Rgba;
class AABB2;
class Camera;
class Shader;
class FrameBuffer;
struct Vertex3D_PCU;
class Texture;
class Sampler;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================
enum PrimitiveType;

//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class Renderer
{
public:
	Renderer();
	~Renderer();

public:
	void RenderStartup(void* hwnd);
	void RenderPostStartUp();
	void BeginFrame();
	void EndFrame();
	bool CopyFrameBuffer( FrameBuffer* dst, FrameBuffer* src );

public:
	void ClearScreen( const Rgba& clearColor );
	void SetCamera( Camera* theCamera = nullptr );
	void BindCameraToShader(const Camera& theCamera);
	void SetShader( Shader* shader = nullptr );
	void SetCurrentTexture( int bindIndex = 0, Texture* texture = nullptr);
	Texture* CreateRenderTarget( int width, int height, eTextureFormat format = TEXTURE_FORMAT_RGBA8 );

public:
	void DrawAABB2( const AABB2& bounds, const Rgba& color, bool filled = true );
	void DrawMeshImmediate( PrimitiveType primitiveType, Vertex3D_PCU* vertices, int numOfVertices );
	void DrawMeshImmediate( PrimitiveType thePrimitive, uint vertexCount, Vertex3D_PCU* vertices, uint indicesCount = 0, uint* indices = nullptr );

public:
	static Renderer* GetInstance();

private:
	void* GetGLLibrary();

private:
	static Renderer* s_theRenderer;

public:

	Camera*				m_currentCamera = nullptr;
	Camera*				m_defaultCamera = nullptr;
									  
	Shader*				m_currentShader = nullptr;
	Shader*				m_defaultShader = nullptr;

	Sampler*			m_defaultSampler = nullptr;
	Sampler*			m_currentSampler = nullptr;

	RenderBuffer*		m_immediateBuffer = nullptr;

	UniformBuffer		m_cameraMatrixBuffer;
	CameraMatrixT		m_cameraMatrixData;

	UniformBuffer		m_modelMatrixBuffer;
	ModelT				m_modelMatrixData;

	Texture*			m_currentTexture = nullptr;
	Texture*			m_defaultTexture = nullptr;
	Texture*			m_defaultColorTarget = nullptr;
	Texture*			m_defaultDepthTarget = nullptr;
	unsigned int		m_defaultVAO; 
	
};

//====================================================================================
// Standalone C Functions
//====================================================================================
void GLShutdown();

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================