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
class Sprite;
class BitmapFont;
class Mesh;
class ShaderProgram;
struct RenderState;

//====================================================================================
// Type Defs + Defines
//====================================================================================
constexpr float C_TEXT_PADDING = 1.5f;

//====================================================================================
// ENUMS
//====================================================================================
enum PrimitiveType;

//-----------------------------------------------------------------------------------------------
enum DrawTextMode
{
	DRAW_TEXT_MODE_WRAPPED, // Goes to new line
	DRAW_TEXT_MODE_SHRINKED, // shrinks to fit in bounds
	DRAW_TEXT_MODE_OVERFLOW, // keeps going past bounds

	NUM_OF_DRAW_TEXT_MODES
};

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
	void SetCurrentTexture( int bindIndex = 0, const Texture* texture = nullptr);
	void CreateAllRenderTargets(int width, int height);
	void ResizeViewport(int width, int height);

public:
	void SetUniform( const String& name, float uniform );
	void SetUniform( const String& name, const Vector3& uniform );
	void SetUniform( const String& name, const Vector4& uniform ); 
	void SetUniform( const String& name, const Rgba& uniform ); 
	void SetUniform(const String& name, const Matrix44& uniform);

	void SetSamplerUniform(const String& name, int uniform);

public:
	void DrawLine2D(const Vector2& start, const Vector2& end, const Rgba& color = Rgba(0, 0, 0, 255));
	void DrawLines2D(Vector2* arrayPointer, int numberOfSides, const Rgba& theColor);
	
	void DrawCircleOutline2D(const Vector2& center, float radius, const Rgba& color = Rgba(255, 255, 255, 255), int numberOfEdges = 60);
	void DrawCircleFilled2D(const Vector2& center, float radius, const Rgba& color = Rgba(255, 255, 255, 255), int numberOfEdges = 60);

	void DrawAABB2Outline( const AABB2& bounds, const Rgba& color = Rgba(255, 255, 255, 255));
	void DrawAABB2Filled(const AABB2& bounds, const Rgba& color = Rgba(255, 255, 255, 255));
	void DrawTexturedAABB2(const AABB2& bounds, const Texture& texture,
		const Vector2& texCoordsAtMins, const Vector2& texCoordsAtMaxs, const Rgba& tint );

	//void DrawTexturedOOBB2(const Vector3& position, const Texture& theTexture, const AABB2& uvs, float rotation = 0.f, bool flipX = false, bool flipY = false);

	void DrawSpriteRotated2D(const Vector3& position, const Sprite& theSprite, float rotation = 0.f, bool flipX = false, bool flipY = false);

	void DrawSprite(const Vector3& position, const Sprite& theSprite, bool flipX = false, bool flipY = false,
		const Vector3& right = Vector3::RIGHT, const Vector3& up = Vector3::UP);

public:
	void DrawText2D(const Vector2& startPos, const String& text, float cellHeight, const Rgba& tint = Rgba::WHITE,
		bool drawOutline = false, const Rgba& outlineColor = Rgba::BLACK, float aspectScale = 1.7f, BitmapFont* font = nullptr);
	
	void DrawTextInBox(const String& text, const AABB2& bounds, float cellHeight, float normalizedPercentIntoText = 1.f, DrawTextMode mode = DRAW_TEXT_MODE_OVERFLOW, 
		const Vector2& alignment = Vector2(0, 0), const Rgba& color = Rgba::WHITE, bool drawOutline = false, const Rgba& outlineColor = Rgba::BLACK, 
		float aspect = 1.77f, BitmapFont* font = nullptr);

private:
	void DrawText2DImmediately(const Vector2& drawMins, const String& text, float cellHeight,
		const Rgba& tint = Rgba::WHITE, float aspectScale = 1.7f, BitmapFont* font = nullptr);
		
private: // Do not call these, use DrawTextInBox instead!!!
	void DrawWrappedTextInBox2D(const String& text, const AABB2& boxSize, float cellHeight = 1.f, const Vector2& alignment = Vector2(0,0),
		float aspectScale = 1.77f, const Rgba& textColor = Rgba::WHITE, bool drawOutline = false, const Rgba& outlineColor = Rgba::BLACK, BitmapFont* font = nullptr);

	void DrawShrinkToFitTextInBox2D(const String& text, const AABB2& boxSize, float cellHeight = 1.f, const Vector2& alignment = Vector2(0, 0),
		float aspectScale = 1.77f, const Rgba& textColor = Rgba::WHITE, bool drawOutline = false, const Rgba& outlineColor = Rgba::BLACK, BitmapFont* font = nullptr);

	void DrawOverflowTextInBox2D(const String& text, const AABB2& boxSize, float cellHeight = 1.f, const Vector2& alignment = Vector2(0, 0),
		float aspectScale = 1.77f, const Rgba& textColor = Rgba::WHITE, bool drawOutline = false, const Rgba& outlineColor = Rgba::BLACK, BitmapFont* font = nullptr);

	void DrawTextWithAlignment(const Strings& text, const AABB2& boxSize, float cellHeight = 1.f, const Vector2& alignment = Vector2(0, 0),
		float aspectScale = 1.77f, const Rgba& textColor = Rgba::WHITE, bool drawOutline = false, const Rgba& outlineColor = Rgba::BLACK, BitmapFont* font = nullptr);

public:
	void DrawMeshImmediate( PrimitiveType primitiveType, Vertex3D_PCU* vertices, int numOfVertices );
	//void DrawMeshImmediate( PrimitiveType thePrimitive, uint vertexCount, Vertex3D_PCU* vertices, uint indicesCount = 0, uint* indices = nullptr );
	void DrawMeshImmediateWithoutFramebuffer(PrimitiveType primitiveType, Vertex3D_PCU* vertices, int numOfVertices);
	
	void DrawMesh(Mesh* mesh, bool deleteTempMesh = false);
	void BindMeshToProgram(ShaderProgram* program, Mesh* mesh);
	void BindRenderState(const RenderState& state);

private:
	Texture* CreateRenderTarget(int width, int height, eTextureFormat format = TEXTURE_FORMAT_RGBA8);

public:
	Texture* CreateOrGetTexture(const String& path, bool flip = true);
	bool DeleteTexture(const String& path);
	BitmapFont* CreateOrGetBitmapFont(const String& path);

public:
	static Renderer* GetInstance();

private:
	static Renderer* s_theRenderer;

private:
	std::map<String, Texture*>		m_createdTextures;
	std::map<String, BitmapFont*>	m_createdFonts;

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
	uint				m_maxAmountOfAttributesBinded = 0U;

	Sampler*			m_defaultSampler = nullptr;
	Sampler*			m_currentSampler = nullptr;

	RenderBuffer*		m_immediateBuffer = nullptr;

	Texture*			m_currentTexture = nullptr;
	Texture*			m_defaultTexture = nullptr;
	Texture*			m_defaultColorTarget = nullptr;
	Texture*			m_defaultDepthTarget = nullptr;
	unsigned int		m_defaultVAO; 

	BitmapFont*			m_defaultFont = nullptr;
	
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