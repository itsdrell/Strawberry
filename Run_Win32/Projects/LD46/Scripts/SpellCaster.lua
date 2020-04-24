-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
SpellCaster = 
{
    currentCastTime = 0,
}
g_castingSpell = nil

-------------------------------------------------------------------------
function SpellCaster:Create(spellDefName, target, caster)
    local newInstance = {}
    setmetatable(newInstance, { __index = SpellCaster})

    newInstance.target = target
    newInstance.caster = caster
    newInstance.spellDef = g_allSpellDefinitions[spellDefName]
    newInstance.hasCollision = false

    if newInstance.spellDef == nil then
        dPrint("Can't find spell with name:" .. spellDefName, 3)
    end

    return newInstance
end

-------------------------------------------------------------------------
function SpellCaster:Update(ds)
    if self.spellDef.castTime == 0 or self.currentCastTime >= self.spellDef.castTime then
        self:Cast()
    else
        self.currentCastTime += ds
    end
end

-------------------------------------------------------------------------
function SpellCaster:Cast()
    castSpell = Spell:Create(self.spellDef, self.target)
    castSpell:OnCast()

    self.caster.mana -= self.spellDef.manaCost
    g_theHud:SetCooldownForSelectedSpell(self.spellDef.cooldown)
    
    g_castingSpell = nil
end

-------------------------------------------------------------------------
function SpellCaster:GetPercentDone()
    return (self.currentCastTime / self.spellDef.castTime)
end

-------------------------------------------------------------------------
function SpellCaster:GetSpellName()
    return self.spellDef.name
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

