#pragma once
#include "Engine/Core/Utils/StringUtils.hpp"

//====================================================================================
// Standalone C Functions
//====================================================================================
void CreateAndLogStringToFile(const char* filename, const char* text);
void CreateADirectory(const char* filename);
void LogStringToFile(const char* filename, const char* text, bool overwrite = false);
String RemoveFileFromDirectoryPath(const char* filepath);
bool DoesDirectoryExist(const char* path);
Strings GetAllFoldersInADirectory(const char* directoryPath);
String GetWorkingDirectoryPath();
Strings GetAllLinesFromFile(char const* filename);


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [4/23/2019]
//====================================================================================