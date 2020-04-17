-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
SpawnManager =
{
    spawnRate = {min = 0, max = 0},
    bounds = {minx = 8, miny = 8, maxx = 248, maxy = 248 },
    spawnCooldown = 10
}
g_spawnManager = {}

function SpawnManager:New()
    g_spawnManager = SpawnManager
end

function SpawnManager:Update(ds)
    if self.spawnCooldown <= 0 then
        self:SpawnEnemy()
        self.spawnCooldown = RandomRange(self.spawnRate.min, self.spawnRate.max)
    else
        self.spawnCooldown -= 1
        --dPrint("spawnCooldown: " .. self.spawnCooldown, 2)
    end
end

function SpawnManager:SpawnEnemy()
    local x, y = GetPointOnBoundsOutline(self.bounds)
    Enemy:New(x,y)
end

function SpawnManager:Render()
    DrawAABB2(self.bounds.minx, self.bounds.miny, self.bounds.maxx, self.bounds.maxy, "random")
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function GetPointOnBoundsOutline(bounds)

    local diceRoll = RandomRange(0,100)
    local resultx = 1
    local resulty = 1

    if(diceRoll < 25) then -- left side
        resulty = 8 + (Floor(RandomRange(0, 15)) * 16)
        resultx = 8
    end

    if(diceRoll > 25 and diceRoll <= 50) then -- right side
        resulty = 8 + (Floor(RandomRange(0, 15)) * 16)
        resultx = (16 * 16) - 8
    end

    if (diceRoll > 50 and diceRoll <= 75) then
        resultx = 8 + (Floor(RandomRange(0, 15)) * 16)
        resulty = 8
    end

    if diceRoll > 75 then
        resultx = 8 + (Floor(RandomRange(0, 15)) * 16)
        resulty = (16 * 16) - 8
    end
    
    return resultx, resulty
end
