#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Renderer/Pipeline/RenderBuffer.hpp"
#include "Engine/Renderer/RenderTypes.hpp"
#include "Engine/Math/Vectors/Vector4.hpp"


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
struct SDL_Surface;
struct SDL_Renderer;

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
	void RenderStartup();
	void RenderPostStartUp();
	void BeginFrame();
	void EndFrame();
	bool CopyFrameBuffer( FrameBuffer* dst, FrameBuffer* src );

public:
	void ClearScreen( const Rgba& clearColor );
	void EnableWireframe(bool check);
	void SetLineWidth(float width);
	void SetCamera( Camera* theCamera = nullptr );
	void SetDefaultDrawColor(const Rgba& theColor);
	void BindCameraToShader(const Camera& theCamera);
	void SetShader( Shader* shader = nullptr );
	void SetCurrentTexture( int bindIndex = 0, Texture* texture = nullptr);
	Texture* CreateRenderTarget( int width, int height, eTextureFormat format = TEXTURE_FORMAT_RGBA8 );

public:
	void SetUniform( const String& name, float uniform );
	void SetUniform( const String& name, const Vector3& uniform );
	void SetUniform( const String& name, const Vector4& uniform ); 
	void SetUniform( const String& name, const Rgba& uniform ); 
	void SetUniform(const String& name, const Matrix44& uniform);

public:
	void DrawLine2D(const Vector2& start, const Vector2& end, const Rgba& color = Rgba(0, 0, 0, 255));
	void DrawLines2D(Vector2* arrayPointer, int numberOfSides, const Rgba& theColor);
	
	void DrawCircleOutline2D(const Vector2& center, float radius, const Rgba& color = Rgba(255, 255, 255, 255), int numberOfEdges = 60);
	void DrawCircleFilled2D(const Vector2& center, float radius, const Rgba& color = Rgba(255, 255, 255, 255), int numberOfEdges = 60);

	void DrawAABB2Outline( const AABB2& bounds, const Rgba& color = Rgba(255, 255, 255, 255));
	void DrawAABB2Filled(const AABB2& bounds, const Rgba& color = Rgba(255, 255, 255, 255));
	void DrawTexturedAABB2(const AABB2& bounds, const Texture& texture,
		const Vector2& texCoordsAtMins, const Vector2& texCoordsAtMaxs, const Rgba& tint );

public:
	void DrawMeshImmediate( PrimitiveType primitiveType, Vertex3D_PCU* vertices, int numOfVertices );
	void DrawMeshImmediate( PrimitiveType thePrimitive, uint vertexCount, Vertex3D_PCU* vertices, uint indicesCount = 0, uint* indices = nullptr );
	void DrawMeshImmediateWithoutFramebuffer(PrimitiveType primitiveType, Vertex3D_PCU* vertices, int numOfVertices);


public:
	static Renderer* GetInstance();

private:
	static Renderer* s_theRenderer;

public:
	Vector2				m_windowSize;

	bool				m_clearScreen = true;
	Rgba				m_clearScreenColor;
	Rgba				m_defaultDrawColor;

	Camera*				m_currentCamera = nullptr;
	Camera*				m_defaultCamera = nullptr;
	Camera*				m_defaultUICamera = nullptr;
									  
	Shader*				m_currentShader = nullptr;
	Shader*				m_defaultShader = nullptr;

	Sampler*			m_defaultSampler = nullptr;
	Sampler*			m_currentSampler = nullptr;

	RenderBuffer*		m_immediateBuffer = nullptr;

	Texture*			m_currentTexture = nullptr;
	Texture*			m_defaultTexture = nullptr;
	Texture*			m_defaultColorTarget = nullptr;
	Texture*			m_defaultDepthTarget = nullptr;
	unsigned int		m_defaultVAO; 
	
	SDL_Surface*		m_webScreen = nullptr;
	SDL_Renderer*		m_theSDLRenderer = nullptr;
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