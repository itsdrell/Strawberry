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
constexpr float GIF_DEFAULT_LENGTH_IN_SECONDS = 8.f;


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
	static GifRecorder* Instance();

public:
	void Record();
	void SetLength(float gifLengthInSeconds);

public:
	void Start();
	void Stop();

	void AddFrame();
	void Save();

	void StopAndSave();
	void ClearFrames();

public:
	std::vector<Screenshot*>	m_frames;
	IntVector2					m_startingDimensions;
	StopWatch*					m_captureDelayTimer = nullptr;
	bool						m_isRecording = false;
	bool						m_isSaving = false;

public:
	float m_gifLengthInSeconds = GIF_DEFAULT_LENGTH_IN_SECONDS;
	float m_gifSnapDelay;
	int m_maxNumberOfGifFrames;

private:
	static GifRecorder* s_instance;
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