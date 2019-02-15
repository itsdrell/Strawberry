#include "ShaderProgram.hpp"
#include "Engine/Core/Tools/ErrorWarningAssert.hpp"
#include "Engine/Renderer/BuiltInShaders.hpp"

//===============================================================================================
ShaderProgram::ShaderProgram()
{

}

//-----------------------------------------------------------------------------------------------
ShaderProgram::ShaderProgram(const String& name, const char * fs, const char * vs, Strings defines)
{
	m_name = name;

	// Compile the two stages we're using (all shaders will implement the vertex and fragment stages)
	// later on, we can add in more stages;
	GLuint vert_shader = LoadShaderFromLiteral( vs, GL_VERTEX_SHADER , defines); 
	GLuint frag_shader = LoadShaderFromLiteral( fs, GL_FRAGMENT_SHADER , defines ); 

	// Link the program
	// program_handle is a member GLuint. 
	m_programHandle = CreateAndLinkProgram( vert_shader, frag_shader ); 
	glDeleteShader( vert_shader ); 
	glDeleteShader( frag_shader ); 
}

//===============================================================================================
GLuint LoadShaderFromLiteral(const char * shaderSource, GLenum type, Strings defines)
{
	String shader = String(shaderSource);

	// This is not supported for string literals, will crash program
	//uint totalLinesAddedFromInclude = AddIncludesToShaderProgram(shader, std::string("Data/Shaders/glsl"));

	// make sure we only add when we have stuff to do
	if(defines.size() > 0)
	{
		if(defines.at(0) != "") // dont put a space here
		{
			size_t location = shader.find("#version 420 core") + 19;

			for(uint i = 0; i < defines.size(); i++)
			{

				std::string current = "#define ";
				current += defines.at(i);
				current += " \n";

				shader.insert(location, current);

				location += current.length() ;
			}
		}
	}

	// Create a shader
	GLuint shader_id = glCreateShader(type);
	ASSERT_RECOVERABLE(shader_id != NULL, "Could not read Shader");

	// Bind source to it, and compile
	// You can add multiple strings to a shader – they will 
	// be concatenated together to form the actual source object.
	//GLint shader_length = (GLint)strlen(src);
	GLint shader_length = (GLint)shader.length();
	const char* newSrc = shader.c_str();

	glShaderSource(shader_id, 1, &newSrc, &shader_length);
	glCompileShader(shader_id);

	// Check status
	GLint status;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) 
	{
		LogShaderError(shader_id, "Built in engine shader", ((uint) defines.size() * 2)); 
		glDeleteShader(shader_id);

		// If you want to see what the shader looked like
		//LogStringToFile("Data/ShaderLog.txt", shader.c_str());

		Strings blank;

		// Load invalid shader instead
		if(type == GL_VERTEX_SHADER)
			shader_id = LoadShaderFromLiteral(BuiltInShaders::GetInvalidVertex(), GL_VERTEX_SHADER, blank);
		else
			shader_id = LoadShaderFromLiteral(BuiltInShaders::GetInvalidFragment(), GL_FRAGMENT_SHADER, blank);

		//free(src);

		return shader_id;
	}

	//free(src);

	return shader_id;
}

//-----------------------------------------------------------------------------------------------
GLuint CreateAndLinkProgram(GLint vs, GLint fs)
{
	// create the program handle - how you will reference
	// this program within OpenGL, like a texture handle
	GLuint program_id = glCreateProgram();
	ASSERT_RECOVERABLE( program_id != 0 , "Could not create the program handle");

	// Attach the shaders you want to use
	glAttachShader( program_id, vs );
	glAttachShader( program_id, fs );

	// Link the program (create the GPU program)
	glLinkProgram( program_id );

	// Check for link errors - usually a result
	// of incompatibility between stages.
	GLint link_status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);

	if (link_status == GL_FALSE) 
	{
		LogProgramError(program_id);
		glDeleteProgram(program_id);
		program_id = 0;
	} 

	// no longer need the shaders, you can detach them if you want
	// (not necessary)
	glDetachShader( program_id, vs );
	glDetachShader( program_id, fs );

	return program_id;
}

//-----------------------------------------------------------------------------------------------
void LogProgramError(GLuint program_id)
{
	// get the buffer length
	GLint length;
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);

	// copy the log into a new buffer
	char *buffer = new char[length + 1];
	glGetProgramInfoLog(program_id, length, &length, buffer);

	// print it to the output pane
	buffer[length] = NULL;
	DebuggerPrintf("class", buffer);          

	// cleanup
	delete buffer;
}

//===============================================================================================
void LogShaderError(GLuint shader_id, const char* filename, uint defineOffset)
{
	// figure out how large the buffer needs to be
	GLint length;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

	// make a buffer, and copy the log to it. 
	char *buffer = new char[length + 1];
	glGetShaderInfoLog(shader_id, length, &length, buffer);

	// Print it out (may want to do some additional formatting)
	buffer[length] = NULL;

	FormatAndPrintShaderErrors(std::string(buffer), std::string(filename), defineOffset);

	// free up the memory we used. 
	delete buffer;
}

//-----------------------------------------------------------------------------------------------
void FormatAndPrintShaderErrors(std::string log, std::string path, uint defineOffset)
{
	// Title Screen
	DebuggerPrintf("\n---------------------------------------------------- \n");
	String title = String("Errors and warnings for: " + path + "\n");
	DebuggerPrintf(title.c_str());
	DebuggerPrintf(" \n");

	// Format string
	String errors = log;
	String directory = GetDirectoryPath();

	while(errors.length() > 0)
	{

		size_t first = errors.find("(");
		size_t second = errors.find(")");
		String lineNumber = String(errors, first +1, (second - first) - 1 ); // only getting the number here
		errors.erase(first,second);

		//////////////////////////////////////////////////////////////////////////
		// Calculating the offsets due to the define
		int currentLineNumber = std::stoi(lineNumber);
		currentLineNumber -= ((int)defineOffset);
		String finalLineNumber = std::to_string(currentLineNumber);
		//////////////////////////////////////////////////////////////////////////

		size_t endLine = errors.find("(");
		String errorString = String(errors, 3, endLine -5);

		String finalString = directory + path + "(" + finalLineNumber + "):" + errorString;
		DebuggerPrintf(finalString.c_str());
		DebuggerPrintf("\n");

		// erase already parsed lines
		errors.erase(0, endLine);
	}

	DebuggerPrintf("----------------------------------------------------- \n");
}
