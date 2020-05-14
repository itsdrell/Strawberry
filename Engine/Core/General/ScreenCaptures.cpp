#pragma once
#include "Engine/Core/General/ScreenCaptures.hpp"
#include "Engine/Math/Vectors/IntVector2.hpp"
#include "Engine/ThirdParty/stbi/stb_image_write.h"
#include "Engine/Core/Platform/Time.hpp"
#include "Engine/Core/Platform/File.hpp"
#include "Engine/Core/Platform/Window.hpp"
#include <thread>

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
}

//-----------------------------------------------------------------------------------------------
Screenshot::Screenshot()
{
	m_dimensions = Window::GetInstance()->GetDimensions().GetAsIntVector2();
}

//-----------------------------------------------------------------------------------------------
Screenshot::~Screenshot()
{
	if (m_data != nullptr)
		delete m_data;
}

//-----------------------------------------------------------------------------------------------
void Screenshot::SaveToFullDirectoryPath(const String& path)
{
	m_path = path;
	String justDirectory = RemoveFileFromDirectoryPath(m_path.c_str());
	
	CreateADirectory(justDirectory.c_str());
	CreateScreenshot();
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
	CreateScreenshot();
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

	std::thread newThread = std::thread(CreateScreenshotPNG, (void*)this);
	newThread.detach();
}

