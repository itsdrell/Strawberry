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
