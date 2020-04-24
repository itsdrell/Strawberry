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
    for i=1, 5 do 
        xPos = 32 + (i * 32)
        g_allEntities[i].pos = Vector2:Create(xPos, 64)
        g_allEntities[i].rotationAmount = 1
    end

    PlayOneShot("victory.mp3")
end

-------------------------------------------------------------------------
function GameWonScreen:Update(ds)
    if(WasKeyJustReleased("enter")) then GoToState("attract") end
end

-------------------------------------------------------------------------
function GameWonScreen:Draw()
    DrawMap()
    --DrawAllEntities()

    for i=1, 5 do 
        g_allEntities[i]:Draw()
    end

    bounds = GetAABB2FromAABB2(Vector2:Create(.2,.4), Vector2:Create(.8,.6), AABB2:Create(0,0,256,256))
    Berry.DrawAABB2Fill(bounds, "lightGreen")
    Berry.DrawAABB2(bounds, "white")
    DrawText("Victory!", bounds.mins.x + 12, bounds.mins.y + 20, 10)


    bounds = GetAABB2FromAABB2(Vector2:Create(.1,.1), Vector2:Create(.9,.15), AABB2:Create(0,0,256,256))
    Berry.DrawAABB2Fill(bounds, "lightGreen")
    Berry.DrawAABB2(bounds, "white")
    DrawText("Hit Enter to Continue!", bounds.mins.x + 8, bounds.mins.y + 4, 5)

end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

