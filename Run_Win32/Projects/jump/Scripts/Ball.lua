-------------------------------------------------------------------------
-- Ball Globals
-------------------------------------------------------------------------
g_balls = {}

-------------------------------------------------------------------------
-- Ball Class
-------------------------------------------------------------------------
Ball = 
{
    x = 0,
    y = 0,
    r = 6,
    xvelocity = 60,
    yvelocity = 60,
    isDead = false
}

-------------------------------------------------------------------------
function Ball:New(x,y)
    local newBall = {}
    newBall.x = x
    newBall.y = y
    
    setmetatable(newBall,self)
    self.__index = self
    
    table.insert(g_balls, newBall)
    return newBall
end

-------------------------------------------------------------------------
function Ball:Delete()
    self:RemoveFromList();

    if #g_balls == 0 then
        g_player.GoToServe()
        LoseLife()
    end
end

-------------------------------------------------------------------------
function Ball:RemoveFromList()
    for k,v in ipairs(g_balls) do
        if self == v then
            table.remove(g_balls, k)
        end
    end
end

-------------------------------------------------------------------------
function Ball:Update(ds)
    self.x += self.xvelocity * ds
    self.y += self.yvelocity * ds

    self:CollisionWithWalls()
    self:CollisionWithTiles()
    self:CollisionWithPlayer()

    if self.isDead then
        self:Delete()
    end
end

-------------------------------------------------------------------------
function Ball:CollisionWithPlayer()
    local maxx = Player.x + Player.width
    local maxy = Player.y + Player.height

    local closestx = Clamp(self.x, Player.x, maxx)
    local closesty = Clamp(self.y, Player.y, maxy)

    if DoesDiscOverlapBox(self.x, self.y, self.r, Player.x, Player.y, maxx, maxy) then
        -- We might have to do a bit of corrective first :(

        amountX, amountY = DiscCorrective(self.x, self.y, self.r, Player.x, Player.y, maxx, maxy)
        normalx, normaly = GetNormalFromTwoPoints(self.x, self.y, closestx, closesty)
        
        self.xvelocity, self.yvelocity = Reflect(self.xvelocity, self.yvelocity, normalx, normaly)
        self.x += amountX
        self.y += amountY

        --Screenshake(.1)
    end
end

-------------------------------------------------------------------------
function Ball:CollisionWithTiles()
    for tile in all(g_tiles) do
        self:CollisionWithATile(tile)
    end
end

-------------------------------------------------------------------------
function Ball:CollisionWithATile(theTile)

    local maxx = theTile.x + theTile.width
    local maxy = theTile.y + theTile.height

    local centerX = theTile.x + ((maxx - theTile.x) * .5)
    local centerY = theTile.y + ((maxy - theTile.y) * .5)

    if DoesDiscOverlapBox(self.x, self.y, self.r, theTile.x, theTile.y, maxx, maxy) then

        normalx, normaly = GetNormalFromTwoPoints(self.x, self.y, centerX, centerY) 
        self.xvelocity, self.yvelocity = Reflect(self.xvelocity, self.yvelocity, normalx, normaly)

        theTile:Hit()
    end
end

-------------------------------------------------------------------------
function Ball:CollisionWithWalls()
    self:CollisionWithAWall(256,0,258,256, 1,0) -- right
    self.isDead = self:CollisionWithAWall(0,44,12,256, -1,0) -- left
    self:CollisionWithAWall(0,0,256,44 ,0,1) -- bottom
    self:CollisionWithAWall(0,224,256,256 ,0,-1) -- top
end

-------------------------------------------------------------------------
function Ball:CollisionWithAWall(minx, miny, maxx, maxy, normalx, normaly)

    if DoesDiscOverlapBox(self.x, self.y, self.r, minx, miny, maxx, maxy) then
        self.xvelocity, self.yvelocity = Reflect(self.xvelocity, self.yvelocity, normalx, normaly)
        
        --Screenshake(.2)
        return true
    end

    return false
end

-------------------------------------------------------------------------
function Ball:Render()
    DrawCircleFill(self.x, self.y, self.r, "random")
    --DrawCircle(self.x, self.y, self.r, "random")
    -- might be cool to have an outline of the previous color
end

-------------------------------------------------------------------------
-- Standalone Ball Functions
-------------------------------------------------------------------------
function UpdateBalls(ds)
    for ball in all(g_balls) do
        ball:Update(ds)
    end
end

-------------------------------------------------------------------------
function RenderBalls()
    for ball in all(g_balls) do
        ball:Render()
    end
end

-------------------------------------------------------------------------
function DoesDiscOverlapBox(discx, discy, radius, minx, miny, maxx, maxy)
    closestx = Clamp(discx, minx, maxx)
    closesty = Clamp(discy, miny, maxy)

    distance = GetDistance(closestx, closesty, discx, discy)

    if distance < radius then
        return true
    end

    return false
end

-------------------------------------------------------------------------
function DiscCorrective(discx, discy, radius, minx, miny, maxx, maxy)
    closestx = Clamp(discx, minx, maxx)
    closesty = Clamp(discy, miny, maxy)

    distance = GetDistance(closestx, closesty, discx, discy)

    displacementx = discx - closestx
    displacementy = discy - closesty

    length = radius - distance

    dirX, dirY = Normalize(displacementx, displacementy)

    amountX = dirX * length
    amountY = dirY * length

    return amountX, amountY
end

-------------------------------------------------------------------------
function Reflect(incomingx, incomingy, surfaceNormalx, surfaceNormaly)
        
    dot = Dot(incomingx, incomingy, surfaceNormalx,surfaceNormaly)

    surfaceNormalLength = Sqrt((surfaceNormalx*surfaceNormalx) + (surfaceNormaly*surfaceNormaly))

    dotTimesSurfaceNormal = dot / surfaceNormalLength
    vnx = dotTimesSurfaceNormal * (surfaceNormalx / surfaceNormalLength) -- surfaceNormalize.x
    vny = dotTimesSurfaceNormal * (surfaceNormaly / surfaceNormalLength) -- surfaceNormalize.y

    resultx = incomingx - (2 * vnx)
    resulty = incomingy - (2 * vny)
    
    return resultx, resulty
end

-------------------------------------------------------------------------
function GetNormalFromTwoPoints(incomingx, incomingy, solidx, solidy)

    xx = incomingx - solidx 
    yy = incomingy - solidy

    displacementx, displacementy = Normalize(xx, yy)

    if Dot(displacementx, displacementy, 1, 0) >= .5 then
        return 1,0
    elseif Dot(displacementx, displacementy, -1, 0) >= .5 then
        return -1,0
    elseif Dot(displacementx, displacementy, 0, 1) >= .5 then 
        return 0,1
    elseif Dot(displacementx, displacementy, 1, 0) >= .5 then
        return 0,-1
    else
        return 1,1
    end
end
