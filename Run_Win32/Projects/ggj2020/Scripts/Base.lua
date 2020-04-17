-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_theBase = {}
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Base = {}

function Base:New(x,y)
    Base = setmetatable({}, {__index = Base})
    Base.__index = Base
    
    -- you don't need the Entity New
    local self = setmetatable(Entity:New(x,y), Base)

    self.sprite = 1
    self.x = x
    self.y = y
    self.maxHealth = 100
    self.health = self.maxHealth

    g_theBase = self
    return self
end

function Base:TakeDamage(amount)
    PlayOneShot("BaseDamage.mp3")
    self.health -= amount
end

function Base:Render()
    --DrawAABB2(self.x,self.y, self.x + 16, self.y + 16,"red")
end


-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

