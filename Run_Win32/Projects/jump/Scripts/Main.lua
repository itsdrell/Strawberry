#include "Tiles.lua"
#include "TileDefinition.lua"
#include "Player.lua"
#include "Ball.lua"
#include "ScoreManager.lua"
#include "LevelManager.lua"
-------------------------------------------------------------
-- Globals
-------------------------------------------------------------
totaltime = 0

-------------------------------------------------------------
-- Called once at the start
function StartUp()
    CreateAllTileDefinitions()
    NewGame()
end

------------------------------------------------------------
-- called every frame
function Update(ds)
    CheckIfLevelHasBeenBeaten()
    UpdateBalls(ds)
    Player:Update(ds)
    
    totaltime += ds

    if IsKeyPressed("z") then
        Screenshake(.3)
    end

    if WasKeyJustPressed("n") then
        g_tiles = {}
    end
end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()

    -- background
    DrawAABB2Fill(0,0,256,256,"light grey")
    DrawAABB2Fill(0,0,256,44, "red") -- floor
    DrawAABB2Fill(0,224,256,256, "red") -- roof
    DrawAABB2(0,44,12,256, "green") -- game over boundary

    Player:Render()

    RenderBalls()
    -- tiles
    RenderTiles()

    -- test wall
    DrawAABB2(256,0,258,256,"black")
    DrawAABB2(0,44,12,256,"black")
    DrawAABB2(0,0,256,44, "black")
    DrawAABB2(0,224,256,256,"black")

    g_theScoreManager:Render()

end


-- Good luck <3


