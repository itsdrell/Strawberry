#include <string>

//====================================================================================
// This is all the native lua helpers I want to append to all lua scripts
//====================================================================================
const char* rawLibraryString = \
R"(
function all(t)
	local i = 0
	local n = #t
	return function()
	         i = i + 1
	         if i <= n then return t[i] end
	       end
end

)";

const std::string g_NativeLuaLibrary = std::string(rawLibraryString);

//====================================================================================
// Written by Zachary Bracken : [12/11/2019]
//====================================================================================

