#include "Vector2.lua"
#include "Disc.lua"
#include "AABB2.lua"
#include "BerryUtils.lua"

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
    dPrint(newBox:ToString(), displayTimer)
end




-- Good luck <3


