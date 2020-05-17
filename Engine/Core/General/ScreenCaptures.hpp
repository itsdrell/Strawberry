#include "Engine/Renderer/Images/Texture.hpp"

//====================================================================================
// Forward Declare
//====================================================================================
class IntVector2;
class StopWatch;

//====================================================================================
// Type Defs + Defines
//====================================================================================
constexpr int GIF_FRAMES_PER_SECOND = 10;
constexpr int GIF_LENGTH_IN_SECONDS = 8;

constexpr int MAX_NUMBER_OF_GIF_FRAMES = (GIF_FRAMES_PER_SECOND * GIF_LENGTH_IN_SECONDS); // 30 frames x 8 seconds
constexpr float GIF_SNAP_DELAY = 1.f / ((float) GIF_FRAMES_PER_SECOND);

//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class Screenshot
{
public:
	Screenshot(); // just use the framebuffer?
	Screenshot(const Texture& theTextureToUse);
	
	~Screenshot();

public:
	void* GetData() { return m_data; }
	void GetDataFlipped(void* outFlippedBuffer);
	void FlipData();

public:
	void SaveToFullDirectoryPath(const String& path);
	void SaveToScreenshotProjectFolder(const String& gameName);

private:
	void CreateScreenshot();

public:
	IntVector2			m_dimensions;
	String				m_path;

private:
	unsigned char*		m_data = nullptr;
	const Texture*		m_texture;
};

//===============================================================================================
class GifRecorder
{
public:
	GifRecorder();
	~GifRecorder();

public:
	void Record();
	
public:
	void Start();
	void Stop();

	void AddFrame();
	void Save();

	void ClearFrames();

public:
	std::vector<Screenshot*>	m_frames;
	IntVector2					m_startingDimensions;
	StopWatch*					m_captureDelayTimer = nullptr;
	bool						m_isRecording = false;
	bool						m_isSaving = false;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [5/13/2020]
//====================================================================================