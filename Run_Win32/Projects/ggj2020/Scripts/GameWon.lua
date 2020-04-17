-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
GameWonScreen = {}
g_GameWonScreen = GameWonScreen

-------------------------------------------------------------------------
function GameWonScreen:Enter()
    self.showMenu = false
    self.currentRowIndex = 0
    self.amountOfRows = 16
    self.timer = 0
    self.timeBeforeNextRow = .1

    self:SpawnExplosionOnRow()
end

-------------------------------------------------------------------------
function GameWonScreen:Update(ds)
    
    if self.showMenu then
        if(WasKeyJustReleased("enter")) then g_currentState = "attract" end
    end

    if self.timer > self.timeBeforeNextRow and self.currentRowIndex <= 16 then
        self:SpawnExplosionOnRow()
        self.timer = 0
    else
        self.timer += ds
    end

    if #g_explosions == 0 then
        self.showMenu = true
    end

    UpdateAllExplosions(ds * .5)
end

-------------------------------------------------------------------------
function GameWonScreen:SpawnExplosionOnRow()
    local currenty = 248 - (self.currentRowIndex * 16)
    local currentx = 8

    for i=0,16 do
        local xx = currentx + (i * 16)
        
        Explosion:Create(xx,currenty)
    end

    self.currentRowIndex++
end

-------------------------------------------------------------------------
function GameWonScreen:Draw()
    DrawAABB2Fill(0,0,1000,1000,"black")
    DrawMap();
    
    RenderAllEnemies()
    g_player:Render()
    self:RenderCoveredMap()
    RenderAllExplosions()

    if self.showMenu then 
        self:DrawMenu()
    end
end

-------------------------------------------------------------------------
function GameWonScreen:RenderCoveredMap() 
    local currenty = 248 - (self.currentRowIndex * 16)
    local startingx = 8

    for y=0, self.currentRowIndex -2 do
        local currenty = 248 - (y * 16)
        for x = 0, 16 do
            local currentx = startingx + (x * 16)

            DrawAABB2Fill(currentx - 8, currenty - 8, currentx + 8, currenty + 8, "black")
            DrawSprite(5,currentx,currenty)
        end
    end

end

-------------------------------------------------------------------------
function GameWonScreen:DrawMenu()
    DrawAABB2Fill(256 * .1, 256 * .2, 256 * .9, 256 * .8, "black")
    DrawAABB2(256 * .1, 256 * .2, 256 * .9, 256 * .8, "random")

    DrawText("Help was too", 48, 172, 8, "white")
    DrawText("expensive", 66, 148, 8, "white")
    DrawText("This was", 74, 128, 8, "white")
    DrawText("easier", 90, 104, 8, "white")
        
    DrawText("Hit Enter to continue!", 36, 72,5, "white")
end
-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

