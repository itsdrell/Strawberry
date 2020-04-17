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

--------------------------------------------------------------------------
-- helper for making classes
-- taken from Game Coding Complete v4
function class(baseClass, body)
    local ret = body or {};
    
    -- if there’s a base class, attach our new class to it
    if (baseClass ~= nil) then
        setmetatable(ret, ret);
        ret.__index = baseClass;
    end
    
    -- Add the Create() function (third param must be nil)
    ret.Create = function(self, constructionData, originalSubClass)
        local obj;
        if (self.__index ~= nil) then
            if (originalSubClass ~= nil) then
                obj = self.__index:Create(constructionData, originalSubClass);
            else
                obj = self.__index:Create(constructionData, self);
            end
        else
            obj = constructionData or {};
        end

        setmetatable(obj, obj);
        obj.__index = self;
        
        -- copy any operators over
        if (self.__operators ~= nil) then
            for key, val in pairs(self.__operators) do
                obj[key] = val;
            end
        end
        
        return obj;
    end
    
    return ret;
end

)";

const std::string g_NativeLuaLibrary = std::string(rawLibraryString);

//====================================================================================
// Written by Zachary Bracken : [12/11/2019]
//====================================================================================

