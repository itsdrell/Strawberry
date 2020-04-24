-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Player = 
{
}
g_player = {}

-------------------------------------------------------------------------
function Player:Create(x,y)
    local newinst = {}
    
    setmetatable( Player, { __index = Entity } )
    setmetatable( newinst, { __index = Player } )

    newinst.sprite = 0
    newinst.deathSprite = 25
    newinst.pos = Vector2:Create(x,y)
    newinst.maxHealth = 100
    newinst.health = newinst.maxHealth
    
    newinst.maxMana = 100
    newinst.mana = newinst.maxMana
    newinst.manaRegen = .1
    newinst.name = "player"

    g_player = newinst
    Entity:Add(g_player, #g_allEntities + 1)
    return newinst
end

-------------------------------------------------------------------------
function Player:Update(ds)
    if self.isDead == true then return end

    didWeMove = self:UpdateMovement(ds)

    if g_castingSpell ~= nil then
        if didWeMove or self:DidWeCancelSpell() then
            self:Interrupt()
        else
            g_castingSpell:Update(ds)
        end
    elseif WasMouseButtonJustReleased("l") then
        if self:CanCast() then
            g_castingSpell = SpellCaster:Create(g_theHud:GetSelectedSpellDef().name, g_theHud:GetSelectedPartyMember(), self)
            g_theHud:ClearInterrupt()
        elseif g_theHud:IsMouseOnHealthBar() == true then
            self:DisplayWhyWeCantCast()
        end
    end

    self.mana = Clamp(self.mana + self.manaRegen, 0, self.maxMana)

    self:AnimateMovement(didWeMove, ds)
end

-------------------------------------------------------------------------
function Player:DisplayWhyWeCantCast()
    if self:HasEnoughManaToCast() == false then
        g_theHud:ShowCastErrorMessage("Not enough mana!")
    end
    
    if g_theHud:IsSelectedSpellOnCooldown() then
        g_theHud:ShowCastErrorMessage("On Cooldown!")
    end

    if g_theHud:IsTargetDead() then
        g_theHud:ShowCastErrorMessage("they ded")
    end

    -- todo dead (LOS)
end

-------------------------------------------------------------------------
function Player:CanCast()
    return self:HasEnoughManaToCast() and g_theHud:IsMouseOnHealthBar() and (not g_theHud:IsSelectedSpellOnCooldown()) and (not g_theHud:IsTargetDead())
end

-------------------------------------------------------------------------
function Player:HasEnoughManaToCast()
    spellDef = g_theHud:GetSelectedSpellDef()
    return spellDef.manaCost <= self.mana
end

-------------------------------------------------------------------------
function Player:DidWeCancelSpell()
    return WasMouseButtonJustReleased("l") or WasMouseButtonJustReleased("r") or WasKeyJustReleased("space")
end

-------------------------------------------------------------------------
function Player:UpdateMovement(ds)
    
    dir = Vector2:Create(0,0)
    if IsKeyPressed("w") or IsKeyPressed("up") then
        dir += Vector2:Create(0,1)
    end

    if IsKeyPressed("s") or IsKeyPressed("down") then
        dir += Vector2:Create(0,-1)
    end

    if IsKeyPressed("a") or IsKeyPressed("left") then
        dir += Vector2:Create(-1,0)
    end
    
    if IsKeyPressed("d") or IsKeyPressed("right") then
        dir += Vector2:Create(1,0)
    end

    self.pos += (dir * self.speed * ds)

    if dir == Vector2:Create(0,0) then return false end
    return true
end

-------------------------------------------------------------------------
function Player:Interrupt()
    g_castingSpell = nil
    g_theHud:Interrupt()
    PlayOneShot("ErrorCast.wav")
end

-------------------------------------------------------------------------
function Player:GetPercentLeftOfMana()
    return self.mana / self.maxMana
end

-------------------------------------------------------------------------
function Player:Draw()
    if self.isDead == false then
        DrawSprite(self.sprite, self.pos.x, self.pos.y, self:CalculateRotationAmount())
        DrawSprite(self.sprite + 16, self.pos.x, self.pos.y + 16)
    else
        DrawSprite(self.deathSprite, self.pos.x, self.pos.y)
    end

    
    --DrawCircle(self.pos.x, self.pos.y, self.collisionRadius, "green")
end
-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

