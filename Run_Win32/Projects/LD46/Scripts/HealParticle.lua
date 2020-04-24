-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
HealParticle = {}
g_allParticles = {}


-------------------------------------------------------------------------
function HealParticle:Create(target)
    local newinst = {}
    setmetatable(newinst, {__index = HealParticle})

    newinst.sprite = 7
    newinst.currentSprite = newinst.sprite
    newinst.animationLength = .1
    newinst.animationTimer = 0

    newinst.lifetime = .3
    newinst.lifetimeTimer = 0

    newinst.target = target

    table.insert(g_allParticles, newinst)
end

-------------------------------------------------------------------------
function HealParticle:Add(particle)
    table.insert(g_allParticles, particle)
end

-------------------------------------------------------------------------
function HealParticle:Remove()
    for k,v in ipairs(g_allParticles) do
        if self == v then
            table.remove(g_allParticles, k)
            self = nil
        end
    end
end

-------------------------------------------------------------------------
function HealParticle:Update(ds)
    if self.animationTimer >= self.animationLength then
        self:UpdateCurrentSprite()
        self.animationTimer = 0
    else
        self.animationTimer += ds
    end

    if self.lifetimeTimer >= self.lifetime then
        self:Remove()
    else
        self.lifetimeTimer += ds
    end
end

-------------------------------------------------------------------------
function HealParticle:UpdateCurrentSprite()
    if self.currentSprite == self.sprite then self.currentSprite = self.sprite + 1 return end
    if self.currentSprite == self.sprite + 1 then self.currentSprite = self.sprite + 2 return end
    if self.currentSprite == self.sprite + 2 then self.currentSprite = self.sprite return end
end

-------------------------------------------------------------------------
function HealParticle:Draw()
    DrawSprite(self.currentSprite, self.target.pos.x, self.target.pos.y + 8)
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function UpdateAllParticles(ds)
    for e in all(g_allParticles) do
        e:Update(ds)
    end
end

-------------------------------------------------------------------------
function DrawAllParticles()    
    for e in all(g_allParticles) do
        e:Draw()
    end  
end

