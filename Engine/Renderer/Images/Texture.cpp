#include "Texture.hpp"
#include "Engine/ThirdParty/stbi/stb_image.h"
#include "Engine/ThirdParty/stbi/stb_image_write.h"
#include "Engine/Renderer/Images/Image.hpp"
#include "Engine/Core/General/EngineCommon.hpp"
#include "Engine/Internal/EmscriptenCommon.hpp"
#include "Engine/ThirdParty/SDL2/SDL.h"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"

//===============================================================================================
Texture::Texture()
{
	m_textureID = 0; 
	m_dimensions = IntVector2(0,0);
}

//-----------------------------------------------------------------------------------------------
Texture::Texture(const String& filePath, bool flip /*= true*/)
	: m_textureID( 0 )
	, m_dimensions( 0, 0 )
{
	GL_CHECK_ERROR();

	int numComponents = 0; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
	int numComponentsRequested = 0; // don't care; we support 3 (RGB) or 4 (RGBA)

									// This flips my UVs which are current top left is 0,0 to bottom left being 0,0
	if(flip)
		stbi_set_flip_vertically_on_load(1);
	else
		stbi_set_flip_vertically_on_load(0);

	// Load (and decompress) the image RGB(A) bytes from a file on disk, and create an OpenGL texture instance from it
	unsigned char* imageData = stbi_load( filePath.c_str(), &m_dimensions.x, &m_dimensions.y, &numComponents, numComponentsRequested );

	if (imageData == nullptr || m_dimensions.x == 0)
		PrintLog("Could not load texture: " + filePath);

	PopulateFromData( imageData, m_dimensions, numComponents );
	stbi_image_free( imageData );
}

//-----------------------------------------------------------------------------------------------
Texture::~Texture()
{
	glDeleteTextures(1, (GLuint*) &m_textureID);
}

//-----------------------------------------------------------------------------------------------
void Texture::PopulateFromData(unsigned char * imageData, const IntVector2 & texelSize, int numComponents)
{
	GL_CHECK_ERROR();

	m_dimensions = texelSize;

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );						GL_CHECK_ERROR();

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	glGenTextures( 1, (GLuint*) &m_textureID );						GL_CHECK_ERROR();

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture( GL_TEXTURE_2D, m_textureID );					GL_CHECK_ERROR();

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ); // GL_CLAMP or GL_REPEAT (WAS GL_CLAMP)
	GL_CHECK_ERROR();

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ); // GL_CLAMP or GL_REPEAT
	GL_CHECK_ERROR();

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); // one of: GL_NEAREST, GL_LINEAR
	GL_CHECK_ERROR();

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
	GL_CHECK_ERROR();

	GLenum bufferFormat = GL_RGBA; // was GL_RGBA8, the format our source pixel data is in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if( numComponents == 3 )
		bufferFormat = GL_RGB8;

	GLenum channels = GL_RGBA;
	if(numComponents == 3)
		channels = GL_RGB;

	GLenum internalFormat = bufferFormat; // the format we want the texture to be on the card; allows us to translate into a different texture format as we upload to OpenGL

	// create the gpu buffer
	// note: only this is needed for render targets
	//glTexStorage2D( GL_TEXTURE_2D,
	//	1,               // number of levels (mip-layers)
	//	internalFormat, // how is the memory stored on the GPU
	//	m_dimensions.x, 
	//	m_dimensions.y ); // dimensions
	//
	//GL_CHECK_ERROR();
	//// copies cpu memory to the gpu - needed for texture resources
	//glTexSubImage2D( GL_TEXTURE_2D,
	//	0,             // mip layer we're copying to
	//	0, 0,          // offset
	//	m_dimensions.x, 
	//	m_dimensions.y, // dimensions
	//	channels,      // which channels exist in the CPU buffer
	//	GL_UNSIGNED_BYTE,     // how are those channels stored
	//	imageData ); // cpu buffer to copy;

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		internalFormat,
		m_dimensions.x,
		m_dimensions.y,
		0,
		bufferFormat,
		GL_UNSIGNED_BYTE,
		imageData);

	GL_CHECK_ERROR();

#ifdef EMSCRIPTEN_PORT
	glActiveTexture(GL_TEXTURE0);
#else
	glActiveTexturePls( GL_TEXTURE0 );								GL_CHECK_ERROR();
#endif
	glBindTexture( GL_TEXTURE_2D, m_textureID );				GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
bool Texture::CreateRenderTarget(int width, int height, eTextureFormat format)
{
	GL_CHECK_ERROR();

	// generate the link to this texture
	glGenTextures( 1, &m_textureID );							GL_CHECK_ERROR();
	if (m_textureID == NULL) {
		return false; 
	}

	// TODO - add a TextureFormatToGLFormats( GLenum*, GLenum*, GLenum*, eTextureFormat )
	//        when more texture formats are required; 
	GLenum internal_format = GL_RGBA; // was GL_RGBA8 
	GLenum channels = GL_RGBA;  
	GLenum pixel_layout = GL_UNSIGNED_BYTE;  
	if (format == TEXTURE_FORMAT_D24S8) 
	{
		internal_format = GL_DEPTH_STENCIL; 
		channels = GL_DEPTH_STENCIL; 
		pixel_layout = GL_UNSIGNED_INT_24_8;  
	}

	// Copy the texture - first, get use to be using texture unit 0 for this; 
#ifdef EMSCRIPTEN_PORT
	glActiveTexture(GL_TEXTURE0);
#else
	glActiveTexturePls( GL_TEXTURE0 );								GL_CHECK_ERROR();
#endif

	glBindTexture( GL_TEXTURE_2D, m_textureID );				GL_CHECK_ERROR();

	// Copy data into it;
	glTexImage2D( GL_TEXTURE_2D, 0, 
		internal_format, // what's the format OpenGL should use
		width, 
		height,        
		0,             // border, use 0
		channels,      // how many channels are there?
		pixel_layout,  // how is the data laid out
		nullptr );     // don't need to pass it initialization data

	GL_CHECK_ERROR(); 

	//////////////////////////////////////////////////////////////////////////
	// cleanup after myself and unset it
	glBindTexture( GL_TEXTURE_2D, NULL );						GL_CHECK_ERROR();

	// Save this all off
	m_dimensions.x = width;  
	m_dimensions.y = height; 

	m_format = format; // I save the format with the texture
					// for sanity checking.

	// great, success
	return true; 
}

//-----------------------------------------------------------------------------------------------
Texture * Texture::CreateFromImage(const Image & imageToCreateFrom)
{
	GL_CHECK_ERROR();

	m_dimensions = imageToCreateFrom.GetDimensions();

	// The image class calls the flip command so we don't have to worry about it!
	unsigned char* imageBuffer = imageToCreateFrom.GetColorCharPointer();
	PopulateFromData((unsigned char* ) imageBuffer, m_dimensions, 4 );

	GL_CHECK_ERROR();
	return this;
}

//-----------------------------------------------------------------------------------------------
SDL_Surface* Texture::GetSDLSurfaceFromGLTexture()
{

// does not work on web because of glGetTexImage
#ifndef EMSCRIPTEN_PORT
	//glBindTexture( GL_TEXTURE_2D, m_textureID );    // bind our texture to our current texture unit (0)
	//
	//const int size = m_dimensions.x * m_dimensions.y * 4; //4 because texels rgba
	//
	//if(m_data != NULL)
	//	free(m_data);
	//m_data = (unsigned char*)malloc(size);
	//
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE,m_data);
	//
	//// the color format you request stb_image to output,
	//// use STBI_rgb if you don't want/need the alpha channel
	//int req_format = STBI_rgb_alpha;
	//int width = m_dimensions.x; 
	//int height = m_dimensions.y; 
	//int /*orig_format*/ depth, pitch;
	//Uint32 pixel_format;
	//if (req_format == STBI_rgb) {
	//	depth = 24;
	//	pitch = 3*width; // 3 bytes per pixel * pixels per row
	//	pixel_format = SDL_PIXELFORMAT_RGB24;
	//} else { // STBI_rgb_alpha (RGBA)
	//	depth = 32;
	//	pitch = 4*width;
	//	pixel_format = SDL_PIXELFORMAT_RGBA32;
	//}
	//
	//SDL_Surface* theSurface = SDL_CreateRGBSurfaceWithFormatFrom((void*)m_data, width, height,
	//	depth, pitch, pixel_format);
	//
	//return theSurface;
	return nullptr;
#else
	return nullptr;
#endif
}

//-----------------------------------------------------------------------------------------------
SDL_Texture* Texture::GetSDLTextureFromGLTexture()
{
	Renderer* r = Renderer::GetInstance();
	
	SDL_Surface* tempSurface = GetSDLSurfaceFromGLTexture();

	SDL_Texture* theTexture = SDL_CreateTextureFromSurface(r->m_theSDLRenderer, tempSurface);

	SDL_FreeSurface(tempSurface);

	return theTexture;
}
