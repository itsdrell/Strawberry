-------------------------------------------------------------------------
-- Player Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Player Class
-------------------------------------------------------------------------
Player = 
{
    x = 24,
    y = 44,
    width = 16,
    height = 32,
    state = "serving", -- grounded, jumping, serving
    maxJumpHeight = 180,
    startingJumpHeight = 44,
    yVelocity = 0,
    jumpVelocity = 265,
    gravity = 265
}
g_player = Player


ChargeBar = 
{
    min = g_player.startingJumpHeight,
    max = 180,
    strength = 0,
    barHeight = 1
}

-------------------------------------------------------------------------
function Player:Update(ds)
    
    if IsKeyPressed("space") and self.state == "grounded" then
        ChargeBar.strength += ds
    end

    if self.state == "serving" and self:IsGrounded() then
        if WasKeyJustReleased("space") then
            self:ServeBall()
            self.state = "grounded"
        end
    end

    if self.state == "jumping" then
        if self.y == self.startingJumpHeight then
            self.state = "grounded"
            ChargeBar.strength = 0
        end
    end
    
    if WasKeyJustReleased("space") and self.state == "grounded" then
        t = Fract(ChargeBar.strength) + .3
        self.yVelocity = Lerp(0, self.jumpVelocity, t)
        self.state = "jumping"
    end

    -- If you want more tuning you can make self.gravity small again 2.75 and add another
    -- number to multiply against (like 250 or something)
    self.yVelocity = Clamp(self.yVelocity - self.gravity * ds, -self.jumpVelocity, self.jumpVelocity)
    self.y = Clamp(self.y +  self.yVelocity * ds, self.startingJumpHeight, self.maxJumpHeight)
end

-------------------------------------------------------------------------
function Player:Render()
    self:DrawEnergyBar()
    
    DrawAABB2(self.x, self.y, self.x + self.width, self.y + self.height)

    if self.state == "serving" and self.y == self.startingJumpHeight then
        DrawCircleFill(self.x + self.width, self.y + self.height, 6, "random")
    end

end

-------------------------------------------------------------------------
function Player:GoToServe()
    g_player.state = "serving"
    ChargeBar.strength = 0
end

-------------------------------------------------------------------------
function Player:DrawEnergyBar()
    
    if Fract(ChargeBar.strength) < .1 or self:IsGrounded() == false then
        return
    end

    currenty = Lerp(ChargeBar.min, ChargeBar.max, Fract(ChargeBar.strength))

    -- "player"
    DrawAABB2(
        g_player.x, 
        currenty, 
        g_player.x + g_player.width, 
        currenty + g_player.height, 
        "grey")

    -- dotted width bar
    local startX = g_player.x + g_player.width
    local stepSize = 6
    local barWidth = 6
    for i=0,20 do 
        DrawAABB2Fill(startX, currenty + g_player.height, 
        startX + barWidth, currenty + g_player.height + 1, "green")

        startX += (barWidth + stepSize)
    end

end

-------------------------------------------------------------------------
function Player:ServeBall()
    Ball:New(self.x + self.width + 6, self.y + self.height)
end

-------------------------------------------------------------------------
function Player:IsGrounded()
    return self.y == self.startingJumpHeight
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
