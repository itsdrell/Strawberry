#pragma once
#include "Engine/Core/General/ScreenCaptures.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Engine/ThirdParty/stbi/stb_image_write.h"
#include "Engine/Core/Platform/Time.hpp"
#include "Engine/Core/Platform/File.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include "Engine/ThirdParty/gif_h/gif.h"
#include "Engine/Core/Tools/DebugRendering.hpp"
#include "Engine/Core/Tools/StopWatch.hpp"
#include "Engine/Core/Tools/Clock.hpp"
#include <thread>
#include "BlackBoard.hpp"

//===============================================================================================
GifRecorder* GifRecorder::s_instance = nullptr;

//===============================================================================================
static void CreateScreenshotPNG(void* screenshot)
{
	Screenshot* shot = (Screenshot*)screenshot;

	stbi_flip_vertically_on_write(1);
	stbi_write_png(shot->m_path.c_str(), shot->m_dimensions.x, shot->m_dimensions.y, 4, shot->GetData(), 0); // using zero auto formats

	delete shot;
}

//===============================================================================================
Screenshot::Screenshot(const Texture& theTextureToUse)
{
	m_texture = &theTextureToUse;
	//m_dimensions = m_texture->GetDimensions();

	// need to do window size because the window can change sizes and 
	// the dimensions change. Maybe don't need if using a actual framebuffer texture
	m_dimensions = Window::GetInstance()->GetDimensions().GetAsIntVector2();

	CreateScreenshot();
}

//-----------------------------------------------------------------------------------------------
Screenshot::Screenshot()
{
	m_dimensions = Window::GetInstance()->GetDimensions().GetAsIntVector2();
	CreateScreenshot();
}

//-----------------------------------------------------------------------------------------------
Screenshot::~Screenshot()
{
	if (m_data != NULL)
		free(m_data);
}

//-----------------------------------------------------------------------------------------------
void Screenshot::GetDataFlipped(void* outFlippedBuffer)
{
	Rgba* original = (Rgba*) m_data;
	Rgba* flipped = (Rgba*) outFlippedBuffer;
	 
	int width = m_dimensions.x;
	int height = m_dimensions.y;

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			int currentIndex = y * width + x;
			int swapIndex = (height - y) * width - (width - x);
			flipped[currentIndex] = original[swapIndex];
		}
	}
}

//-----------------------------------------------------------------------------------------------
void Screenshot::FlipData()
{
	int size = m_dimensions.x * m_dimensions.y;
	void* tempbuffer = malloc(sizeof(Rgba) * size);
	
	GetDataFlipped(tempbuffer);

	free(m_data);
	m_data = (unsigned char*) tempbuffer;
}

//-----------------------------------------------------------------------------------------------
void Screenshot::SaveToFullDirectoryPath(const String& path)
{
	m_path = path;
	String justDirectory = RemoveFileFromDirectoryPath(m_path.c_str());
	
	CreateADirectory(justDirectory.c_str());

	std::thread newThread = std::thread(CreateScreenshotPNG, (void*)this);
	newThread.detach();
}

//-----------------------------------------------------------------------------------------------
void Screenshot::SaveToScreenshotProjectFolder(const String& gameName)
{
	std::string dir = "Captures/" + gameName + "/";
	std::string path = GetWorkingDirectoryPath() + "\\" + dir;
	std::string timeStamp = CurrentDateTime();

	std::string fullPath = path + timeStamp + ".png";
	m_path = fullPath;

	CreateADirectory(dir.c_str());

	std::thread newThread = std::thread(CreateScreenshotPNG, (void*)this);
	newThread.detach();
}

//-----------------------------------------------------------------------------------------------
void Screenshot::CreateScreenshot()
{
	const int size = m_dimensions.x * m_dimensions.y * 4; //4 because texels rgba
	m_data = (unsigned char*)malloc(size);

	// unbind any textures so we can read straight from the buffer
	// TODO: if we get framebuffers, we will have to bind the texture id instead
	// TODO2: could also make this a renderer function so we aren't calling gl functions from out here
	glBindTexture(GL_TEXTURE_2D, 0);    // bind our texture to our current texture unit (0)
	glReadPixels(0, 0, m_dimensions.x, m_dimensions.y, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
}

//===============================================================================================
GifRecorder::GifRecorder()
{
	SetLength(g_theEngineBlackboard->GetValue("gifLength", GIF_DEFAULT_LENGTH_IN_SECONDS));

	m_captureDelayTimer = new StopWatch(g_theMasterClock);
	m_captureDelayTimer->SetTimer(m_gifSnapDelay);

	s_instance = this;
}

//-----------------------------------------------------------------------------------------------
GifRecorder::~GifRecorder()
{
	ClearFrames();

	delete m_captureDelayTimer;
	m_captureDelayTimer = nullptr;
}

//-----------------------------------------------------------------------------------------------
STATIC GifRecorder* GifRecorder::Instance()
{
	return s_instance;
}

//-----------------------------------------------------------------------------------------------
void GifRecorder::Start()
{
	if (m_isSaving) { return; } // dont try anything while the thread is saving!!!
	
	if(m_isRecording)
	{
		Stop();
		ClearFrames();
	}
	
	m_frames.reserve(m_maxNumberOfGifFrames);
	m_isRecording = true;
	m_startingDimensions = Window::GetInstance()->GetDimensions().GetAsIntVector2();
	
	m_captureDelayTimer->Reset();
}

//-----------------------------------------------------------------------------------------------
void GifRecorder::Stop()
{
	m_isRecording = false;
}

//-----------------------------------------------------------------------------------------------
void GifRecorder::Record()
{
	if (m_captureDelayTimer->CheckAndReset())
	{
		AddFrame();
	}

	// make sure the window didnt change sizes, cause that will break stuff
	if(m_startingDimensions != Window::GetInstance()->GetDimensions().GetAsIntVector2())
	{
		Stop();
	}

	// we have added enough, lets stop and save
	if(m_frames.size() == m_maxNumberOfGifFrames)
	{
		Stop();
		Save();
	}

	DebugRenderLog("RECORDING", 0, Rgba::WHITE);
}

//-----------------------------------------------------------------------------------------------
void GifRecorder::AddFrame()
{
	Screenshot* newFrame = new Screenshot();
	m_frames.push_back(newFrame);
}

//-----------------------------------------------------------------------------------------------
void GifRecorder::SetLength(float gifLengthInSeconds)
{
	m_gifLengthInSeconds	= gifLengthInSeconds;
	
	m_maxNumberOfGifFrames	= (int)(GIF_FRAMES_PER_SECOND * m_gifLengthInSeconds);
	m_gifSnapDelay			= 1.f / ((float)GIF_FRAMES_PER_SECOND);

	if(m_isRecording)
	{
		Stop();
		ClearFrames();
	}

	m_frames.reserve(m_maxNumberOfGifFrames);
}

//-----------------------------------------------------------------------------------------------
static void SaveToGif(void* data)
{
	GifRecorder* recorder = (GifRecorder*)data;
	
	int width = recorder->m_startingDimensions.x;
	int height = recorder->m_startingDimensions.y;

	std::string dir = "Captures/" + g_currentProjectName + "/";
	std::string path = GetWorkingDirectoryPath() + "\\" + dir;
	std::string timeStamp = CurrentDateTime();

	std::string fullPath = path + timeStamp + ".gif";

	CreateADirectory(dir.c_str());

	GifWriter g;
	int delay = (int)recorder->m_gifSnapDelay;
	GifBegin(&g, fullPath.c_str(), width, height, delay);

	for (uint i = 0; i < recorder->m_frames.size(); i++)
	{
		Screenshot* current = recorder->m_frames.at(i);
		const uint8_t* dataCasted = (const uint8_t*)current->GetData();
		GifWriteFrame(&g, dataCasted, width, height, delay);
	}

	GifEnd(&g);
	recorder->ClearFrames();
	recorder->m_isSaving = false;
}

//-----------------------------------------------------------------------------------------------
void GifRecorder::Save()
{
	m_isSaving = true;
	std::thread newThread = std::thread(SaveToGif, (void*)this);
	newThread.detach();
}

//-----------------------------------------------------------------------------------------------
void GifRecorder::ClearFrames()
{
	for(uint i = 0; i < m_frames.size(); i++)
	{
		delete m_frames.at(i);
	}

	m_frames.clear();
}
