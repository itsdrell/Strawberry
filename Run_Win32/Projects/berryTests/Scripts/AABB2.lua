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
