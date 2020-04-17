
Berry = {}

displayTimer = 10

-- this doesnt seem to work :l 
-- it might be because I add all includes to the bottom of the script :l 
--zerovec = Vector2:Create(0,0)

-------------------------------------------------------------
-- Called once at the start
function StartUp()
end

------------------------------------------------------------
-- called every frame
function Update(ds)

    if WasKeyJustPressed("space") then
        Vec2Tests()
        DiscUnitTests()
        AABB2Tests()
    end

end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
    Cls()
end

------------------------------------------------------------
function Vec2Tests()
    testVec2 = Vector2:Create(10,20)
    dPrint("testVec2: " .. testVec2, 10)

    addTestVec = testVec2 - testVec2
    dPrint("X is: " .. addTestVec.x .. " Y is: " .. addTestVec.y, displayTimer)


    vec2A = Vector2:Create(0,0)
    vec2B = vec2A
    vec2C = Vector2:Create(1,1)

    resultA = (vec2A == vec2B)
    resultB = (vec2A == vec2C)
    dPrint("Does vec2A == vec2B : " .. Berry.BoolToString(resultA), displayTimer)
    dPrint("Does vec2A == vec2C : " .. Berry.BoolToString(resultB), displayTimer)

    -- uniform scale operand
    vec2A = Vector2:Create(1,1)
    dPrint("Vec2 scaled by 2 is: " .. vec2A * 2, displayTimer)

    -- dot
    vec2A = Vector2:Create(-1,0)
    vec2B = Vector2:Create(1,0)
    dPrint("Dot of vec2A and vec2B is: " .. vec2A * vec2B, displayTimer)

    -- length
    vec2A = Vector2:Create(1,0)
    vec2B = Vector2:Create(.5,.5)
    dPrint("Length Of vecA is: " .. #vec2A, displayTimer)
    dPrint("Length Of vecA is: " .. vec2A:GetLength(), displayTimer)
    dPrint("Length Of vecB is: " .. #vec2B, displayTimer)


end

-------------------------------------------------------------------------
function DiscUnitTests()
    dPrint("____________________", displayTimer)
    dPrint("Disc stuff",displayTimer)
    
    discA = Disc:Create(1,2,3)
    dPrint(discA:ToString(), displayTimer)

    discB = Disc:Create(Vector2:Create(4,5),6)
    dPrint(discB:ToString(), displayTimer)

    -- == 
    discD = discB 
    dPrint("Does discA == discB (shouldnt) : " .. Berry.BoolToString(discA == discB), displayTimer)
    dPrint("Does discD == discB (should) : " .. Berry.BoolToString(discD == discB), displayTimer)

    -- Is point inside
    discA = Disc:Create(1,1,5)
    pointA = Vector2:Create(1,2)
    pointB = Vector2:Create(10,10)
    resultA = discA:IsPointInside(pointA)
    resultB = discA:IsPointInside(pointB.x, pointB.y)
    dPrint("Is pointA in Disc (should): " .. Berry.BoolToString(resultA), displayTimer)
    dPrint("Is pointB in Disc (shouldnt): " .. Berry.BoolToString(resultB), displayTimer)

end

-------------------------------------------------------------------------
function AABB2Tests()

    dPrint("____________________", displayTimer)
    dPrint("AABB2 stuff",displayTimer)

    boxA = AABB2:Create(1,1,2,2)
    boxB = AABB2:Create(Vector2:Create(3,3),4,4)
    boxC = AABB2:Create(5,5, Vector2:Create(6,6))
    boxD = AABB2:Create(Vector2:Create(7,7), Vector2:Create(10,10))

    -- to string
    dPrint(boxA:ToString(), displayTimer)

    -- equal 
    boxE = boxA 
    dPrint("Does boxA == boxB (shouldnt) : " .. Berry.BoolToString(boxA == boxB), displayTimer)
    dPrint("Does boxA == boxE (should) : " .. Berry.BoolToString(boxA == boxE), displayTimer)

    -- width / height
    width = boxD:GetWidth()
    height = boxD:GetHeight()
    dPrint("BoxD width: " .. width .. " height: " .. height, displayTimer)

    -- dimensions
    dim = boxD:GetDimensions()
    dPrint("BoxD dim: " .. dim, displayTimer)

    -- Is point in box
    box = AABB2:Create(0,0,1,1)
    resultA = box:IsPointInside(Vector2:Create(.5,.5))
    resultB = box:IsPointInside(Vector2:Create(100, 100))
    dPrint("Is point inside (should) : " .. Berry.BoolToString(resultA), displayTimer)
    dPrint("Is point inside (shouldnt): " .. Berry.BoolToString(resultB), displayTimer)

    -- Get position inside box
    box = AABB2:Create(0,0,1,1)
    percentIn = Vector2:Create(.5,.5)
    pos = box:GetPositionInside(percentIn)
    dPrint("pos in:" .. pos, displayTimer)

    -- Grow to square
    box = AABB2:Create(0,0,1,2)
    newBox = box:GetBiggerBox()
    dPrint(newBox:ToString(), displayTimer)

    -- shrink to square
    newBox = box:GetSmallerBox()
    dPrint(newBox:ToString(), displayTimer)

    -- Get AABB2
    box = AABB2:Create(1,1,2,2)
    mins = Vector2:Create(.25,.25)
    maxs = Vector2:Create(.75, .75)
    newBox = GetAABB2FromAABB2(mins, maxs, box)
    dPrint(newBox.maxs:ToString(), displayTimer)
end




-- Good luck <3




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

-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

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
-- Globals
-------------------------------------------------------------------------

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
-- Globals
-------------------------------------------------------------------------

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
Berry.BoolToString = function(bool)
    if bool then return "True" end 
    return 'False'
end

-------------------------------------------------------------------------
-- Misc things added to namespace
-------------------------------------------------------------------------
Berry.Screenshake = Screenshake 
