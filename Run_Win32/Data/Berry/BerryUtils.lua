Berry = {}

-------------------------------------------------------------------------
-- Utils functions that wrap exposed c++ functions with 
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
function Berry.DrawTextWrapped(string, box, height, r,g,b,a)
    DrawTextWrapped(string, box.mins.x, box.mins.y, box.maxs.x, box.maxs.y, height, r,g,b,a)
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