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
    ClearEntityTable()
    self:StartUp()
end

-------------------------------------------------------------------------
function Playing:StartUp()
    Player:Create(64,64)
    PartyMember:CreateAllMembers()
    
    HUD:Create(g_allEntities, g_selectedSpells)
    
    Boss:Create(128,128)
end

-------------------------------------------------------------------------
function Playing:Update(ds)
    g_theHud:Update(ds)
    UpdateAllEntities(ds)
    UpdateAllParticles(ds)

    if WasKeyJustPressed("p") then
        dir = Vector2:Create(RandomRange(-1,1), RandomRange(-1,1))
        SlimeOrb:Create(128,128, dir:Normalize())
    end

    if WasKeyJustPressed("o") then
        HealParticle:Create(g_player)
    end


    if WasKeyJustPressed("k") or g_boss.health <= 0 then GoToState("gameWon") end
    if WasKeyJustPressed("l") or DidWeLoseTheGame() then GoToState("gameLost") end
end

-------------------------------------------------------------------------
function DidWeLoseTheGame()
    if g_player.isDead == true then return true end

    for i=2,5 do 
        if g_allEntities[i].isDead == false then return false end
    end

    return true
end

-------------------------------------------------------------------------
function Playing:Draw()
    DrawAABB2Fill(0,0,256,256,"grey")
    DrawAABB2(0,0,256,256,"grey")
    DrawMap();
    
    DrawAllEntities()
    DrawAllParticles()

    g_theHud:Render()
end
-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

