-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_currentLevel = 1
g_levels = {}

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------

-------------------------------------------------------------------------
function NewGame()
    CreateTiles()
end

-------------------------------------------------------------------------
function CheckIfLevelHasBeenBeaten()
    if #g_tiles == 0 then
        GoToNextLevel()
    end
end

function GoToNextLevel()
    g_currentLevel++
    g_currentLevel = Clamp(g_currentLevel, 1, #g_levels)
    CreateTiles();

    g_balls = {}
    g_player.state = "serving"
end

-------------------------------------------------------------------------
function CreateTiles()
    currentX = 240
    currentY = 220
    xpadding = 4
    ypadding = 4
    i = 1
    for x=1,6 do
        for y=1,6 do
            currentDef = GetDefinitionFromTileIndex(i)

            if currentDef ~= nil then
                Tile:New(currentX - (x * (Tile.width + xpadding)), 
                        currentY - (y * (Tile.height + ypadding)),
                        currentDef)
            end
            i++
        end
    end
end

-------------------------------------------------------------------------
function GetDefinitionFromTileIndex(index)
    local currentLevel = g_levels[g_currentLevel]
    ascii = currentLevel[index]
    def = g_asciiLookupTable[ascii]
    return def
end

-------------------------------------------------------------------------
-- Levels
-------------------------------------------------------------------------
g_levels[1] = 
{
    "1","1","1","1","1","1",
    "1","1","1","1","1","1",
    "1","1","1","1","1","1",
    "1","1","1","1","1","1",
    "1","1","1","1","1","1",
    "1","1","1","1","1","1"
}

g_levels[2] = 
{
    "b","","","","","",
    "","","","","","",
    "","","b","b","","",
    "","","b","b","","",
    "","","","","","",
    "","","","","",""
}

g_levels[3] = 
{
    "1","","","","","1",
    "","2","","","2","",
    "","","3","3","","",
    "","","3","3","","",
    "","2","","","2","",
    "1","","","","","1"
}

g_levels[4] = 
{
    "3","3","3","3","3","3",
    "","","","","","",
    "","2","2","2","2","",
    "","","","","","",
    "","","1","1","","",
    "","","","","",""
}

-- a nice ball tutorial
g_levels[5] = 
{
    "","","b","b","","",
    "","3","3","3","3","",
    "","","","","","",
    "","","","","","",
    "","2","2","2","2","",
    "","","","","",""
}

--[[ 
    template
g_levels[] = 
{
    "","","","","","",
    "","","","","","",
    "","","","","","",
    "","","","","","",
    "","","","","","",
    "","","","","",""
}
--]]

-- fun ones
g_levels["balls"] = 
{
    "b","b","b","b","b","b",
    "b","b","b","b","b","b",
    "b","b","b","b","b","b",
    "b","b","b","b","b","b",
    "b","b","b","b","b","b",
    "b","b","b","b","b","b"
}

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

