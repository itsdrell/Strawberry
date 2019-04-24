#include "File.hpp"
#include <iostream>
#include <fstream>

#ifndef EMSCRIPTEN_PORT

//#include "Engine/Core/Platform/Window.hpp"
#include <windows.h>

#endif


//-----------------------------------------------------------------------------------------------
void LogStringToFile(const char* filename, const char* text, bool overwrite /*= false*/)
{
	std::ofstream outputFile;

	// try to open file http://www.cplusplus.com/reference/fstream/fstream/open/
	if (overwrite == false)
		outputFile.open(filename, std::fstream::out | std::fstream::app);
	else
		outputFile.open(filename, std::fstream::out | std::fstream::trunc); // erase the file (for clean up)

	// See if we opened it
	if (outputFile.is_open() == false)
	{
		// this can be called if the file isn't checked out btw :l
		//ERROR_RECOVERABLE("Couldn't log to: " + std::string(filename));
		return;
	}

	std::string log = std::string(text);

	outputFile << text;

	outputFile.close();
}

//-----------------------------------------------------------------------------------------------
String RemoveFileFromDirectoryPath(const char* filepath)
{
	String filePath = String(filepath);
	Strings seperateDirectories = SplitString(filePath, "/");

	String currentPath;
	for (uint i = 0; i < seperateDirectories.size() - 1; i++)
	{
		currentPath += seperateDirectories.at(i);

		CreateDirectoryA(currentPath.c_str(), NULL);

		currentPath += "/";
	}

	return currentPath;
}

//-----------------------------------------------------------------------------------------------
bool DoesDirectoryExist(const char* path)
{
	// https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi/8233867
	DWORD ftyp = GetFileAttributesA(path);
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

//-----------------------------------------------------------------------------------------------
void CreateAndLogStringToFile(const char* filename, const char* text)
{
	String justDirectory = RemoveFileFromDirectoryPath(filename);
	
	// Create only does anything if it doesnt already exist
	CreateADirectory(justDirectory.c_str());
	LogStringToFile(filename, text, true);

	return;
}

//-----------------------------------------------------------------------------------------------
void CreateADirectory(const char* filename)
{
#ifndef EMSCRIPTEN_PORT

	String filePath = String(filename);
	Strings seperateDirectories = SplitString(filePath, "/");

	String currentPath;
	for (uint i = 0; i < seperateDirectories.size(); i++)
	{
		currentPath += seperateDirectories.at(i);

		CreateDirectoryA(currentPath.c_str(), NULL);

		currentPath += "/";
	}

#endif
}
