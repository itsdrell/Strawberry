#include "Engine/Renderer/BuiltInShaders.hpp"
#include "Engine/Renderer/Components/Shader.hpp"
#include "Engine/Renderer/Components/ShaderProgram.hpp"

//===============================================================================================
void BuiltInShaders::CreateAllBuiltInShaders()
{
	//Shader::AddShader("default", CreateDefaultShader());
	//Shader::AddShader("invalid", CreateDefaultShader());
}

//-----------------------------------------------------------------------------------------------
Shader* BuiltInShaders::CreateDefaultShader()
{

#pragma region FragmentAndVertexCode

	//////////////////////////////////////////////////////////////////////////
	// FS
	const char* fs = R"(#version 100

precision mediump float; //important

//-----------------------------------------------------------------------------------------------
uniform sampler2D gTexDiffuse;

//-----------------------------------------------------------------------------------------------
// We match the name and type of the previous stages out
varying vec4 passColor;
varying vec2 passUV; 

//-----------------------------------------------------------------------------------------------
void main( void )
{
   vec4 diffuse = texture2D( gTexDiffuse, passUV ); 
   
   //vec3 diffuseColor = pow(texture2D(diffuse, passUV).xyz, vec3(2.2))
	
	// this is gamma correction and we do it to the final color, instead of to the
	// texture sample color because the texture has no color (atm)
	vec4 color = diffuse * passColor;
	//color.x = pow(color.x, 2.2);
	//color.y = pow(color.y, 2.2);
	//color.z = pow(color.z, 2.2);

	gl_FragColor = color;

})";

	//////////////////////////////////////////////////////////////////////////
	// VS
	const char* vs = R"(// define the shader version (this is required)
#version 100

//-----------------------------------------------------------------------------------------------
uniform mat4 VIEW;
uniform mat4 PROJECTION;
uniform mat4 VIEW_PROJECTION;
uniform mat4 MODEL;

//-----------------------------------------------------------------------------------------------
attribute vec3 POSITION;
attribute vec4 COLOR;      
attribute vec2 UV;          

//-----------------------------------------------------------------------------------------------
//pass variables or "varying" variables
// These are variables output from this stage for use by the next stage
// These are called "varying" because for a triangle, we'll only output 3, and 
// the Raster stage may output hundreds of pixels, each a blended value
// of the initial 3, hence - they "vary"
varying vec2 passUV; 
varying vec4 passColor; 

//-----------------------------------------------------------------------------------------------
void main( void )
{
    vec4 local_pos = vec4( POSITION, 1 ); 
    vec4 clip_pos =  PROJECTION * (VIEW * local_pos); 
   
    passColor = COLOR; // pass it on. 
    passUV = UV; // we have to set our outs.
	
	//gl_texcoord = UV;
    gl_Position = clip_pos;  
})";

#pragma endregion

	Shader* newShader = new Shader();

	//////////////////////////////////////////////////////////////////////////
	// Create shader program
	Strings theDefines;
	//theDefines.push_back("hi");

	newShader->SetProgram(new ShaderProgram("default", fs,vs, theDefines));

	//////////////////////////////////////////////////////////////////////////
	// Add all properties
	//newShader->m_textures.push_back(Renderer::GetInstance()->CreateOrGetTexture("Data/Images/defaultTexture.png"));

	return newShader;
}

//-----------------------------------------------------------------------------------------------
Shader* BuiltInShaders::CreateInvalidShader()
{
	Shader* newShader = new Shader();

	//////////////////////////////////////////////////////////////////////////
	// Create shader program
	Strings theDefines;

	newShader->SetProgram(new ShaderProgram("default", GetInvalidFragment(),GetInvalidVertex(), theDefines));

	//////////////////////////////////////////////////////////////////////////
	// Add all properties
	//newShader->m_textures.push_back(Renderer::GetInstance()->CreateOrGetTexture("Data/Images/defaultTexture.png"));

	return newShader;
}

//-----------------------------------------------------------------------------------------------
Shader* BuiltInShaders::CreateStrawberryShader()
{
#pragma region FragmentAndVertexCode

	//////////////////////////////////////////////////////////////////////////
	// FS
	const char* fs = R"(#version 100

precision mediump float; //important

//-----------------------------------------------------------------------------------------------
uniform sampler2D gDefaultTexDiffuse;
uniform sampler2D gDefaultFontTexDiffuse;

uniform sampler2D gSpriteSheetTexDiffuse_0;
uniform sampler2D gSpriteSheetTexDiffuse_1;
uniform sampler2D gSpriteSheetTexDiffuse_2;
uniform sampler2D gSpriteSheetTexDiffuse_3;

//-----------------------------------------------------------------------------------------------
// We match the name and type of the previous stages out
varying vec4 passColor;
varying vec2 passUV; 
varying float passTextureID;

//-----------------------------------------------------------------------------------------------
void main( void )
{
	vec4 textureSamples[6];
	textureSamples[0] = texture2D( gDefaultTexDiffuse, passUV );
	textureSamples[1] = texture2D( gDefaultFontTexDiffuse, passUV );
	textureSamples[2] = texture2D( gSpriteSheetTexDiffuse_0, passUV );
	textureSamples[3] = texture2D( gSpriteSheetTexDiffuse_1, passUV );
	textureSamples[4] = texture2D( gSpriteSheetTexDiffuse_2, passUV );
	textureSamples[5] = texture2D( gSpriteSheetTexDiffuse_3, passUV );

	int textureIDToUse = int(passTextureID);

	vec4 diffuse = textureSamples[textureIDToUse];
	vec4 finalColor = diffuse * passColor;

	gl_FragColor = finalColor;

})";

	//////////////////////////////////////////////////////////////////////////
	// VS
	const char* vs = R"(// define the shader version (this is required)
#version 100

//-----------------------------------------------------------------------------------------------
uniform mat4 VIEW;
uniform mat4 PROJECTION;
uniform mat4 VIEW_PROJECTION;
uniform mat4 MODEL;

//-----------------------------------------------------------------------------------------------
attribute vec3 POSITION;
attribute vec4 COLOR;      
attribute vec2 UV;          
attribute float TEXTURE_ID;

//-----------------------------------------------------------------------------------------------
//pass variables or "varying" variables
// These are variables output from this stage for use by the next stage
// These are called "varying" because for a triangle, we'll only output 3, and 
// the Raster stage may output hundreds of pixels, each a blended value
// of the initial 3, hence - they "vary"
varying vec2 passUV; 
varying vec4 passColor; 
varying float passTextureID;

//-----------------------------------------------------------------------------------------------
void main( void )
{
    vec4 local_pos = vec4( POSITION, 1 ); 
    vec4 clip_pos =  PROJECTION * (VIEW * local_pos); 
   
    passColor = COLOR; // pass it on. 
    passUV = UV; // we have to set our outs.
	passTextureID = TEXTURE_ID;	

    gl_Position = clip_pos;  
})";

#pragma endregion

	Shader* newShader = new Shader();
	newShader->m_name = "Strawberry";
	Strings theDefines;

	newShader->SetProgram(new ShaderProgram("Strawberry", fs, vs, theDefines));

	return newShader;
}

const char* BuiltInShaders::GetInvalidFragment()
{
	const char* fs = R"(#version 420 core

// create a uniform for our sampler2D.
// layout binding = 0 is us binding it to texture slot 0.  
layout(binding = 0) uniform sampler2D gTexDiffuse;


// We match the name and type of the previous stages out
in vec4 passColor; // NEW, passed color
in vec2 passUV; 


// Outputs
out vec4 outColor; 

// Entry Point
void main( void )
{
   // sample (gather) our texel colour for this UV
   vec4 diffuse = texture( gTexDiffuse, passUV ); 
   //passColor = vec4(1.0, 0.0, 1.0, 1.0);
   outColor = vec4(1.0, 0.0, 1.0, 1.0);
})";

	return fs;
}

const char* BuiltInShaders::GetInvalidVertex()
{
	const char* vs = R"(// define the shader version (this is required)
#version 420 core

// Frame information ;
layout(binding=1, std140) uniform cFrameBlock 
{
   float GAME_TIME;
   float GAME_DELTA_TIME;
   float SYSTEM_TIME; 
   float SYSTEM_DELTA_TIME; 
}; 

// Camera Constants
layout(binding=2, std140) uniform cCameraBlock 
{
    mat4 VIEW;
    mat4 PROJECTION; 
    mat4 VIEW_PROJECTION; 

    mat4 INV_VIEW;          // also just called the camera matrix
    mat4 INV_PROJECTION; 
    mat4 INT_VIEW_PROJECTION; 

    // dupliate of INV_VIEW, but just convenient
    vec3 CAMERA_RIGHT;      float pad0;
    vec3 CAMERA_UP;         float pad1; 
    vec3 CAMERA_FORWARD;    float pad2; 
    vec3 CAMERA_POSITION;   float pad3;  

}; 

uniform mat4 MODEL;


// Attributes - input to this shasder stage (constant as far as the code is concerned)
in vec3 POSITION;
in vec4 COLOR;       // NEW - GLSL will use a Vector4 for this;
in vec2 UV;          // new


//pass variables or "varying" variables
// These are variables output from this stage for use by the next stage
// These are called "varying" because for a triangle, we'll only output 3, and 
// the Raster stage may output hundreds of pixels, each a blended value
// of the initial 3, hence - they "vary"
out vec2 passUV; 
out vec4 passColor;  // NEW - to use it in the pixel stage, we must pass it.

// Entry point - required.  What does this stage do?
void main( void )
{
   // multiply it through - for now, local_pos
   // is being treated as view space position
   // this will be updated later once we introduce the other matrices
    vec4 local_pos = vec4( POSITION, 1 ); 
    vec4 clip_pos =  PROJECTION * (VIEW * local_pos); 
   
    passColor = COLOR; // pass it on. 
    passUV = UV; // we have to set our outs.

    gl_Position = clip_pos;  // changed
})";

	return vs;
}

