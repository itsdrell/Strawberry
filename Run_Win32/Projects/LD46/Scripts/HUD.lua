-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
HUD = 
{
    bounds = GetAABB2FromAABB2(Vector2:Create(.2,.0), Vector2:Create(.8,.2), AABB2:Create(0,0,256,256)),
    castBounds = GetAABB2FromAABB2(Vector2:Create(.3,.21), Vector2:Create(.7,.25), AABB2:Create(0,0,256,256)),
    manaBounds = GetAABB2FromAABB2(Vector2:Create(.15,0), Vector2:Create(.85,.2), AABB2:Create(0,0,256,256)),
    enemyBounds = GetAABB2FromAABB2(Vector2:Create(.2,.93), Vector2:Create(.8,.98), AABB2:Create(0,0,256,256)),
    enemyCastBounds = GetAABB2FromAABB2(Vector2:Create(.25,.88), Vector2:Create(.75,.92), AABB2:Create(0,0,256,256)),

    spells = {},
    party = {},
    spellSlots = {},

    selectedSpellIndex = 1,
    selectedPartyMember = 1,

    showErrorMessage = false,
    popUpCurrentTimer = 0,
    popUpLength = 1,
    errorText = "idk"
}
g_theHud = {}

-------------------------------------------------------------------------
function HUD:Create(party, spellTable)
    local newInst = {}
    setmetatable( newInst, { __index = HUD } )

    newInst:CreateAllBounds()

    newInst:AddParty(party)
    newInst:AddSpells(spellTable)

    g_theHud = newInst
    return newInst
end

-------------------------------------------------------------------------
function HUD:CreateAllBounds()
    self.hpBars = {}
    self.spellBars = {}
    
    for i=1,5 do
        minx = (i - 1) * .2
        maxx = i * .2

        self.hpBars[i] = GetAABB2FromAABB2(Vector2:Create(minx,.5), Vector2:Create(maxx,1), self.bounds)
        self.spellBars[i] = GetAABB2FromAABB2(Vector2:Create(minx,0), Vector2:Create(maxx,.5), self.bounds)
    end
    
end

-------------------------------------------------------------------------
function HUD:AddParty(party)
    for k,v in ipairs(party) do
        self.party[k] = v
    end
end

-------------------------------------------------------------------------
function HUD:AddSpells(spells)
    for k,v in ipairs(spells) do
        self.spells[k] = v
    end

    -- Maybe lets create a struct that holds a CD and the spell def
    -- that way we have a place to store a CD for each slot
    self.spellSlots[1] = { cooldown = 0, spellDef = self.spells[1]}
    self.spellSlots[2] = { cooldown = 0, spellDef = self.spells[2]}
    self.spellSlots[3] = { cooldown = 0, spellDef = self.spells[3]}
    self.spellSlots[4] = { cooldown = 0, spellDef = self.spells[4]}
    self.spellSlots[5] = { cooldown = 0, spellDef = self.spells[5]}

end

-------------------------------------------------------------------------
function HUD:GetSelectedSpellDef()
    return self.spells[self.selectedSpellIndex]
end

-------------------------------------------------------------------------
function HUD:GetSelectedPartyMember()
    return self.party[self.selectedPartyMember]
end

-------------------------------------------------------------------------
function HUD:IsSelectedSpellOnCooldown()
    return self.spellSlots[self.selectedSpellIndex].cooldown > 0
end

-------------------------------------------------------------------------
function HUD:IsTargetDead()
    return self.party[self.selectedPartyMember].isDead
end

-------------------------------------------------------------------------
function HUD:Update(ds)

    if WasMouseButtonJustReleased("l") then
        self:LeftClick()
    end

    if self.showErrorMessage then
        if self.popUpCurrentTimer >= self.popUpLength then
            self.showErrorMessage = false
        else
            self.popUpCurrentTimer += ds 
        end
    end

    self:HandleCooldowns(ds)

    --dPrint(self:GetSelectedSpellDef().name)
end

-------------------------------------------------------------------------
function HUD:HandleCooldowns(ds)
    for slot in all(self.spellSlots) do
        if slot.cooldown > 0 then
            slot.cooldown -= ds
        end
    end
end

-------------------------------------------------------------------------
function HUD:SetCooldownForSelectedSpell(cooldown)
    self.spellSlots[self.selectedSpellIndex].cooldown = cooldown
end

-------------------------------------------------------------------------
function HUD:Interrupt()
    self:ShowCastErrorMessage("Failed :(")
end

-------------------------------------------------------------------------
function HUD:ClearInterrupt()
    self:ClearErrorMessage()
end

-------------------------------------------------------------------------
function HUD:ShowCastErrorMessage(message)
    self.showErrorMessage = true
    self.popUpCurrentTimer = 0
    self.errorText = message
end

-------------------------------------------------------------------------
function HUD:ClearErrorMessage()
    self.showErrorMessage = false
    self.popUpCurrentTimer = 0
    self.errorText = "idk"
end

-------------------------------------------------------------------------
function HUD:IsMouseOnHealthBar()
    allHealthBarsBounds = GetAABB2FromAABB2(Vector2:Create(0,.5), Vector2:Create(1,1), self.bounds)
    return allHealthBarsBounds:IsPointInside(Berry.GetMousePosition())
end

-------------------------------------------------------------------------
function HUD:LeftClick()
    mousePos = Berry.GetMousePosition()
    for i=1,5 do
        if self.hpBars[i]:IsPointInside(mousePos) then
            PlayOneShot("ui.wav")
            self.selectedPartyMember = i
            return
        end

        if self.spellBars[i]:IsPointInside(mousePos) and g_castingSpell == nil then
            PlayOneShot("ui.wav")
            self.selectedSpellIndex = i
            return
        end
    end
end

-------------------------------------------------------------------------
function HUD:Render()
    self:DrawManaBar()
    Berry.DrawAABB2(self.bounds, "white")
    Berry.DrawAABB2Fill(self.bounds, "black")
    
    for i=1,5 do
        self:DrawSpellBox(i, self.spellBars[i])
        self:DrawHPBar(i, self.hpBars[i])
    end
    self:DrawHightlightSelection()
    self:DrawCastBar()
    self:DrawEnemyHealthBar()

    if self.showErrorMessage then
        self:DrawSpellErrorBar()
    end

    --self:DrawBossCastBar("spell", Fract(g_totalTime))
end

-------------------------------------------------------------------------
function HUD:DrawHightlightSelection()
    Berry.DrawAABB2(self.hpBars[self.selectedPartyMember], "random")
    Berry.DrawAABB2(self.spellBars[self.selectedSpellIndex], "random")
end

-------------------------------------------------------------------------
function HUD:DrawHPBar(index, bounds)
    member = self.party[index]
    
    if member ~= nil and member.isDead == false then
        width = Lerp(0,1,(member.health / member.maxHealth))
        healthBar = bounds:GetAABB2FromSelf(Vector2:Create(0,0), Vector2:Create(width, 1))
        
        Berry.DrawAABB2Fill(healthBar,255,0,0,150)
        DrawSprite(member.sprite + 16, healthBar.mins.x + 16, healthBar.mins.y + 12)
    end
    Berry.DrawAABB2(bounds, "white") 
end

-------------------------------------------------------------------------
function HUD:DrawManaBar()
    percent = g_player:GetPercentLeftOfMana()

    manaBar = self.manaBounds:GetAABB2FromSelf(Vector2:Create(0,0), Vector2:Create(1, percent))

    Berry.DrawAABB2Fill(manaBar, "blue")
    Berry.DrawAABB2(self.manaBounds, "white")
end

-------------------------------------------------------------------------
function HUD:DrawSpellBox(index, bounds)
    Berry.DrawAABB2(bounds, "white")

    DrawSprite(self.spellSlots[index].spellDef.sprite, bounds.mins.x + 16, bounds.mins.y + 12)
    
    currentCD = self.spellSlots[index].cooldown
    if currentCD > 0 then
        Berry.DrawAABB2Fill(bounds, 149,165,166, 200)

        if currentCD >= 9 then 
            DrawText(math.floor(currentCD + 1), bounds.mins.x + 7, bounds.mins.y + 10, 5)
        else
            DrawText(math.floor(currentCD + 1), bounds.mins.x + 12, bounds.mins.y + 10, 5)
        end
    end
end

-------------------------------------------------------------------------
function HUD:DrawCastBar()
    if g_castingSpell == nil then return end
    if g_castingSpell.spellDef.castTime == 0 then return end

    percentDone = g_castingSpell:GetPercentDone()
    progressBar = self.castBounds:GetAABB2FromSelf(Vector2:Create(0,0), Vector2:Create(percentDone, 1))

    Berry.DrawAABB2Fill(self.castBounds, "black")
    Berry.DrawAABB2Fill(progressBar, "blue")
    Berry.DrawAABB2(self.castBounds, "white")
    Berry.DrawTextWrapped(g_castingSpell:GetSpellName(), self.castBounds, 4)
end

-------------------------------------------------------------------------
function HUD:DrawSpellErrorBar()
    Berry.DrawAABB2Fill(self.castBounds, "red")
    Berry.DrawAABB2(self.castBounds, "white")
    Berry.DrawTextWrapped(self.errorText, self.castBounds, 3)
end

-------------------------------------------------------------------------
function HUD:DrawEnemyHealthBar()
    Berry.DrawAABB2Fill(self.enemyBounds, "black")
    
    percent = g_boss:GetPercentHealthLeft()
    healthbar = self.enemyBounds:GetAABB2FromSelf(Vector2:Create(0,0), Vector2:Create(percent, 1))
    Berry.DrawAABB2Fill(healthbar, "red")
    
    Berry.DrawAABB2(self.enemyBounds, "white")
    pos = self.enemyBounds:GetPositionInside(Vector2:Create(.35,.25))
    DrawText("Boss", pos.x, pos.y, 6)
end

-------------------------------------------------------------------------
function HUD:DrawBossCastBar(text, percent)
    Berry.DrawAABB2Fill(self.enemyCastBounds, "black")

    fillBar = self.enemyCastBounds:GetAABB2FromSelf(Vector2:Create(0,0), Vector2:Create(percent, 1))
    Berry.DrawAABB2Fill(fillBar, "blue")

    pos = self.enemyCastBounds:GetPositionInside(Vector2:Create(.05, .20))
    DrawText(text, pos.x, pos.y, 4)

    Berry.DrawAABB2(self.enemyCastBounds, "white")
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

