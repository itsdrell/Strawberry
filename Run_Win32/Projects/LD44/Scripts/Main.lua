-------------------------------------------------------------
-- GLOBAL Variables
-------------------------------------------------------------
maxHealth = 100;
currentHealth = maxHealth;
BOARD_WIDTH = 7;
BOARD_HEIGHT = 6;
TILE_DIMENSION = 16;
CURSOR_COOLDOWN_LENGTH = .1;

-------------------------------------------------------------
-- Enums
-------------------------------------------------------------
TypesOfTiles =
{
    HEALTH = 0;
    HEALTHY_FOOD = 1;
    JUNK = 2;
    EMPTY = 3;
    
    NUMBER_OF_TILES = 4;
};

-------------------------------------------------------------
GameStates =
{
    ATTRACT = 0;
    GAME = 1;
    VICTORY = 2;
    DEATH = 3;
};
eCurrentMode = GameStates.ATTRACT;

-------------------------------------------------------------
CursorMovementType = 
{
    MOVING = 0;
    SWAPPING = 1;
};

-------------------------------------------------------------
-- Objects
-------------------------------------------------------------
theCursor = 
{
    x = 0;
    y = 0;
    movementType = CursorMovementType.MOVING;
    movementCooldown = CURSOR_COOLDOWN_LENGTH;
}

-------------------------------------------------------------
function theCursor.CheckCooldown(ds)
    if(theCursor.movementCooldown <= 0) then 
        return true;
    else
        theCursor.movementCooldown = theCursor.movementCooldown - ds;
    end
    return false;
end

-------------------------------------------------------------
theBoard = 
{
    minX = -32;
    minY = -48;
    maxX = -32 + (BOARD_WIDTH * TILE_DIMENSION);
    maxY = -48 + (BOARD_HEIGHT * TILE_DIMENSION);
}

-------------------------------------------------------------
-- Global Tables
-------------------------------------------------------------
tiles = {}

-------------------------------------------------------------
-- Called once at the start
function StartUp()
    InitGame();    

end

-------------------------------------------------------------
function InitGame()
    currentHealth = maxHealth;
    CreateTileBoard();
end

------------------------------------------------------------
-- called every frame
function Update(ds)

    if(eCurrentMode == GameStates.ATTRACT) then UpdateAttract(ds) end
    if(eCurrentMode == GameStates.GAME) then UpdateGame(ds) end
    if(eCurrentMode == GameStates.VICTORY) then UpdateAttract() end
    if(eCurrentMode == GameStates.DEATH) then UpdateAttract() end
end

------------------------------------------------------------
function UpdateAttract(ds)
    
    if(WasKeyJustPressed("enter")) then
        eCurrentMode = GameStates.GAME;
        InitGame();
    end

end

------------------------------------------------------------
function UpdateGame(ds)
    
    
    if theCursor.CheckCooldown(ds) then UpdateCursor() end

end

------------------------------------------------------------
function UpdateCursor()
    if theCursor.movementType == CursorMovementType.MOVING then
        MoveCursor();
    else
        SwapTile();
    end
end

------------------------------------------------------------
function MoveCursor()
    if(IsKeyPressed("left")) then
        theCursor.x = Clamp(theCursor.x - 1, 0, BOARD_WIDTH - 1);
        theCursor.movementCooldown = CURSOR_COOLDOWN_LENGTH;
    end

    if(IsKeyPressed("right")) then
        theCursor.x = Clamp(theCursor.x + 1, 0, BOARD_WIDTH - 1);
        theCursor.movementCooldown = CURSOR_COOLDOWN_LENGTH;
    end

    if(IsKeyPressed("up")) then
        theCursor.y = Clamp(theCursor.y + 1, 0, BOARD_HEIGHT - 1);
        theCursor.movementCooldown = CURSOR_COOLDOWN_LENGTH;
    end

    if(IsKeyPressed("down")) then
        theCursor.y = Clamp(theCursor.y - 1, 0, BOARD_HEIGHT - 1);
        theCursor.movementCooldown = CURSOR_COOLDOWN_LENGTH;        
    end
end

------------------------------------------------------------
function SwapTile();

end

-------------------------------------------------------------
function CreateTileBoard()
    for tileY = 1, BOARD_HEIGHT do
        for tileX = 1, BOARD_WIDTH do
            local tile = Floor(RandomRange(0, TypesOfTiles.NUMBER_OF_TILES));
            local index = (tileY * BOARD_WIDTH) + tileX;
            tiles[index] = tile;
            --table.insert(tiles, tile);
        end
    end 
end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
    Cls();

    if(eCurrentMode == GameStates.ATTRACT) then RenderAttract() end
    if(eCurrentMode == GameStates.GAME) then RenderGame() end
    if(eCurrentMode == GameStates.VICTORY) then RenderAttract() end
    if(eCurrentMode == GameStates.DEATH) then UpdateAttract() end


end

------------------------------------------------------------
function RenderAttract()
    DrawText("Attract", -64, 32, 8, "white");
    DrawAABB2(-32,-32,32,32,"white");
end

------------------------------------------------------------
function RenderGame()
    DrawGrid();
    DrawPlayer();
    DrawDay();

    RenderCursor();
end

------------------------------------------------------------
function RenderCursor()
    local boardWidth = theBoard.maxX - theBoard.minX;
    local boardHeight = theBoard.maxY - theBoard.minY;
    local minX = theBoard.minX + ((theCursor.x / BOARD_WIDTH) * boardWidth);
    local minY = theBoard.minY + ((theCursor.y / BOARD_HEIGHT) * boardHeight);
    local maxX = minX + TILE_DIMENSION;
    local maxY = minY + TILE_DIMENSION;

    DrawAABB2(minX, minY, maxX, maxY, "black");

end

------------------------------------------------------------
function DrawGrid()
    local startX = theBoard.minX;
    local startY = theBoard.minY;
    local stepX = TILE_DIMENSION;
    local stepY = TILE_DIMENSION;
    
    for gridX = 1, BOARD_HEIGHT do
        for gridY = 1, BOARD_WIDTH do
            local tileType = GetTileType(gridY,gridX);
            DrawTile(startX, startY, startX + stepX, startY + stepY, tileType);
            startX = startX + stepX;
        end
        
        startX = theBoard.minX;
        startY = startY + stepY;
    end
    
    DrawAABB2(theBoard.minX, theBoard.minY, theBoard.maxX, theBoard.maxY, "red");
end

------------------------------------------------------------
function DrawTile(minX, minY, maxX, maxY, tileType)
    DrawAABB2Fill(minX, minY, maxX, maxY, GetTileRepresentationForType(tileType));
    --DrawAABB2Fill(minX, minY, maxX, maxY, 0);    
    DrawAABB2(minX, minY, maxX, maxY, "darkGrey");
end

------------------------------------------------------------
function DrawPlayer()

    local startX = -80;
    local startY = -48;
    local width = 16;
    local height = 32;

    DrawAABB2Fill(startX, startY, startX + width, startY + height, "Red");
end

------------------------------------------------------------
function DrawDay()

    local x = -80;
    local y = 48;
    
    DrawText("Day 1", x , y, 4, "white");

end

------------------------------------------------------------
function GetTileRepresentationForType(tileType)

    if(tileType == TypesOfTiles.HEALTH) then return "white" end
    if(tileType == TypesOfTiles.JUNK) then return "brown" end
    if(tileType == TypesOfTiles.HEALTHY_FOOD) then return "yellow" end
    if(tileType == TypesOfTiles.EMPTY) then return "grey" end

    return "mustard";

end


------------------------------------------------------------
function GetTileType(x,y)
    local index = (y * BOARD_WIDTH) + x;
    return tiles[index];
end

-- Good luck <3


