-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_allTurrets = {}
TurretSprites = {}
TurretSprites[1] = 16
TurretSprites[2] = 17
TurretSprites[3] = 18
TurretSprites[4] = 19
TurretSprites[5] = 20

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Turret = {}

function Turret:New(x,y,def)

    local newinst = {}

    setmetatable( Turret, { __index = Entity:New(x,y) } )

    setmetatable( newinst, { __index = Turret } )
    
    -- you don't need the Entity New
    newinst.x = x
    newinst.y = y
    newinst.maxHealth = 100
    newinst.health = newinst.maxHealth --RandomRange(0,100)
    newinst.shootCost = 5
    newinst.attackRange = 3 * 16
    newinst.damage = 100
    newinst.target = nil
    newinst.poweredOn = true
    
    table.insert(g_allTurrets, newinst)
    newinst.sprite = TurretSprites[#g_allTurrets]

    return newinst
end

function Turret:Update(ds)
    if self.target ~= nil then
        if GetDistance(self.x, self.y, self.target.x, self.target.y) > self.attackRange then
            self.target = nil
        else
            self:Shoot(self.target)
        end
    end

    if self.target == nil then
        self:FindTarget()
    end

    if self.health <= 0 and self.poweredOn == true then
        self.poweredOn = false
        PlayOneShot("TurretPowerDown.wav")
    end
    
end

function Turret:FindTarget()
    if (self.health > 0) and (#g_allEnemies > 0) then
        target,distance = GetClosestEnemy(self.x, self.y)
        if distance < self.attackRange then 
            self.target = target
        end
    end
end

function Turret:Shoot(enemy)
    if enemy:TakeDamage(self.damage) then
        self.target = nil
    end
    self.health -= self.shootCost
    
    PlayOneShot("Shoot.wav")
end

function Turret:Repair(amount)
    if self.health < self.maxHealth then
        PlayOneShot("Repair.wav")
        self.poweredOn = true
    end

    self.health = Clamp(self.health + amount, 0, self.maxHealth)
end

function Turret:Render()
    if self.target ~= nil and self.health > 0 then
        DrawLine(self.x, self.y, self.target.x,self.target.y, "random")
    end
    
    if self.health > 0 then DrawCircleFill(self.x, self.y, 48, 255,0,0,25) end
    DrawSprite (self.sprite, self.x, self.y)

    --DrawText(self.health, self.x, self.y + 16, 5)
end



-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function UpdateTurrets(ds)
    for turret in all(g_allTurrets) do
        turret:Update(ds)
    end

end

function RenderTurrets()
    for turret in all(g_allTurrets) do
        turret:Render()
    end
end
