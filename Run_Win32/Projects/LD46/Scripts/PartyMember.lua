-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_theTank = {}

-------------------------------------------------------------------------
-- PartyMemberDef
-------------------------------------------------------------------------
PartyMemberDefinition =
{
    name = "idk",
    maxHealth = 100,
    health = 100,
}
g_allPartyMemberDefinitions = {}

-------------------------------------------------------------------------
function PartyMemberDefinition:Create(name)
    local newInstance = {}
    setmetatable(newInstance, {__index = PartyMemberDefinition})

    newInstance.name = name

    g_allPartyMemberDefinitions[name] = newInstance
    return newInstance
end

-------------------------------------------------------------------------
function PartyMemberDefinition:CreateAllDefinitions()

    newDef = PartyMemberDefinition:Create("tank")
    newDef.maxHealth = 500
    newDef.attackCooldown = 3
    newDef.attackDamage = 5
    newDef.updateFunctions =
    {
        normal = DefaultCombatUpdate
    }

    newDef = PartyMemberDefinition:Create("melee")
    newDef.maxHealth = 250
    newDef.attackCooldown = 1
    newDef.attackDamage = 5
    newDef.updateFunctions =
    {
        normal = DefaultCombatUpdate
    }

    newDef = PartyMemberDefinition:Create("ranged")
    newDef.maxHealth = 100
    newDef.attackCooldown = 2
    newDef.attackDamage = 7
    newDef.updateFunctions =
    {
        normal = DefaultCombatUpdate
    }
end

-------------------------------------------------------------------------
-- PartyMember
-------------------------------------------------------------------------
PartyMember = 
{
    cooldownTimer = RandomRange(0,1)
}
g_allPartyMembers = {} -- maybe allies and not player?

-------------------------------------------------------------------------
function PartyMember:Create(x,y, sprite, deathSprite, defAsString)
    local newinst = {}
    
    setmetatable( PartyMember, { __index = Entity } )
    setmetatable( newinst, { __index = PartyMember } )

    newinst.pos = Vector2:Create(x,y)
    newinst.originalSpot = Vector2:Create(x,y)
    newinst.sprite = sprite
    newinst.deathSprite = deathSprite
    newinst.definition = g_allPartyMemberDefinitions[defAsString] 
    newinst.maxHealth = newinst.definition.maxHealth
    newinst.health = newinst.definition.maxHealth

    newinst.attackDamage = newinst.definition.attackDamage

    newinst.randomDir = Vector2:Create(RandomRange(-1,1), RandomRange(-1,1))
    newinst.changeDirectionTimer = 0
    newinst.changeDirectionLength = 2
    
    newinst.target = g_boss

    Entity:Add(newinst, #g_allEntities + 1)
    return newinst
end

-------------------------------------------------------------------------
function PartyMember:CreateAllMembers()
    g_theTank = PartyMember:Create(64,128, 1, 24, "tank")
    PartyMember:Create(172,128, 2, 23, "melee")
    PartyMember:Create(126,200, 3,22, "ranged")
    PartyMember:Create(200,200, 4,21, "ranged")

end

-------------------------------------------------------------------------
function PartyMember:Update(ds)
    if self.isDead == true then return end

    --func = self.definition.updateFunctions[g_combatState]
    --if func ~= nil then func(self, ds) end

    if g_combatState == "normal" then
        DefaultCombatUpdate(self,ds)
    else
        MemberBossCastingSpellUpdate(self, ds)
    end

end

-------------------------------------------------------------------------
function PartyMember:Draw()
    if self.isDead == false then
        DrawSprite(self.sprite, self.pos.x, self.pos.y, self:CalculateRotationAmount())
        DrawSprite(self.sprite + 16, self.pos.x, self.pos.y + 16)
    else
        DrawSprite(self.deathSprite, self.pos.x, self.pos.y)
    end

    
    --DrawCircle(self.pos.x, self.pos.y, self.collisionRadius, "random")
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function DefaultCombatUpdate(member, ds)
    member.target = g_boss
    
    if member.cooldownTimer > member.definition.attackCooldown then
        member:Attack()
        member.cooldownTimer = 0
    else
        member.cooldownTimer += ds
    end

    distance = Vector2:GetDistance(member.pos, member.originalSpot)
    if distance >= 1 then
        dir = member.originalSpot - member.pos 
        member.pos += (dir * ds)

        member:AnimateMovement(true, ds)
    else
        member:AnimateMovement(false, ds)
    end

end

-------------------------------------------------------------------------
function MemberBossCastingSpellUpdate(member, ds)
    previousPos = member.pos
    
    if member.changeDirectionTimer >= member.changeDirectionLength then
        member.randomDir = Vector2:Create(RandomRange(-1,1), RandomRange(-1,1))
        member.changeDirectionTimer = 0
    else
        member.changeDirectionTimer += ds
    end

    member.pos += (member.randomDir * 10 * ds)
        
    -- dont walk off map
    member.pos.x = Clamp(member.pos.x, 32, 220)
    member.pos.y = Clamp(member.pos.y, 32, 220)

    -- dont walk in boss
    if g_boss.collisionBounds:IsPointInside(member.pos) then 
        member.pos = previousPos 
        member.changeDirectionTimer = member.changeDirectionLength
    end

    member:AnimateMovement(true, ds)

end