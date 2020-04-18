#include <string>

//====================================================================================
// This is all the native lua helpers I want to append to all lua scripts
//====================================================================================
const char* rawLibraryString = \
R"(
Berry = {}

-------------------------------------------------------------------------
-- Utils functions that wrap exposed c = c + 1 functions with 
-- util data structures
-------------------------------------------------------------------------
-------------------------------------------------------------------------
-- ternary?? select/Pick/iif
-- I think some people call it iff
-- if valueIfTrue is false or nil, the expression will fall through and choose valueIfFalse.
function Pick(condition, valueIfTrue, valueIfFalse)
    return condition and valueIfTrue or valueIfFalse
end
Berry.Pick = Pick

-------------------------------------------------------------------------
-- shallow copy
Berry.Copy = function (oldTable)
    local newTable = {}
    for k,v in pairs(oldTable) do
        newTable[k] = v
    end
    return newTable
end

-------------------------------------------------------------------------
Berry.BoolToString = function(bool)
    if bool then return "True" end 
    return 'False'
end

--------------------------------------------------------------------------
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
    
    -- if there's a base class, attach our new class to it
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

-------------------------------------------------------------------------
-- Input
-------------------------------------------------------------------------
function Berry.GetMousePosition()
	xpos,ypos = GetMousePosition()
	
	return Vector2:Create(xpos,ypos) 
end

-------------------------------------------------------------------------
-- Rendering
-------------------------------------------------------------------------
function Berry.DrawLine(vec2startPos, vec2endPos, r, g, b, a)
    DrawLine(vec2startPos.x, vec2startPos.y, vec2endPos.x, vec2endPos.y, r, g, b, a)
end

-------------------------------------------------------------------------
function Berry.DrawCircle(disc, r,g,b,a)
    DrawCircle(disc.center.x, disc.center.y, disc.radius, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawCircleFill(disc, r,g,b,a)
    DrawCircleFill(disc.center.x, disc.center.y, disc.radius, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawAABB2(box, r,g,b,a)
    DrawAABB2(box.mins.x, box.mins.y, box.maxs.x, box.maxs.y, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawAABB2Fill(box, r,g,b,a)
    DrawAABB2Fill(box.mins.x, box.mins.y, box.maxs.x, box.maxs.y, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawSprite(index, pos, rotation, width, height, flipX, flipY, pixelsPerUnit)
    DrawSprite(index, pos.x, pos.y, rotation, width, height, flipX, flipY, pixelsPerUnit)
end

-------------------------------------------------------------------------
function Berry.DrawText(string, pos, height, r,g,b,a)
    DrawText(string, pos.x, pos.y, height, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawTextWrapped(string, box, height, r,g,b,a)
    DrawTextWrapped(string, box.mins.x, box.mins.y, box.maxs.x, box.maxs.y, height, r,g,b,a)
end

-------------------------------------------------------------------------
-- Misc things added to namespace
-------------------------------------------------------------------------
Berry.Screenshake = Screenshake 
-------------------------------------------------------------------------
-- Adding to the namespace berry
-------------------------------------------------------------------------
Berry.Abs = Abs
Berry.ATan2 = ATan2
Berry.CosDegrees = CosDegrees
Berry.SinDegrees = SinDegrees
Berry.Floor = Floor
Berry.Ceil = Ceil
Berry.Min = Min
Berry.Max = Max
Berry.Sqrt = Sqrt
Berry.Clamp = Clamp
Berry.RandomRange = RandomRange
Berry.Lerp = Lerp
Berry.Chance = Chance
Berry.Fract = Fract
Berry.RangeMap = RangeMap

-------------------------------------------------------------------------
-- Overlap functions for AABB2 and Disc pls
-------------------------------------------------------------------------
-- todo 
-------------------------------------------------------------------------
--  Vector Class
-------------------------------------------------------------------------
Vector2 = 
{
    x = 0,
    y = 0,   
}

-------------------------------------------------------------------------
function Vector2:Create(x,y)
    local newInstance = {}
    setmetatable( newInstance, 
    { 
        __index = Vector2,
        __add = AddTwoVectors,
        __sub = SubtractTwoVectors,
        __mul = MultiplyVector, -- Does either dot, or uniform scale based off of the other variable
        __eq = DoTwoVectorsEqual,
        __len = LengthOfVector,
        __concat = Vector2ConcatWithString
    })

    -- if you add a copy constructor, dont just set them equal to another
    -- since thats a reference and not a copy

    newInstance.x = x
    newInstance.y = y

    return newInstance
end

-------------------------------------------------------------------------
function Vector2:Normalize()
    return Normalize(self.x, self.y)
end

-------------------------------------------------------------------------
function Vector2:GetLength()
    return GetLength(self.x, self.y)
end

-------------------------------------------------------------------------
function Vector2:GetLengthSquared()
    return GetLengthSquared(self.x, self.y)
end

-------------------------------------------------------------------------
function Vector2:ToString()
    return "X is: " .. self.x .. " Y is: " .. self.y
end

-------------------------------------------------------------------------
function Vector2:Dot(vectorB)
    return Dot(self.x, self.y, vectorB.x, vectorB.y)
end

-------------------------------------------------------------------------
function Vector2:GetDistance(startVec, endVec)
    return GetDistance(startVec.x, startVec.y, endVec.x, endVec.y)
end

-------------------------------------------------------------------------
function Vector2:GetDistanceSquared(startVec, endVec)
    return GetDistanceSquared(startVec.x, startVec.y, endVec.x, endVec.y)
end

-------------------------------------------------------------------------
-- Operators
-------------------------------------------------------------------------
function AddTwoVectors(vecA, vecB)
    return Vector2:Create(vecA.x + vecB.x, vecA.y + vecB.y)
end

-------------------------------------------------------------------------
function SubtractTwoVectors(vecA, vecB)
    return Vector2:Create(vecA.x - vecB.x, vecA.y - vecB.y)
end

-------------------------------------------------------------------------
function Vector2ConcatWithString(theString, vec)
    return theString .. vec:ToString()
end

-------------------------------------------------------------------------
function DotProduct(vecA, vecB)
    return vecA:Dot(vecB)
end

-------------------------------------------------------------------------
function UniformScaleVector(vecA, scale)
    local result = Vector2:Create(0,0)
    result.x = vecA.x * scale
    result.y = vecA.y * scale 
    return result
end

-------------------------------------------------------------------------
-- Note this only works with the vector one the right. Ex: Vector * <another vector or scale>
function MultiplyVector(vecA, other)
    if type(other) == "table" then 
        return DotProduct(vecA, other)
    else
        return UniformScaleVector(vecA, other)
    end
end

-------------------------------------------------------------------------
function DoTwoVectorsEqual(vecA, vecB)
    return (vecA.x == vecB.x) and (vecA.y == vecB.y)
end

-------------------------------------------------------------------------
function LengthOfVector(vec)
    return vec:GetLength()
end
 
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Disc =
{
    center = {x = 0, y = 0},
    radius = 1
}

-------------------------------------------------------------------------
function Disc:Create(input1, input2, input3)
    local newInstance = {}
    setmetatable( newInstance, 
    { 
        __index = Disc,
        __eq = DoTwoDiscsEqual,
        __concat = Vector2ConcatWithString
    })

    -- if you add a copy constructor, dont just set them equal to another
    -- since thats a reference and not a copy
    
    -- figure out if we are using a vec2 to create or x,y,r
    -- gotta do this cause lua doesn't support overloading
    if type(input1) == "table" then
        newInstance.center = Vector2:Create(input1.x, input1.y)
        newInstance.radius = input2
    else
        newInstance.center = Vector2:Create(input1,input2)
        newInstance.radius = input3
    end

    return newInstance
end

-------------------------------------------------------------------------
function Disc:IsPointInside(input1,input2)
    if type(input1) == "table" then
        return IsPointInsideDisc(self.center.x, self.center.y, self.radius, input1.x, input1.y)
    else
        return IsPointInsideDisc(self.center.x, self.center.y, self.radius, input1, input2)
    end
end

-------------------------------------------------------------------------
function Disc:ToString()
    return "Center.x is: " .. self.center.x .. " Center.y is: " .. self.center.y .. " Radius is: " .. self.radius
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function DiscConcactWithString(theString, disc)
    return theString .. disc:ToString()
end

-------------------------------------------------------------------------
function DoTwoDiscsEqual(discA, discB)
    return ((discA.center == discB.center) and (discA.radius == discB.radius))
end
 
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
AABB2 = 
{
    mins = {x = 0, y = 0},
    maxs = {x = 1, y = 1}
}

-------------------------------------------------------------------------
-- YOU CAN NOT JUST ASSIGN THEM TO MINS,MAXS IF ITS A VECTOR
-- BECAUSE ITS NOT A COPY, ITS A REFERENCE AND IF YOU CHANGE THE MINS
-- YOU WILL CHANGE THE VALUES YOU PASSED IN DURING CONSTRUCTION AS WELL
-- think of them as needing to be const
function AABB2:Create(input1, input2, input3, input4)
    local newInstance = {}
    setmetatable( newInstance, 
    { 
        __index = AABB2,
        __eq = DoesAABB2EqualAABB2,
        __concat = AABB2ConcactWithString
    })

    -- if you add a copy constructor, dont just set them equal to another
    -- since thats a reference and not a copy

    if type(input1) == "table" and type(input2) == "table" then 
        newInstance.mins = Vector2:Create(input1.x, input1.y) 
        newInstance.maxs = Vector2:Create(input2.x, input2.y)
    
    elseif type(input1) == "table" and type(input2) ~= "table" then
        newInstance.mins = Vector2:Create(input1.x, input1.y)
        newInstance.maxs = Vector2:Create(input2, input3)

    elseif type(input1) ~= "table" and type(input3) == "table" then
        newInstance.mins = Vector2:Create(input1, input2)
        newInstance.maxs = Vector2:Create(input3.x, input3.y)

    else
        newInstance.mins = Vector2:Create(input1, input2)
        newInstance.maxs = Vector2:Create(input3,input4)
    end

    return newInstance
end

-------------------------------------------------------------------------
function AABB2:GetWidth()
    return Abs(self.maxs.x - self.mins.x)
end

-------------------------------------------------------------------------
function AABB2:GetHeight()
    return Abs(self.maxs.y - self.mins.y)
end

-------------------------------------------------------------------------
function AABB2:GetDimensions()
    return Vector2:Create(self:GetWidth(), self:GetHeight())
end

-------------------------------------------------------------------------
function AABB2:GetPositionInside(vec2Percent)
    posx,posy = AABB2GetPositionInside(self.mins.x, self.mins.y, self.maxs.x, self.maxs.y, vec2Percent.x, vec2Percent.y)

    return Vector2:Create(posx, posy)
end

-------------------------------------------------------------------------
function AABB2:IsPointInside(vec2Point)
    return AABB2IsPointInside(self.mins.x, self.mins.y, self.maxs.x, self.maxs.y, vec2Point.x, vec2Point.y)
end

-------------------------------------------------------------------------
function AABB2:GrowToSquare()
    self.mins.x, self.mins.y, self.maxs.x, self.maxs.y = AABB2GrowToSquare(self.mins.x, self.mins.y, self.maxs.x, self.maxs.y)
end

-------------------------------------------------------------------------
function AABB2:GetBiggerBox()
    copyOfSelf = AABB2:Create(self.mins, self.maxs)
    copyOfSelf:GrowToSquare()
    return copyOfSelf
end

-------------------------------------------------------------------------
function AABB2:ShrinkToSquare()
    self.mins.x, self.mins.y, self.maxs.x, self.maxs.y = AABB2ShrinkToSquare(self.mins.x, self.mins.y, self.maxs.x, self.maxs.y)
end

-------------------------------------------------------------------------
function AABB2:GetSmallerBox()
    copyOfSelf = AABB2:Create(self.mins, self.maxs)
    copyOfSelf:ShrinkToSquare()
    return copyOfSelf
end

-------------------------------------------------------------------------
function AABB2:GetAABB2FromSelf(vec2MinPercentage, vec2MaxPercentage)
    newminx, newminy, newmaxx, newmaxy = AABB2GetAABB2FromAABB2(self.mins.x, self.mins.y, self.maxs.x, self.maxs.y, vec2MinPercentage.x, vec2MinPercentage.y, vec2MaxPercentage.x, vec2MaxPercentage.y)

    return AABB2:Create(newminx, newminy,newmaxx, newmaxy)
end

-------------------------------------------------------------------------
function AABB2:ToString() 
    local stringmin = "Mins.x is: " .. self.mins.x .. " Mins.y is: " .. self.mins.y
    local stringmax = "Maxs.x is: " .. self.maxs.x .. " Maxs.y is: " .. self.maxs.x
    return stringmin .. " " .. stringmax
end

-------------------------------------------------------------------------
-- Operators
-------------------------------------------------------------------------
function DoesAABB2EqualAABB2(boxA, boxB)
    return (boxA.mins == boxB.mins) and (boxA.maxs == boxB.maxs)
end

-------------------------------------------------------------------------
function AABB2ConcactWithString(theString, box)
    return theString .. box:ToString()
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function GetAABB2FromAABB2(vec2MinPercentage, vec2MaxPercentage, theBounds)
    return theBounds:GetAABB2FromSelf(vec2MinPercentage,vec2MaxPercentage)
end

)";

const std::string g_NativeLuaLibrary = std::string(rawLibraryString);

//====================================================================================
// Written by Zachary Bracken : [12/11/2019]
//====================================================================================

