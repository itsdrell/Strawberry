-------------------------------------------------------------------------
--  Vector Class
-------------------------------------------------------------------------
Vector2 = 
{
    x = 0,
    y = 0, 
}

-- allows us to do Vector2(x,y)
setmetatable(Vector2, 
{
    __call = function(theTable, x,y) return Vector2:Create(x,y) end
})

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
