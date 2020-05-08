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
-- Doesn't seem to work with inheritence??
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
-- Lua Language Helpers
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
function add(t, item)
    table.insert(t, item)
end

--------------------------------------------------------------------------
function remove(t, item)
    table.remove(t, item)
end

--------------------------------------------------------------------------
-- idk if this is safe to call in a forloop
function del(t, item)
    for k,v in ipairs(t) do
        if item == v then
            table.remove(t, k)
            item = nil
        end
    end
end

--------------------------------------------------------------------------
function foreach(theTable, theFunc)
    for item in all(theTable) do 
        theFunc(item)
    end
end

--------------------------------------------------------------------------
-- helper for making classes
-- taken from Game Coding Complete v4
function Class(baseClass, body)
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
function Berry.DrawTextWrapped(string, box, height, alignmentVec2, percentInString, r,g,b,a)
    DrawTextWrapped(string, box.mins.x, box.mins.y, box.maxs.x, box.maxs.y, height, alignmentVec2.x, alignmentVec2.y, percentInString, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawTextOverflow(string, box, height, alignmentVec2, percentInString, r,g,b,a)
    DrawTextOverflow(string, box.mins.x, box.mins.y, box.maxs.x, box.maxs.y, height, alignmentVec2.x, alignmentVec2.y, percentInString, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawTextShrink(string, box, height, alignmentVec2, percentInString, r,g,b,a)
    DrawTextShrink(string, box.mins.x, box.mins.y, box.maxs.x, box.maxs.y, height, alignmentVec2.x, alignmentVec2.y, percentInString, r,g,b,a)
end

-------------------------------------------------------------------------
-- Map
-------------------------------------------------------------------------
function Berry.GetBoundsFromTilePos(tilePos)
	minx = tilePos.x * 16
	miny = tilePos.y * 16
	maxx = (tilePos.x + 1) * 16
	maxy = (tilePos.y + 1) * 16

	return AABB2:Create(Vector2:Create(minx, miny), Vector2:Create(maxx, maxy))
end

-------------------------------------------------------------------------
function Berry.GetTileSprite(tilePos)
    return GetTileSprite(tilePos.x * 16, tilePos.y * 16)
end

-------------------------------------------------------------------------
function Berry.DoesTileHaveSprite(tilePos, sprite)
    return Berry.GetTileSprite(tilePos) == sprite
end

-------------------------------------------------------------------------
function Berry.DoesTileHaveCollision(tilePos, channel)
    return DoesTileHaveCollision(tilePos.x * 16, tilePos.y * 16, channel)
end

-------------------------------------------------------------------------
function Berry.GetNeighborHoodCollisions(disc, currentTile)
    local tilesWithCollision = {} 

    tilePos = currentTile + Vector2:Create(-1,1)
	if Berry.DiscvsTile(disc, tilePos) then add(tilesWithCollision, tilePos) end

	tilePos = currentTile + Vector2:Create(0,1)
	if Berry.DiscvsTile(disc, tilePos) then add(tilesWithCollision, tilePos) end

	tilePos = currentTile + Vector2:Create(1,1)
	if Berry.DiscvsTile(disc, tilePos) then add(tilesWithCollision, tilePos) end

	tilePos = currentTile + Vector2:Create(1,0)
	if Berry.DiscvsTile(disc, tilePos) then add(tilesWithCollision, tilePos) end

	tilePos = currentTile + Vector2:Create(1,-1)
	if Berry.DiscvsTile(disc, tilePos) then add(tilesWithCollision, tilePos) end

	tilePos = currentTile + Vector2:Create(0,-1)
	if Berry.DiscvsTile(disc, tilePos) then add(tilesWithCollision, tilePos) end

	tilePos = currentTile + Vector2:Create(-1,-1)
	if Berry.DiscvsTile(disc, tilePos) then add(tilesWithCollision, tilePos) end

	tilePos = currentTile + Vector2:Create(-1,0)
	if Berry.DiscvsTile(disc, tilePos) then add(tilesWithCollision, tilePos) end

    return tilesWithCollision
end

-------------------------------------------------------------------------
-- Misc things added to namespace
-------------------------------------------------------------------------
Berry.Screenshake = Screenshake 
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
AABB2 = 
{
    mins = {x = 0, y = 0},
    maxs = {x = 1, y = 1}
}

-- allows us to do AABB2(x,y, xx,yy)
setmetatable(AABB2, 
{
    __call = function(theTable, x,y, xx,yy) return AABB2:Create(x,y,xx,yy) end
})

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
    newminx, newminy, newmaxx, newmaxy = AABB2GetAABB2FromAABB2(vec2MinPercentage.x, vec2MinPercentage.y, vec2MaxPercentage.x, vec2MaxPercentage.y, self.mins.x, self.mins.y, self.maxs.x, self.maxs.y)
    
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
 
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Disc =
{
    center = {x = 0, y = 0},
    radius = 1
}

-- allows us to do Disc(x,y,r)
setmetatable(Disc, 
{
    __call = function(theTable, x,y,r) return Disc:Create(x,y,r) end
})

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
Berry.AABB2vsDisc = function(aabb2, disc)
    closestx = Clamp(disc.center.x, aabb2.mins.x, aabb2.maxs.x)
    closesty = Clamp(disc.center.y, aabb2.mins.y, aabb2.maxs.y)

    distance = GetDistance(closestx, closesty, disc.center.x, disc.center.y)

    if distance < disc.radius then
        return true
    end

    return false
end

-------------------------------------------------------------------------
Berry.DiscvsAABB2 = function(disc, aabb2)
    return Berry.AABB2vsDisc(aabb2, disc)
end

-------------------------------------------------------------------------
Berry.AABB2vsAABB2 = function(aabb2A, aabb2B)
	if(aabb2A.maxs.x < aabb2B.mins.x or aabb2A.mins.x > aabb2B.maxs.x) then return false end
	if(aabb2A.maxs.y < aabb2B.mins.y or aabb2A.mins.y > aabb2B.maxs.y) then return false end

	return true;
end

-------------------------------------------------------------------------
Berry.DiscvsDisc = function(discA, discB)
    distance = Vector2:GetDistance(discA.center, discB.center)
    radiusCombined = discA.radius + discB.radius
    return distance < radiusCombined
end

-------------------------------------------------------------------------
Berry.DiscvsTile = function(disc, tilePos)
    return Berry.DiscvsAABB2(disc, Berry.GetBoundsFromTilePos(tilePos))
end

-------------------------------------------------------------------------
Berry.AABB2vsTile = function(disc, tilePos)
    return Berry.DiscvsAABB2(disc, Berry.GetBoundsFromTilePos(tilePos))
end

-------------------------------------------------------------------------
-- Collision math
-------------------------------------------------------------------------
-- This updates the disc as well as returns the position so you can 
-- set a player position to the resulting value
Berry.DiscAABB2Corrective = function(disc, aabb2)
    closestx = Clamp(disc.center.x, aabb2.mins.x, aabb2.maxs.x)
    closesty = Clamp(disc.center.y, aabb2.mins.y, aabb2.maxs.y)

    distance = GetDistance(closestx, closesty, disc.center.x, disc.center.y)

    displacementx = disc.center.x - closestx
    displacementy = disc.center.y - closesty

    length = disc.radius - distance

    dirX, dirY = Normalize(displacementx, displacementy)

    amountX = dirX * length
    amountY = dirY * length

    amount = Vector2:Create(amountX, amountY)
    disc.center = disc.center + amount
    return disc.center
end

-------------------------------------------------------------------------
Berry.DiscCorrective = function(discA, discB, normalizedSecondWeight)
    weight = normalizedSecondWeight or .5

    distance = GetDistance(discA.center.x, discA.center.y, discB.center.x, discB.center.y)
    radiusCombined = discA.radius + discB.radius

    if distance > radiusCombined then return end

    disp = Vector2:Create(discB.center.x - discA.center.x, discB.center.y - discA.center.y)
    disp:Normalize()
    oppDir = disp * -1

    totalPushAmount = radiusCombined - distance
    aAmount = totalPushAmount * weight
    bAmount = totalPushAmount * (1-weight)

    discA.center = discA.center + (oppDir * aAmount)
    discB.center = discB.center + (disp * bAmount)
    return discA.center, discB.center
end

-------------------------------------------------------------------------
Berry.DiscTileCorrective = function(disc, tilePos)
    return Berry.DiscAABB2Corrective(disc, Berry.GetBoundsFromTilePos(tilePos))
end

-------------------------------------------------------------------------
Berry.Reflect = function(incoming, surfaceNormal)
        
    dot = Dot(incoming.x, incoming.y, surfaceNormal.x,surfaceNormal.y)

    surfaceNormalLength = Sqrt((surfaceNormal.x*surfaceNormal.x) + (surfaceNormal.y*surfaceNormal.y))

    dotTimesSurfaceNormal = dot / surfaceNormalLength
    vnx = dotTimesSurfaceNormal * (surfaceNormal.x / surfaceNormalLength) -- surfaceNormalize.x
    vny = dotTimesSurfaceNormal * (surfaceNormal.y / surfaceNormalLength) -- surfaceNormalize.y

    resultx = incoming.x - (2 * vnx)
    resulty = incoming.y - (2 * vny)
    
    return Vector2:Create(resultx, resulty)
end 
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Timer = 
{
    length = 1,
    elapsedTime = 0
}
g_allTimers = {}

-------------------------------------------------------------------------
function Timer:Create(length)
    local newInst = {}
    setmetatable(newInst, {__index = Timer})

    newInst.length = length

    table.insert(g_allTimers, newInst)
    return newInst
end

-------------------------------------------------------------------------
function Timer:SetLength(length)
    self.length = length

    -- assume we should reset it as well?
    self.elapsedTime = 0
end

-------------------------------------------------------------------------
function Timer:Update(ds)
    if self.elapsedTime > self.length then return end 

    self.elapsedTime = self.elapsedTime + ds
end

-------------------------------------------------------------------------
function Timer:HasElapsed()
    return self.elapsedTime >= self.length
end

-------------------------------------------------------------------------
function Timer:GetElapsedTime()
    return self.elapsedTime
end

-------------------------------------------------------------------------
function Timer:GetNormalizedElapsedTime()
    return Clamp(self.elapsedTime / self.length, 0 , 1)
end

-------------------------------------------------------------------------
function Timer:Reset()
    self.elapsedTime = 0
end

-------------------------------------------------------------------------
function Timer:CheckAndReset()
    if self.elapsedTime >= self.length then
        self.elapsedTime = 0
        return true
    end
end

-------------------------------------------------------------------------
function Timer:ToString()
    return (self.elapsedTime .. self.length)
end

-------------------------------------------------------------------------
-- Standalone
-------------------------------------------------------------------------
function UpdateAllTimers(ds)
    for timer in all(g_allTimers) do 
        timer:Update(ds)
    end
end 
-------------------------------------------------------------------------
-- Queue
-------------------------------------------------------------------------
Queue = 
{
    front = 0, 
    back = -1, 
    size = 0
}

-- allows us to do Queue(table)
setmetatable(Queue, 
{
    __call = function(theTable, table) return Queue:Create(table) end
})

-------------------------------------------------------------------------
-- note it defaults to Pushing Left
function Queue:Create(table)
    local newQ = {}
    setmetatable(newQ, {__index = Queue})
  
    newQ.front = 1
    newQ.back = 0
    newQ.size = 0

    if table ~= nil then 
        for entry in all(table) do
            newQ:Push(entry)
        end
    end

    return newQ
end

-------------------------------------------------------------------------
function Queue:Clear()
    for k,v in pairs(self) do
        self[k] = nil
    end

    self = Queue:Create()
end

-------------------------------------------------------------------------
function Queue:Push(value)
  self.back = self.back + 1
  self[self.back] = value
  self.size = self.size + 1
end

-------------------------------------------------------------------------
function Queue:Pop()
    if self.front > self.back then error("Tried to pop a queue when it was empty!") end 
    local value = self[self.front]
    self[self.front] = nil
    self.front = self.front + 1
    self.size = self.size - 1
    return value
end

-------------------------------------------------------------------------
function Queue:Peek() return self[self.front] end

-------------------------------------------------------------------------
function Queue:IsEmpty()
    return self.size == 0
end

-------------------------------------------------------------------------
function Queue:Copy()
    local copy = Berry.Copy(self)
    setmetatable(copy, {__index = Queue })

    return copy
end

-------------------------------------------------------------------------
function Queue:ToString()
    local copy = self:Copy()
    
    if copy:IsEmpty() then return "Empty" end

    local theString = ""
    while copy.size ~= 0 do  
        value = copy:Pop()
        theString = theString .. value .. ","
    end

    return theString
end 



fallingBall = Disc:Create(64,224,6)
acc = Vector2:Create(0, -9.8 * 3)

groundBox = AABB2:Create(0,64,256,72)
solidDisc = Disc:Create(64, 180, 6)

textPos = 0

Player = 
{
	pos = Vector2:Create(72,72),
	collision = Disc:Create(64,64,6),
	
	bounds = 
	{
		center = AABB2:Create(0,0,0,0),
		tl = AABB2:Create(0,0,0,0),
		t = AABB2:Create(0,0,0,0),
		tr = AABB2:Create(0,0,0,0),
		r = AABB2:Create(0,0,0,0),
		br = AABB2:Create(0,0,0,0),
		b = AABB2:Create(0,0,0,0),
		bl = AABB2:Create(0,0,0,0),
		l = AABB2:Create(0,0,0,0),
	},
}
g_player = Player

-------------------------------------------------------------
-- Called once at the start
function StartUp()
	test = Vector2(10,10)
	--dPrint(test:ToString(), 10)

	test = Disc(10,10,5)
	--dPrint(test:ToString(),10)

	test = AABB2(1,2,3,4)
	--dPrint(test:ToString(),10)

end


------------------------------------------------------------
-- called every frame
function Update(ds)

	--g_player.pos = 	--g_player.pos - (Vector2:Create(0,9.8) * ds)
	g_player.collision.center = g_player.pos

	UpdatePos(ds)
	UpdateBounds()
	--Collision()
	CollisionByTiles()
	CollisionByChannel()

	textPos = 	textPos + (ds * .1)
end

------------------------------------------------------------
function UpdatePos(ds)

	speed = 100
	if IsKeyPressed("w") then
		g_player.pos = 		g_player.pos + (Vector2:Create(0,1) * speed * ds)
	end

	if IsKeyPressed("s") then
		g_player.pos = 		g_player.pos + (Vector2:Create(0,-1) * speed * ds)
	end

	if IsKeyPressed("d") then
		g_player.pos = 		g_player.pos + (Vector2:Create(1,0) * speed * ds)
	end

	if IsKeyPressed("a") then
		g_player.pos = 		g_player.pos + (Vector2:Create(-1,0) * speed * ds)
	end
end

------------------------------------------------------------
function CollisionByChannel()
	currentTile = Vector2:Create(Floor(g_player.pos.x / 16), Floor(g_player.pos.y / 16))

	collisions = Berry.GetNeighborHoodCollisions(g_player.collision, currentTile)
	for collision in all(collisions) do 
		if Berry.DoesTileHaveCollision(collision, 1) then 
			g_player.pos = Berry.DiscTileCorrective(g_player.collision, collision)
		end
	end
end

------------------------------------------------------------
function CollisionByTiles()
	currentTile = Vector2:Create(Floor(g_player.pos.x / 16), Floor(g_player.pos.y / 16))
	
	collisions = Berry.GetNeighborHoodCollisions(g_player.collision, currentTile)
	for collision in all(collisions) do 
		if Berry.DoesTileHaveSprite(collision, 81) then 
			g_player.pos = Berry.DiscTileCorrective(g_player.collision, collision)
		end
	end
end

------------------------------------------------------------
function CollisionByBounds()
	
	bound = g_player.bounds.tl
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)

	end

	bound = g_player.bounds.t
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.tr
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.r
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.br
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.b
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.bl
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.l
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	--g_player.pos = g_player.collision.center
end

------------------------------------------------------------
function UpdateBounds()

	currentTile = Vector2:Create(Floor(g_player.pos.x / 16), Floor(g_player.pos.y / 16))

	g_player.bounds.center = Berry.GetBoundsFromTilePos(currentTile)
	g_player.bounds.tl = Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(-1,1))
	g_player.bounds.t 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(0,1))
	g_player.bounds.tr 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(1,1))
	g_player.bounds.r 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(1,0))
	g_player.bounds.br 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(1,-1))
	g_player.bounds.b 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(0,-1))
	g_player.bounds.bl 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(-1,-1))
	g_player.bounds.l 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(-1,0))

end

------------------------------------------------------------
function BounceUpdate(ds)
	fallingBall.center = 	fallingBall.center + (acc * ds)

	if Berry.DiscvsAABB2(fallingBall, groundBox) then
		dPrint("colliding")
		Berry.DiscBoxCorrective(fallingBall,groundBox)
	end

	Berry.DiscCorrective(fallingBall, solidDisc)
end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
	Cls()
	DrawMap()

	Berry.DrawCircle(g_player.collision, "green")
	Berry.DrawAABB2(g_player.bounds.center, "red")

	Berry.DrawAABB2(g_player.bounds.tl  , Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.tl), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.t 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.t ), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.tr 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.tr), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.r 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.r ), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.br 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.br), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.b 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.b ), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.bl 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.bl), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.l 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.l ), "red", "white"))

	-- Hey Emily, \nGo \nfuuuuuuuuuuuuuuuuuck yourself. \nOkay thanks :p
	Berry.DrawAABB2Fill(AABB2(0,0,256,256), "red")
	--Berry.DrawTextWrapped("Hey Emily, \nGo \nfuuuuuuuuuuuuuuuuuck yourself. \nOkay thanks :p", AABB2(0,0,256,256), 8, Vector2(0,0), textPos)
	Berry.DrawTextShrink("fuck\n fuck\n fuck\n fuck\n fuck\n fuck\n fuck\n fuck\n fuck\n fuck\n fuck", AABB2(0,0,256,256), 20, Vector2(1,0), 1)
	Berry.DrawTextWrapped("fuck\n fuck\n fuck\n fuck\n fuck\n fuck\n fuck\n fuck\n fuck\n fuck\n fuck", AABB2(0,0,256,256), 13.1484337, Vector2(0,0), 1)

end

------------------------------------------------------------
function BounceDraws()
	Berry.DrawCircle(fallingBall, "red")
	Berry.DrawAABB2(groundBox, "blue")
	Berry.DrawCircle(solidDisc, "green")
end

------------------------------------------------------------
function OverlapsDraws()
	boxA = AABB2:Create(32,32,48,48)
	boxB = AABB2:Create(50,50,64,64)
	result = Berry.AABB2vsAABB2(boxA, boxB)

	dPrint(Berry.BoolToString(result))
	Berry.DrawAABB2(boxA, "blue")
	Berry.DrawAABB2(boxB, "red")

	boxA = AABB2:Create(70,32,82,48)
	boxB = AABB2:Create(76,42,90,64)
	result = Berry.AABB2vsAABB2(boxA, boxB)

	dPrint(Berry.BoolToString(result))
	Berry.DrawAABB2(boxA, "blue")
	Berry.DrawAABB2(boxB, "red")

	boxA = AABB2:Create(96,32,106,48)
	discA = Disc:Create(112,40,6)
	result = Berry.DiscvsAABB2(discA, boxA)

	dPrint(Berry.BoolToString(result))
	Berry.DrawAABB2(boxA, "blue")
	Berry.DrawCircle(discA, "red")

	boxA = AABB2:Create(112,32,124,48)
	discA = Disc:Create(116,40,6)
	result = Berry.DiscvsAABB2(discA, boxA)

	dPrint(Berry.BoolToString(result))
	Berry.DrawAABB2(boxA, "blue")
	Berry.DrawCircle(discA, "red")
end


-- Good luck <3



