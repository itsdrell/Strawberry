#include "StrawberryFileUtils.hpp"
#include "Engine/Core/Platform/File.hpp"

//===============================================================================================
void GetAllProjectNames(Strings* out)
{
	Strings allFolders = GetAllFoldersInADirectory("Projects");

	for (uint i = 0; i < allFolders.size(); i++)
	{
		String current = allFolders.at(i);
		String theText = String(current, 9);

		out->push_back(theText);
	}
}

//-----------------------------------------------------------------------------------------------
bool DoesProjectHaveCoverImage(const String& projectName)
{
	String path = "Projects/" + projectName + "/Cover.png";
	return DoesDirectoryExist(path.c_str());
}

//-----------------------------------------------------------------------------------------------
String GetCurveDataFilePath()
{
	String path = "Projects/" + g_currentProjectName + "/" + g_currentProjectName + ".curvedata";
	return path;
}
