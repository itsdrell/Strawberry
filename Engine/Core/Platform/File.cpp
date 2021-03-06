#include "File.hpp"
#include <iostream>
#include <fstream>

#ifndef EMSCRIPTEN_PORT

//#include "Engine/Core/Platform/Window.hpp"
#include <windows.h>
#include <filesystem>
namespace fs = std::filesystem;

#endif
#include "../Tools/ErrorWarningAssert.hpp"


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
void LogStringsToFile(const char* filename, const Strings& theStrings, bool overwrite /*= false*/)
{
	String stringToLog = "";
	for (String current : theStrings)
	{
		stringToLog += (current + "\n");
	}

	LogStringToFile(filename, stringToLog.c_str(), overwrite);
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

#ifndef EMSCRIPTEN_PORT
		CreateDirectoryA(currentPath.c_str(), NULL);
#endif

		currentPath += "/";
	}

	return currentPath;
}

//-----------------------------------------------------------------------------------------------
bool DoesDirectoryExist(const char* path)
{
#ifndef EMSCRIPTEN_PORT
	// https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi/8233867
	DWORD ftyp = GetFileAttributesA(path);
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!
#endif

	// for web, just return true okay
	return true;   
}

//-----------------------------------------------------------------------------------------------
Strings GetAllFoldersInADirectory(const char* directoryPath)
{
	Strings results;

#ifndef EMSCRIPTEN_PORT
	std::string path = directoryPath;
	for (const auto & entry : fs::directory_iterator(path))
		results.push_back((String)entry.path().string() );
#endif
	return results;
}

//-----------------------------------------------------------------------------------------------
String GetWorkingDirectoryPath()
{
#ifndef EMSCRIPTEN_PORT
	std::filesystem::path cwd = std::filesystem::current_path();
	return cwd.string();
#else
	return "";
#endif
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

//-----------------------------------------------------------------------------------------------
Strings GetAllLinesFromFile(char const* filename)
{
	Strings result;

	std::ifstream infile(filename);

	std::string line;
	while (std::getline(infile, line))
	{
		result.push_back(line);
	}

	infile.close();

	return result;
}

//-----------------------------------------------------------------------------------------------
String GetFileContentAsString(char const* filePath)
{
	PrintLog("Getting FileContentAsString from: " + std::string(filePath));
	
	String result;
	std::ifstream infile(filePath);

	if(!infile.is_open())
	{
		PrintLog("ERROR: Unable to get content from file: " + std::string(filePath));
	}

	char aChar;
	while (infile.get(aChar))
	{
		result.push_back(aChar);
	}

	infile.close();

	return result;
}
