-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_tileDefinitions = {}
g_asciiLookupTable = {}

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
TileDefinition = 
{
    name = "",
    startHealth = 3,
    deathFunction = nil,
    drawFunction = nil,
    asciiRepresentation = "?",
    color = "white"
}

-------------------------------------------------------------------------
function TileDefinition:New(name, representation)
    local tileDefinition = {}
    tileDefinition.name = name
    tileDefinition.asciiRepresentation = representation

    g_tileDefinitions[name] = tileDefinition
    g_asciiLookupTable[representation] = tileDefinition
    return tileDefinition;
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function CreateAllTileDefinitions()
    
    local newDef = TileDefinition:New("1x", "1")
    newDef.startHealth = 1
    newDef.drawFunction = DrawTileBasedOnHealth
    
    local newDef = TileDefinition:New("2x", "2")
    newDef.startHealth = 2
    newDef.drawFunction = DrawTileBasedOnHealth
    
    local newDef = TileDefinition:New("3x", "3")
    newDef.startHealth = 3
    newDef.drawFunction = DrawTileBasedOnHealth
    
    local newDef = TileDefinition:New("ball", "b")
    newDef.startHealth = 1
    newDef.drawFunction = DrawBallTile
    newDef.deathFunction = BallTileDeathFunction
    
end

-------------------------------------------------------------------------
function DrawTileBasedOnHealth(tile)
    if tile.currentHealth  > 2 then
        --DrawAABB2Fill(tile.x + 2, tile.y + 8, tile.x + tile.width - 2, tile.y + tile.height - 8, "red")
        DrawAABB2Fill(tile.x, tile.y, tile.x + tile.width, tile.y + tile.height, "red")
    end
    
    if tile.currentHealth > 1 then
        DrawAABB2Fill(tile.x + 2, tile.y + 2, tile.x + tile.width - 2, tile.y + tile.height - 2,"black")
    end
end

-------------------------------------------------------------------------
function DrawBallTile(tile)
    DrawCircleFill(tile.x + (tile.width * .5), tile.y  + (tile.height * .5), tile.width * .5 - 1, "random")
end

-------------------------------------------------------------------------
function BallTileDeathFunction(tile)
    Ball:New(tile.x, tile.y)
end
