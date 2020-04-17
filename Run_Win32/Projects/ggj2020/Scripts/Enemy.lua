-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_allEnemies = {}
enemyID = 0
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Enemy = {}

function Enemy:New(x,y)

    local newInstance = {}

    setmetatable( Enemy, {__index = Entity})
    setmetatable(newInstance, {__index = Enemy})

    newInstance.sprite = 2
    newInstance.x = x
    newInstance.y = y
    newInstance.id = enemyID
    enemyID++
    newInstance.attackRange = 16
    newInstance.damageAmountForBase = 5

    table.insert(g_allEnemies, newInstance)

    return newInstance;

end

function Enemy:Delete()
    for k,v in ipairs(g_allEnemies) do
        if self == v then
            table.remove(g_allEnemies, k)
            self = nil
        end
    end
end

function Enemy:Die()
    Explosion:Create(self.x,self.y)
    self:Delete()
end

function Enemy:Update(ds)
    self:CanEnemyAttackBase()
    self:Movement(ds)
end

function Enemy:Movement(ds)
    local tilePosX = Floor(self.x / g_theHeatMap.width)
    local tilePosY = Floor(self.y / g_theHeatMap.width)

    local currentHeat = g_theHeatMap:GetHeatFromTilePos(tilePosX, tilePosY)

    local aboveNeighbor = g_theHeatMap:GetHeatFromTilePos(tilePosX, tilePosY + 1)
    local rightNeighbor = g_theHeatMap:GetHeatFromTilePos(tilePosX + 1, tilePosY)
    local belowNeighbor = g_theHeatMap:GetHeatFromTilePos(tilePosX, tilePosY - 1)
    local leftNeighbor = g_theHeatMap:GetHeatFromTilePos(tilePosX - 1, tilePosY)

    if rightNeighbor < currentHeat then
        if not IsThereAnEntityAtThisWorldPos(self.x + 16, self.y) then
            self.x += 16
        end
    elseif aboveNeighbor < currentHeat then
        if not IsThereAnEntityAtThisWorldPos(self.x, self.y + 16) then
            self.y += 16
        end
    elseif belowNeighbor < currentHeat then
        if not IsThereAnEntityAtThisWorldPos(self.x, self.y - 16) then
            self.y -= 16
        end
    elseif leftNeighbor < currentHeat then
        if not IsThereAnEntityAtThisWorldPos(self.x - 16, self.y) then
            self.x -= 16
        end
    end
end

-------------------------------------------------------------------------
-- returns if the enemy dies
function Enemy:TakeDamage(damageAmount)
    self.health -= damageAmount

    if(self.health < 0) then
        self:Die()
        return true
    end

    return false
end

function Enemy:CanEnemyAttackBase()
    if GetDistance(self.x, self.y, g_theBase.x, g_theBase.y) <= self.attackRange then
        self:Attack()
    end
end

function Enemy:Attack()
    g_theBase:TakeDamage(self.damageAmountForBase)
end

function Enemy:Render()
    DrawSprite (self.sprite, self.x, self.y)
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function UpdateAllEnemies(ds)
    for enemy in all(g_allEnemies) do
        enemy:Update(ds)
    end
end

function RenderAllEnemies(ds)
    for enemy in all(g_allEnemies) do
        enemy:Render()
    end
end

function GetClosestEnemy(x,y)  
    local currentClosestEnemy = nil
    local distance = 999
    for enemy in all(g_allEnemies) do
        local enemyDistance = GetDistance(x,y,enemy.x,enemy.y)
        if enemyDistance < distance then
            currentClosestEnemy = enemy
            distance = enemyDistance
        end
    end

    return currentClosestEnemy, distance
end

function IsThereAnEntityAtThisWorldPos(x,y)
    for currentEnemy in all(g_allEnemies) do
        if(currentEnemy.x == x) and (currentEnemy.y == y) then
            return true
        end
    end

    for curretTurret in all(g_allTurrets) do
        if(curretTurret.x == x) and (curretTurret.y == y) then
            return true
        end
    end

    if(g_theBase.x == x) and (g_theBase.y == y) then
        return true
    end

    return false
end
