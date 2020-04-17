-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Attract = {}
g_Attract = Attract

function Attract:Enter() end

function Attract:Update(ds)
    if(WasKeyJustReleased("enter")) then GoToState("tutorial") end
end

function Attract:Draw()
    DrawAABB2Fill(0,0,256,256,"black")
    DrawMap()
    HideMapTiles()

    DrawAABB2Fill(8,190,250,210,"black")
    DrawAABB2(8,190,250,210,"random")
    DrawText("Untitled Game Jam", 11,196,8,"black")
    DrawText("Untitled Game Jam", 10,196,8)

    DrawAABB2Fill(108,124 - 14,108 + 42,124 + 28,"black")
    DrawAABB2(108,124 - 14,108 + 42,124 + 28,"yellow")
    DrawText("#2",108,124,12)

    DrawAABB2Fill(24, 28, 234, 42,"black")
    DrawAABB2(24,28,234,42,"random")
    DrawText("Press Enter to Start", 28,32,6)
end


-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function HideMapTiles()
    DrawAABB2Fill(160, 176, 176, 192, "black")
    DrawSprite(5,168,184)
    
    DrawAABB2Fill(192, 80, 208, 96, "black")
    DrawSprite(5,200,88)
    
    DrawAABB2Fill(144, 112, 160, 128, "black")
    DrawSprite(5,152,120)
    
    DrawAABB2Fill(80, 80, 96, 96, "black")
    DrawSprite(5,88,88)
    
    DrawAABB2Fill(80, 160, 96, 176, "black")
    DrawSprite(5,88,168)

    DrawAABB2Fill(176, 160, 192, 176, "black")
    DrawSprite(5,184,168)
end
