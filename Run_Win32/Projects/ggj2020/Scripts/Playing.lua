-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Playing = {}
g_Playing = Playing

-------------------------------------------------------------------------
function Playing:Enter()
    self:StartUp()
end

-------------------------------------------------------------------------
function Playing:StartUp()
    g_allEnemies = {}
    g_allTurrets = {}

    HeatMap:New(11, 10, 16)
    
    Turret:New(168,184,nil)
    Turret:New(200,88,nil)
    Turret:New(152,120,nil)
    Turret:New(88,168,nil)
    Turret:New(88,88,nil)

    Base:New(184,168)
    HUD:New()
    SpawnManager:New()
    
    g_player = Player:New(168,168)
end

function Playing:Update(ds)
    if(WasKeyJustReleased("w") or WasKeyJustReleased("a") or WasKeyJustReleased("s") or WasKeyJustReleased("d") or WasKeyJustReleased("space")) then
        g_player:Update(ds)
        UpdateAllEnemies(ds)
        UpdateTurrets()
        g_spawnManager:Update(ds) -- idk if this will feel good cause the timer only changes on mouse click
        g_theHUD:Update(ds)
    end

    if WasKeyJustReleased("h") then
        g_theHeatMap:SpreadHeat()
    end

    g_theHeatMap:SpreadHeatIfNeeded()
    
    if g_debug == true then 
        if(WasKeyJustReleased("]")) then GoToState("gameLost") end
        if(WasKeyJustReleased("[")) then GoToState("gameWon") end
        if(WasKeyJustReleased("t")) then Explosion:Create(64,64) end
    end

    if(g_theBase.health <= 0) then
        GoToState("gameLost")
    end

    if(g_theHUD.timeLeft <= 0) then
        GoToState("gameWon")
    end

    UpdateAllExplosions(ds)

end

function Playing:Draw()
    DrawAABB2Fill(0,0,1000,1000,"black")
    DrawMap();

    RenderTurrets();
    RenderAllEnemies()
    g_theBase:Render()
    g_player:Render()
    RenderAllExplosions()

    g_spawnManager:Render()
    g_theHUD:Render()
    --HeatMap:Render()
end
-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

