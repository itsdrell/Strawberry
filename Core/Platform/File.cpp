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
#ifndef EMSCRIPTEN_PORT
	// https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi/8233867
	DWORD ftyp = GetFileAttributesA(path);
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!
#endif

	return false;    // this is not a directory!
}

//-----------------------------------------------------------------------------------------------
Strings GetAllFoldersInADirectory(const char* directoryPath)
{
	Strings results;

	// http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
	
	//std::string pattern(directoryPath);
	//pattern.append("\\*");
	////WIN32_FIND_DATA data;
	//LPWIN32_FIND_DATAA data = NULL;
	//
	//HANDLE hFind;
	//if ((hFind = FindFirstFileA(directoryPath, data)) != INVALID_HANDLE_VALUE) {
	//	do {
	//		results.push_back(data->cFileName);
	//	} while (FindNextFileA(hFind, data) != 0);
	//	FindClose(hFind);
	//}

	//std::string pattern(directoryPath);
	//pattern.append("\\*");
	//WIN32_FIND_DATA data;
	//HANDLE hFind;
	//if ((hFind = FindFirstFile( (LPCWSTR) directoryPath, &data)) != INVALID_HANDLE_VALUE) {
	//	do {
	//		results.push_back(String((unsigned char*) data.cFileName[0]));
	//	} while (FindNextFile(hFind, &data) != 0);
	//	FindClose(hFind);
	//}
	//
	
	// TODO
	results.push_back("Pls do this zac");
	return results;
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
