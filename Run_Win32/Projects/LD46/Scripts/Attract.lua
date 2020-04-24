-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Attract = {}
g_Attract = Attract

-------------------------------------------------------------------------
function Attract:Enter() 
    ClearEntityTable()
    self.spawnTimer = 0
    self.spawnDelay = .5

end

-------------------------------------------------------------------------
function Attract:Update(ds)
    if(WasKeyJustReleased("enter")) then GoToState("playing") end

    if self.spawnTimer >= self.spawnDelay then 
        SlimeOrb:Create(RandomRange(16,240), 256, 0, -1)
        self.spawnTimer = 0
    else
        self.spawnTimer += ds
    end

    UpdateAllEntities(ds)
end

-------------------------------------------------------------------------
function Attract:Draw()
    DrawAABB2Fill(0,0,256,256,"medBlue")
    DrawAABB2(0,0,256,256,"lightGreen")
    
    DrawAllEntities()
    
    --DrawAABB2Fill(8,190,250,210,"medBlue")
    DrawAABB2(8,190,250,210,"lightGreen")
    DrawText("Untitled Game Jam", 11,196,8,"medBlue")
    DrawText("Untitled Game Jam", 10,196,8)
    
    -- aka
    --DrawAABB2Fill(88, 124 - 14,108 + 62, 124 + 28,"medBlue")
    DrawAABB2(88, 124 - 14,108 + 62, 124 + 28,"lightGreen")
    DrawText("#3",108,128,12)
    DrawText("aka LD46", 88,114, 6)
    
    --DrawAABB2Fill(24, 28, 234, 42,"medBlue")
    DrawAABB2(24,28,234,42,"lightGreen")
    DrawText("Press Enter to Start", 28,32,6)
    --DrawText("Attract", 108,128,6, "green")
    
end


-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
