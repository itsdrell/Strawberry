-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Boss = {}
g_boss = {}

-- normal, start_circle_oozes_1, circle_oozes_1, start_beam_oozes_1, beam_oozes_1
g_combatState = "normal"

-------------------------------------------------------------------------
function Boss:Create(x,y)
    local newinst = {}
    
    setmetatable( Boss, { __index = Entity } )
    setmetatable( newinst, { __index = Boss } )

    newinst.sprite = 0
    newinst.pos = Vector2:Create(x,y)
    newinst.maxhealth = 1000
    newinst.health = newinst.maxhealth
    newinst.hasCollision = false
    newinst.name = "boss"

    newinst.normalAttackCooldown = 2
    newinst.normalAttackcooldownTimer = 0
    
    newinst.beamAngle = 0
    newinst.beamAttackCooldown = .5
    newinst.beamAttackTimer = 0

    newinst.circleAttackCooldown = 2
    newinst.circleAttackTimer = newinst.circleAttackCooldown

    newinst.casting = false
    newinst.castTimer = 0
    newinst.castLength = 3

    newinst.phaseTimer = 0
    newinst.finishedSpell = false

    newinst.collisionBounds = AABB2:Create(x - 40, y - 32, x + 40, y + 32)

    newinst.updateFunctions =
    {
        normal = BossDefaultCombatUpdate,
        start_circle_oozes_1 = BossStartCircleOozes1, 
        circle_oozes_1 = BossCircleOozes1,
        start_beam_oozes_1 = BossStartBeamOozes,
        beam_oozes_1 = BossBeamOozes
    }

    -- 1 = 2, 2 = 3, 3 = 4, 4 = 5, 5 = 1
    newinst.attackPriority = {}
    newinst.attackPriority[1] = 2
    newinst.attackPriority[2] = 3
    newinst.attackPriority[3] = 4
    newinst.attackPriority[4] = 5
    newinst.attackPriority[5] = 1

    g_combatState = "normal"

    g_boss = newinst
    Entity:Add(g_boss)
    
    return newinst
end

-------------------------------------------------------------------------
function Boss:Update(ds)
    --self.health = Clamp(self.health - 1, 0, self.maxhealth)
    self.updateFunctions[g_combatState](ds)

    if self.casting then self:UpdateCasting(ds) end
    self:AnimateMovement(false, ds)
    
    --dPrint(g_combatState)
end

-------------------------------------------------------------------------
function Boss:UpdateCasting(ds)
    if self.castTimer >= self.castLength then 
        self:Cast()
        self.castTimer = 0
        self.casting = false
    else
        self.castTimer += ds
    end
end

-------------------------------------------------------------------------
function Boss:Cast()
    if g_combatState == "start_circle_oozes_1" then g_combatState = "circle_oozes_1" end
    if g_combatState == "start_beam_oozes_1" then g_combatState = "beam_oozes_1" end

    g_boss.finishedSpell = false
end

-------------------------------------------------------------------------
function Boss:SelectAttack()
    chance = RandomRange(0,100)
    if chance < 50 then 
        g_combatState = "start_circle_oozes_1"
    else
        g_combatState = "start_beam_oozes_1"
    end

end

-------------------------------------------------------------------------
function Boss:GetPercentHealthLeft()
    return self.health / self.maxhealth
end

-------------------------------------------------------------------------
function Boss:Draw()
    if self.health <= 0 then return end
    
    --DrawCircleFill(self.pos.x, self.pos.y, 24, "red")
    DrawSprite(32, self.pos.x, self.pos.y, self:CalculateRotationAmount(),4,3)

    if self.casting then self:DrawCastBar() end

    if g_combatState == "normal" then
        -- Draw who we are attacking?
        --DrawCircle(self.target.pos.x, self.target.pos.y, 12, "red")
        --DrawCircleFill(self.target.pos.x, self.target.pos.y - 12, 4, "red")
    end

    --Berry.DrawAABB2(self.collisionBounds, "white")
end

-------------------------------------------------------------------------
function Boss:DrawCastBar()
    percent = self.castTimer / self.castLength

    g_theHud:DrawBossCastBar(self.spellName, percent)
end

-------------------------------------------------------------------------
function Boss:GetTarget()
    for i=1, 5 do 
        whoDoWeWant = self.attackPriority[i]
        if g_theHud.party[whoDoWeWant].isDead == false then 
            return g_theHud.party[whoDoWeWant]
        end
    end

    return nil
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function BossDefaultCombatUpdate(ds)
    g_boss.target = g_boss:GetTarget()
    if g_boss.target == nil then return end

    if g_boss.normalAttackcooldownTimer > g_boss.normalAttackCooldown then
        g_boss:Attack()
        --PlayOneShot("slimeAttack.wav", .5)
        g_boss.normalAttackcooldownTimer = 0
    else
        g_boss.normalAttackcooldownTimer += ds
    end


    if g_boss.phaseTimer >= 10 then
        g_boss:SelectAttack()
        g_boss.phaseTimer = 0
    else
        g_boss.phaseTimer += ds
    end
end

-------------------------------------------------------------------------
function BossStartCircleOozes1(ds)
    if g_boss.casting == false then
        g_boss.casting = true
        g_boss.spellName = "Circle Of Oozes"
        g_boss.castTimer = 0
        g_boss.circleAttackTimer = g_boss.circleAttackCooldown
    end
end

-------------------------------------------------------------------------
function BossCircleOozes1(ds)
    if g_boss.circleAttackTimer >= g_boss.circleAttackCooldown then
        SpawnCircleOfOrbs()
        g_boss.circleAttackTimer = 0
    else
        g_boss.circleAttackTimer += ds
    end


    if g_boss.phaseTimer >= 10 then
        g_combatState = "normal"
        g_boss.phaseTimer = 0
    else
        g_boss.phaseTimer += ds
    end
    
end

-------------------------------------------------------------------------
function SpawnCircleOfOrbs()
    currentAngle = RandomRange(0,360)
    for i=0, 12 do 
        direction = Vector2:Create(CosDegrees(currentAngle), SinDegrees(currentAngle))

        SlimeOrb:Create(128,128, direction.x, direction.y)

        currentAngle += 30
    end
end

-------------------------------------------------------------------------
function BossStartBeamOozes(ds)
    if g_boss.casting == false then
        g_boss.casting = true
        g_boss.spellName = "Beam Of Oozes"
        g_boss.castTimer = 0
        g_boss.beamAngle = 0
    end
end

-------------------------------------------------------------------------
function BossBeamOozes(ds)
    if g_boss.beamAttackTimer >= g_boss.beamAttackCooldown then
        g_boss.beamAngle += (ds * 500)

        direction = Vector2:Create(CosDegrees(g_boss.beamAngle), SinDegrees(g_boss.beamAngle))
        SlimeOrb:Create(128,128, direction.x, direction.y)

        direction = Vector2:Create(CosDegrees(g_boss.beamAngle + 90), SinDegrees(g_boss.beamAngle + 90))
        SlimeOrb:Create(128,128, direction.x, direction.y)

        direction = Vector2:Create(CosDegrees(g_boss.beamAngle + 180), SinDegrees(g_boss.beamAngle + 180))
        SlimeOrb:Create(128,128, direction.x, direction.y)

        direction = Vector2:Create(CosDegrees(g_boss.beamAngle + 270), SinDegrees(g_boss.beamAngle + 270))
        SlimeOrb:Create(128,128, direction.x, direction.y)

        g_boss.beamAttackTimer = 0
    else
        g_boss.beamAttackTimer += ds
    end


    if g_boss.beamAngle > 360 then
        g_combatState = "normal"
    end
end
