-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_allEntities = {}
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Entity = 
{
    pos = Vector2:Create(0,0),
    maxHealth = 100,
    health = 100,
    sprite = 0,
    speed = 60,
    name = "idk",
    collisionRadius = 6,
    hasCollision = true,
    isDead = false,
    rotationAmount = 0,
    attackDamage = 30
}

-------------------------------------------------------------------------
function Entity:Add(anEntity, index)
    if index ~= nil then
        g_allEntities[index] = anEntity
    else
        table.insert(g_allEntities, anEntity)
    end
end

-------------------------------------------------------------------------
function Entity:Remove()
    for k,v in ipairs(g_allEntities) do
        if self == v then
            table.remove(g_allEntities, k)
            self = nil
        end
    end
end

-------------------------------------------------------------------------
function Entity:Heal(amount)
    self.health = Clamp(self.health + amount,0, self.maxHealth)
    HealParticle:Create(self)
end

-------------------------------------------------------------------------
function Entity:TakeDamage(amount)
    if self.isDead == true then return end
    
    self.health -= amount

    if self.name ~= "boss" then
        PlayOneShot("damage.wav", .5)
    end

    if self.name == "player" then
        self:Interrupt()
    end

    if self.health <= 0 then 
        self.isDead = true 
        PlayOneShot("death.wav")
    end
end

-------------------------------------------------------------------------
function Entity:Attack()
    self.target:TakeDamage(self.attackDamage)
    self.rotationAmount = 1
end

-------------------------------------------------------------------------
function Entity:AnimateMovement(didWeMove, ds)
    if(didWeMove) then
        self.rotationAmount = Clamp(self.rotationAmount + ds *3, 0, 1)
    else
        self.rotationAmount = Clamp(self.rotationAmount - ds * 3, 0, 1)
    end
end

-------------------------------------------------------------------------
function Entity:CalculateRotationAmount()
    amount = (SinDegrees(g_totalTime * 1000) * self.rotationAmount) * 30
    return amount
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function UpdateAllEntities(ds)
    for e in all(g_allEntities) do
        e:Update(ds)
    end
end

-------------------------------------------------------------------------
function DrawAllEntities()
    if g_currentState == "playing" then g_boss:Draw() end
    
    for e in all(g_allEntities) do
        if e.name ~= "boss" then 
            e:Draw()
        end
    end
    
end

-------------------------------------------------------------------------
function ClearEntityTable()
    for k,v in pairs(g_allEntities) do g_allEntities[k]=nil end
end
