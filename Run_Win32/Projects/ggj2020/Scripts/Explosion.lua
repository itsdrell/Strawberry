-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_explosions = {}

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Explosion = {}

function Explosion:Create(x,y)
    local ex = {}
    setmetatable(ex, {__index = Explosion})

    ex.x = x
    ex.y = y
    
    ex.currentSpriteIndex = 1
    ex.spriteIndexAnimations = {32,33,34,35,36,37,38}
    ex.animationLength = .05
    ex.animationTimer = 0

    ex.isDead = false
    ex.rotation = RandomRange(0,90)
    
    table.insert(g_explosions, ex)

    PlayOneShot("Explosion.aiff")
    Screenshake(.05)
    return ex;
end

function Explosion:Remove()
    for k,v in ipairs(g_explosions) do
        if self == v then
            table.remove(g_explosions, k)
            self = nil
        end
    end
end

function Explosion:Update(ds)
    if self.isDead ~= true then
        if self.animationTimer >= self.animationLength then
            self.animationTimer = 0
            self.currentSpriteIndex++

            if self.currentSpriteIndex >= #self.spriteIndexAnimations then
                self.isDead = true
            end
        else
            self.animationTimer += ds
        end
    end
end

function Explosion:Render()
    if self.isDead ~= true then
        DrawSprite(self.spriteIndexAnimations[self.currentSpriteIndex],self.x, self.y, self.rotation)
    end
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function UpdateAllExplosions(ds)
    for ex in all(g_explosions) do
        if ex.isDead == true then
            ex:Remove()
        else
            ex:Update(ds)
        end
    end
end

function RenderAllExplosions()
    for ex in all(g_explosions) do
        ex:Render()
    end
end
