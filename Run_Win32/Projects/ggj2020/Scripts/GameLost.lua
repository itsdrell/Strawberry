-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
GameLostScreen = 
{

}
g_GameLostScreen = GameLostScreen

-------------------------------------------------------------------------
function GameLostScreen:Enter()
    self.timer = 0;
    self.explosionDelay = .1
    self.spawnedExplosionCount = 0
    self.amountOfExplosionsToSpawn = 8
    self.showMenu = false
    
    Explosion:Create(184,168)

    self.explosionLocations = {}
    self.explosionLocations[1] = {x = 192, y =176}
    self.explosionLocations[2] = {x = 176, y =176}
    self.explosionLocations[3] = {x = 176, y =160}
    self.explosionLocations[4] = {x = 192, y =160}

end

-------------------------------------------------------------------------
function GameLostScreen:Update(ds)

    if self.showMenu then
        if(WasKeyJustReleased("enter")) then g_currentState = "attract" end
    end
    
    if self.timer > self.explosionDelay then
        self.timer = 0

        if self.spawnedExplosionCount <= self.amountOfExplosionsToSpawn then
            self:AddExplosion()
        end
    else
        self.timer += ds
    end

    if #g_explosions == 0 then
        self.showMenu = true
    end

    UpdateAllExplosions(ds * .5)
end

-------------------------------------------------------------------------
function GameLostScreen:AddExplosion()
    index = Floor(RandomRange(1, 5))
    
    xx = self.explosionLocations[index].x
    yy = self.explosionLocations[index].y

    Explosion:Create(xx,yy)

    self.spawnedExplosionCount++
end

-------------------------------------------------------------------------
function GameLostScreen:Draw()
    DrawAABB2Fill(0,0,1000,1000,"black")
    DrawMap();

    g_player:Render()
    DrawAABB2Fill(176, 160, 192, 176, "black")
    DrawSprite(5,184,168)
    
    RenderAllEnemies()
    RenderAllExplosions()

    if self.showMenu then 
        self:DrawMenu()
    end
end

function GameLostScreen:DrawMenu()
    
    
    DrawAABB2Fill(256 * .1, 256 * .4, 256 * .9, 256 * .6, "black")
    DrawAABB2(256 * .1, 256 * .4, 256 * .9, 256 * .6, "random")

    DrawText("The Base has fallen", 32, 140, 6, "white")
        
        
    DrawText("Hit Enter to try again!", 32, 112,5, "white")
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

