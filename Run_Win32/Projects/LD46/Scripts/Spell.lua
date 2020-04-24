-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
SpellDefinition = 
{
    name = "idk",
    healingAmount = 10,
    castTime = 0,
    lifeTime = 1,
    onCastFunction = nil,
    onUpdateFunction = nil,
    drawFunction = nil,
}
g_allSpellDefinitions = {}
g_selectedSpells = {}

-------------------------------------------------------------------------
function SpellDefinition:Create(name)
    local newInstance = {}
    setmetatable(newInstance, { __index = SpellDefinition })

    newInstance.name = name

    g_allSpellDefinitions[name] = newInstance
    return newInstance
end

-------------------------------------------------------------------------
function SpellDefinition:CreateAll()
    newInstance = SpellDefinition:Create("Renew")
    newInstance.amount = 1.2
    newInstance.manaCost = 5
    newInstance.castTime = 0
    newInstance.cooldown = 2
    newInstance.lifetime = 5
    newInstance.onCastFunction = nil
    newInstance.onUpdateFunction = HealingOverTime
    newInstance.drawFunction = nil
    newInstance.sprite = 11
    g_selectedSpells[1] = newInstance

    newInstance = SpellDefinition:Create("Heal")
    newInstance.amount = 5
    newInstance.manaCost = 5
    newInstance.castTime = 2
    newInstance.cooldown = 0
    newInstance.lifetime = 0
    newInstance.onCastFunction = SingleTargetHeal
    newInstance.onUpdateFunction = nil
    newInstance.drawFunction = nil
    newInstance.sprite = 12
    g_selectedSpells[2] = newInstance

    newInstance = SpellDefinition:Create("Quick")
    newInstance.amount = 60
    newInstance.manaCost = 10
    newInstance.castTime = 1
    newInstance.cooldown = 2
    newInstance.lifetime = 0
    newInstance.onCastFunction = SingleTargetHeal
    newInstance.onUpdateFunction = nil
    newInstance.drawFunction = nil
    newInstance.sprite = 13
    g_selectedSpells[3] = newInstance

    newInstance = SpellDefinition:Create("Big Heal")
    newInstance.amount = 100
    newInstance.manaCost = 8
    newInstance.castTime = 3.5
    newInstance.cooldown = 0
    newInstance.lifetime = 0
    newInstance.onCastFunction = SingleTargetHeal
    newInstance.onUpdateFunction = nil
    newInstance.drawFunction = nil
    newInstance.sprite = 14
    g_selectedSpells[4] = newInstance

    newInstance = SpellDefinition:Create("AOE")
    newInstance.amount = 30
    newInstance.manaCost = 12
    newInstance.castTime = 2.5
    newInstance.cooldown = 10
    newInstance.lifetime = 0
    newInstance.onCastFunction = AOEHeal
    newInstance.onUpdateFunction = nil
    newInstance.drawFunction = nil
    newInstance.sprite = 15
    g_selectedSpells[5] = newInstance
end

-------------------------------------------------------------------------
-- Spell
-------------------------------------------------------------------------
Spell = 
{
    currentLifetime = 0,
    pos = Vector2:Create(0,0)
}

-------------------------------------------------------------------------
function Spell:Create(definition, target)
    local newInstance = {}
    
    setmetatable(Spell, { __index = Entity})
    setmetatable(newInstance, { __index = Spell})
   
    newInstance.target = target
    newInstance.definition = definition
    newInstance.hasCollision = false

    newInstance.healTimer = 0
    newInstance.hotCooldown = .2

    if newInstance.definition == nil then
        dPrint("fuck",3)
    end

    PlayOneShot("heal.wav")

    Entity:Add(newInstance)
    return newInstance;
end

-------------------------------------------------------------------------
function Spell:Destroy()
    self:Remove()
end

-------------------------------------------------------------------------
function Spell:OnCast()
    --dPrint("cast:" .. self.definition.name, 1)
    if self.definition.onCastFunction then
        self.definition.onCastFunction(self)
    end
end

-------------------------------------------------------------------------
function Spell:Update(ds)
    if self.currentLifetime >= self.definition.lifetime then
        self:Destroy()
        return 
    end
    
    if self.definition.onUpdateFunction then
        self.definition.onUpdateFunction(self, ds)
    end
    
    self.currentLifetime += ds
end

-------------------------------------------------------------------------
function Spell:Draw()
    if self.definition.drawFunction then
        self.definition.drawFunction(self)
    end
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function SingleTargetHeal(spell, ds)
    spell.target:Heal(spell.definition.amount)
end

-------------------------------------------------------------------------
function HealingOverTime(spell, ds)

    if spell.healTimer >= spell.hotCooldown then
        SingleTargetHeal(spell)
        spell.healTimer = 0
    else
        spell.healTimer += ds
    end

end

-------------------------------------------------------------------------
function AOEHeal(spell, ds)
    -- picks the three lowest and heals them
    for member in all(g_theHud.party) do 
        member:Heal(spell.definition.amount) 
    end
end

-------------------------------------------------------------------------
function SetSpellLocationToTarget(spell, ds)
    spell.pos = Vector2:Create(spell.target.pos.x, spell.target.pos.y)
end

-------------------------------------------------------------------------
function DrawAOECircle(spell, ds)
    DrawCircleFill(spell.pos.x, spell.pos.y, 12, 0,128,225,200)
end