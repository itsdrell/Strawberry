-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
HUD = 
{
    backgroundBounds = {minx = 0, miny = 0, maxx = 256, maxy = 32},
    baseBounds = {minx = 0, miny = 0, maxx = 128, maxy = 16}, 
    turretBounds = {minx = 0, miny = 16, maxx = 256, maxy = 32}, 
    timerBounds = {minx = 128, miny = 0, maxx = 256, maxy = 16},

    lengthOfGame = 300,
    timeLeft = 300
}

function HUD:New()
    g_theHUD = HUD
    g_theHUD.timeLeft = g_theHUD.lengthOfGame
end

function HUD:Update(ds)
    --self.timeLeft -= ds 
    self.timeLeft -= 1 -- one second per move
end

function HUD:Render()
    self:RenderBackground()
    self:RenderBaseUI()
    self:RenderFullTurretUI()
    self:RenderTimer()
end

function HUD:RenderTimer()

    --DrawAABB2(self.timerBounds.minx, self.timerBounds.miny, self.timerBounds.maxx, self.timerBounds.maxy, "random")

    local formattedTime = Floor(self.timeLeft)
    DrawText("Help in: " .. formattedTime, self.timerBounds.minx + 4, self.timerBounds.maxy * .3, 5)
end

function HUD:RenderBackground()
    DrawAABB2Fill(self.backgroundBounds.minx,self.backgroundBounds.miny,self.backgroundBounds.maxx,self.backgroundBounds.maxy, "black")
    DrawAABB2(self.backgroundBounds.minx,self.backgroundBounds.miny,self.backgroundBounds.maxx,self.backgroundBounds.maxy, "white")
end

function HUD:RenderBaseUI()

    healthbar = {
    minx = self.baseBounds.maxx * .4, miny = self.baseBounds.maxy * .2, 
    maxx = self.baseBounds.maxx * .95, maxy = self.baseBounds.maxy * .7 }

    DrawText("Base:", self.baseBounds.minx, self.baseBounds.maxy * .3, 6)

    local lerpHealth = Lerp(healthbar.minx, healthbar.maxx, g_theBase.health / g_theBase.maxHealth)
    DrawAABB2Fill(healthbar.minx,healthbar.miny, healthbar.maxx, healthbar.maxy, "white")
    DrawAABB2Fill(healthbar.minx,healthbar.miny, lerpHealth, healthbar.maxy, "red")
    -- blinking outline??

    -- debug draw bounds
    DrawAABB2(self.baseBounds.minx, self.baseBounds.miny, self.baseBounds.maxx, self.baseBounds.maxy, "white")
end

function HUD:RenderFullTurretUI()

    local allTurretBounds = {}
    
    firstTurretBounds = {
    minx = 0, miny = self.turretBounds.miny, 
    maxx = self.turretBounds.maxx * .2, maxy = self.turretBounds.maxy}

    secondTurretBounds = {
        minx = self.turretBounds.maxx * .2, miny = self.turretBounds.miny, 
        maxx = self.turretBounds.maxx * .4, maxy = self.turretBounds.maxy}

    thirdTurretBounds = {
    minx = self.turretBounds.maxx * .4, miny = self.turretBounds.miny, 
    maxx = self.turretBounds.maxx * .6, maxy = self.turretBounds.maxy}

    fourthTurretBounds = {
        minx = self.turretBounds.maxx * .6, miny = self.turretBounds.miny, 
        maxx = self.turretBounds.maxx * .8, maxy = self.turretBounds.maxy}

    fifthTurretBounds = {
        minx = self.turretBounds.maxx * .8, miny = self.turretBounds.miny, 
        maxx = self.turretBounds.maxx, maxy = self.turretBounds.maxy}

    allTurretBounds[1] = firstTurretBounds
    allTurretBounds[2] = secondTurretBounds
    allTurretBounds[3] = thirdTurretBounds
    allTurretBounds[4] = fourthTurretBounds
    allTurretBounds[5] = fifthTurretBounds
        
    for i = 1, #g_allTurrets do
        HUD:RenderATurretUI(g_allTurrets[i], allTurretBounds[i], false)
    end

    -- debug draw bounds
    --DrawAABB2(self.turretBounds.minx, self.turretBounds.miny, self.turretBounds.maxx, self.turretBounds.maxy, "yellow")

end

function HUD:RenderATurretUI(turret, bounds, debug)

    DrawSprite (turret.sprite, bounds.minx + 8, bounds.maxy -8, 0)

    local startPoint = bounds.minx + ((bounds.maxx - bounds.minx) * .31)

    local healthBarWidth = Lerp(startPoint, bounds.maxx, turret.health / turret.maxHealth)
    DrawAABB2Fill(startPoint, bounds.miny + bounds.maxy * .1, healthBarWidth, bounds.maxy * .9, "red")

    if debug then
        DrawAABB2(bounds.minx, bounds.miny, bounds.maxx, bounds.maxy, "random")
    end
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

