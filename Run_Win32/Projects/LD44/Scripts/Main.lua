-------------------------------------------------------------
-- GLOBAL Variables
-------------------------------------------------------------
maxHealth = 100;
currentHealth = maxHealth;
BOARD_WIDTH = 7;
BOARD_HEIGHT = 6;
TILE_DIMENSION = 16;
CURSOR_COOLDOWN_LENGTH = .1;
COST_PER_MOVE = 1;
COST_PER_SWAP = 5;

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
    TILE_TYPE_ERROR = 69;
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
    cursor.x = 0;
    cursor.y = 0;
    
end

------------------------------------------------------------
-- called every frame
function Update(ds)

    if(eCurrentMode == GameStates.ATTRACT) then UpdateAttract(ds) end
    if(eCurrentMode == GameStates.GAME) then UpdateGame(ds) end
    if(eCurrentMode == GameStates.VICTORY) then UpdateAttract() end
    if(eCurrentMode == GameStates.DEATH) then UpdateDeath() end
end

------------------------------------------------------------
function UpdateAttract(ds)
    
    if(WasKeyJustPressed("space")) then
        eCurrentMode = GameStates.GAME;
        InitGame();
    end

end

------------------------------------------------------------
function UpdateGame(ds) 
    UpdateCursor(ds);
    CheckForDefeat();
end

------------------------------------------------------------
function UpdateDeath()

    if WasKeyJustReleased("space") then
        eCurrentMode = GameStates.ATTRACT;
    end
end

------------------------------------------------------------
function CheckForDefeat()
    if currentHealth <= 0 then
        eCurrentMode = GameStates.DEATH;
    end
end

------------------------------------------------------------
function UpdateCursor(ds)
    if theCursor.movementType == CursorMovementType.MOVING then
        MoveCursor(ds);
    else
        SwapTileMode();
    end
end

------------------------------------------------------------
function MoveCursor(ds)
    if theCursor.CheckCooldown(ds) then

        if(IsKeyPressed("left") or IsKeyPressed("a")) then
            theCursor.x = Clamp(theCursor.x - 1, 0, BOARD_WIDTH - 1);
            DoStuffAfterCursorMoves();
        end

        if(IsKeyPressed("right") or IsKeyPressed("d")) then
            theCursor.x = Clamp(theCursor.x + 1, 0, BOARD_WIDTH - 1);
            DoStuffAfterCursorMoves();
        end

        if(IsKeyPressed("up") or IsKeyPressed("w")) then
            theCursor.y = Clamp(theCursor.y + 1, 0, BOARD_HEIGHT - 1);
            DoStuffAfterCursorMoves();
        end

        if(IsKeyPressed("down") or IsKeyPressed("s")) then
            theCursor.y = Clamp(theCursor.y - 1, 0, BOARD_HEIGHT - 1);
            DoStuffAfterCursorMoves();        
        end
    end

    if(WasKeyJustPressed("enter")) then
        theCursor.movementType = CursorMovementType.SWAPPING;
    end
end

------------------------------------------------------------
function DoStuffAfterCursorMoves()
    theCursor.movementCooldown = CURSOR_COOLDOWN_LENGTH;
    currentHealth = currentHealth - COST_PER_MOVE;
end

------------------------------------------------------------
function SwapTileMode();

    if(WasKeyJustPressed("left") or WasKeyJustPressed("a")) then
        if SwapTile(-1,0) then FinishSwapTile() end     
    end

    if(WasKeyJustPressed("right") or WasKeyJustPressed("d")) then
        if SwapTile(1,0) then FinishSwapTile() end   
    end

    if(WasKeyJustPressed("up") or WasKeyJustPressed("w")) then
        if SwapTile(0,1) then FinishSwapTile() end     
    end

    if(WasKeyJustPressed("down") or WasKeyJustPressed("s")) then
        if SwapTile(0,-1) then FinishSwapTile() end
    end
    
    if(WasKeyJustPressed("backspace")) then
        theCursor.movementType = CursorMovementType.MOVING;
        theCursor.movementCooldown = CURSOR_COOLDOWN_LENGTH;
    end
end

-------------------------------------------------------------
function FinishSwapTile()
    theCursor.movementType = CursorMovementType.MOVING;
    theCursor.movementCooldown = CURSOR_COOLDOWN_LENGTH * 2; 
end

-------------------------------------------------------------
function SwapTile(dirX, dirY)
    tileToSwap = GetTile(theCursor.x + dirX, theCursor.y + dirY);
    currentTile = GetTile(theCursor.x, theCursor.y);

    if tileToSwap.tileType == TypesOfTiles.TILE_TYPE_ERROR then return false end

    local temp = tileToSwap.tileType;
    tileToSwap.tileType = currentTile.tileType;
    currentTile.tileType = temp;

    theCursor.x = theCursor.x + dirX;
    theCursor.y = theCursor.y + dirY;

    return true;    
end

-------------------------------------------------------------
function CreateTileBoard()
    for tileY = 0, BOARD_HEIGHT do
        for tileX = 0, BOARD_WIDTH do
            local tile = {}
            tile.tileType = Floor(RandomRange(0, TypesOfTiles.NUMBER_OF_TILES));
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
    --if(eCurrentMode == GameStates.VICTORY) then RenderAttract() end
    if(eCurrentMode == GameStates.DEATH) then RenderDeath() end


end

------------------------------------------------------------
function RenderAttract()
    DrawText("Attract", -64, 32, 8, "white");
    DrawAABB2(-32,-32,32,32,"white");
end

------------------------------------------------------------
function RenderGame()
    DrawBackground();
    DrawGrid();
    DrawPlayer();
    DrawDay();

    RenderCursor();
end

------------------------------------------------------------
function RenderDeath()
    DrawText("D E A T H", -64, 32, 8, "white");
    DrawAABB2(-100, -100, 100, 100, "green");
end

------------------------------------------------------------
function DrawBackground()
    local maxScreenHeight = 64;
    local currentScreenHeight = Lerp(-maxScreenHeight, maxScreenHeight, (currentHealth / maxHealth));

    DrawAABB2Fill(-96 , -maxScreenHeight, 96, currentScreenHeight, "orangeRed");
end

------------------------------------------------------------
function RenderCursor()
    if theCursor.movementType == CursorMovementType.MOVING then RenderCursorMoving() end
    if theCursor.movementType == CursorMovementType.SWAPPING then RenderCursorSwapping() end
end

------------------------------------------------------------
function RenderCursorMoving()
    local minX, minY, maxX, maxY = GetBoundsForCoords(theCursor.x, theCursor.y);

    --DrawAABB2(minX, minY, maxX, maxY, "black");
    local halfStep = TILE_DIMENSION * .5;
    DrawSprite(0, minX + halfStep, minY + halfStep, 0, 2, 2);
end

------------------------------------------------------------
function RenderCursorSwapping()
    RenderCursorMoving();

    -- left
    if(IsTileCoordsOnLeftEdge(theCursor.x, theCursor.y) == false) then
        local minX, minY, maxX, maxY = GetBoundsForCoords(theCursor.x - 1, theCursor.y);
        DrawAABB2(minX, minY, maxX, maxY, "random");
    end

    -- right
    if(IsTileCoordsOnRightEdge(theCursor.x , theCursor.y) == false) then
        minX, minY, maxX, maxY = GetBoundsForCoords(theCursor.x + 1, theCursor.y);
        DrawAABB2(minX, minY, maxX, maxY, "random");
    end

    -- up
    if(IsTileCoordValid(theCursor.x, theCursor.y + 1)) then
        minX, minY, maxX, maxY = GetBoundsForCoords(theCursor.x, theCursor.y + 1);
        DrawAABB2(minX, minY, maxX, maxY, "random");
    end

    -- down
    if(IsTileCoordValid(theCursor.x , theCursor.y - 1)) then
        minX, minY, maxX, maxY = GetBoundsForCoords(theCursor.x, theCursor.y - 1);
        DrawAABB2(minX, minY, maxX, maxY, "random");
    end
    
end

------------------------------------------------------------
function DrawGrid()
    local startX = theBoard.minX;
    local startY = theBoard.minY;
    local stepX = TILE_DIMENSION;
    local stepY = TILE_DIMENSION;
    
    for gridY = 0, BOARD_HEIGHT - 1 do
        for gridX = 0, BOARD_WIDTH - 1 do
            local tileType = GetTileType(gridX,gridY);
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
    --DrawAABB2Fill(minX, minY, maxX, maxY, GetTileRepresentationForType(tileType));
    local halfStep = TILE_DIMENSION * .5;
    local spriteIndex = GetTileRepresentationForType(tileType);
    DrawSprite(spriteIndex, minX + halfStep, minY + halfStep, 0, 2, 2);   
    
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
-- General Functions
------------------------------------------------------------
function CheckForCombos()


end


------------------------------------------------------------
function GetTileRepresentationForType(tileType)

    if(tileType == TypesOfTiles.HEALTH) then return 2 end
    if(tileType == TypesOfTiles.JUNK) then return 4 end
    if(tileType == TypesOfTiles.HEALTHY_FOOD) then return 6 end
    if(tileType == TypesOfTiles.EMPTY) then return 8 end

    return "magenta";

end

------------------------------------------------------------
function GetTileType(x,y)
    return GetTile(x,y).tileType;
end

------------------------------------------------------------
function GetTile(x,y)
    local index = GetIndexFromCoords(x,y);
    local maxIndex = BOARD_WIDTH * BOARD_HEIGHT

    -- Rizzo says this is gonna be a bug, but she is wrong!
    if index > 0 and index <= maxIndex then
        return tiles[index];
    else
        local tile = {}
        tile.tileType = TypesOfTiles.TILE_TYPE_ERROR;
        return tile;
    end
end

------------------------------------------------------------
function GetIndexFromCoords(x,y)
    return (y * BOARD_WIDTH) + x + 1;
end 

------------------------------------------------------------
function GetBoundsForCoords(tileCoordsX, tileCoordsY)
    
    local boardWidth = theBoard.maxX - theBoard.minX;
    local boardHeight = theBoard.maxY - theBoard.minY;
    local minX = theBoard.minX + ((tileCoordsX / BOARD_WIDTH) * boardWidth);
    local minY = theBoard.minY + ((tileCoordsY / BOARD_HEIGHT) * boardHeight);
    local maxX = minX + TILE_DIMENSION;
    local maxY = minY + TILE_DIMENSION;

    return minX, minY, maxX, maxY;
end

------------------------------------------------------------
function IsTileCoordValid(tileX, tileY)
    
    local index = GetIndexFromCoords(tileX, tileY);
    local maxIndex = (BOARD_WIDTH * BOARD_HEIGHT)

    if index > 0 and index < maxIndex then
        return true;
    else
        return false
    end
end

------------------------------------------------------------
function IsTileCoordsOnEdge(tileX, tileY)

    -- - one cause lua is dumb
    local index = GetIndexFromCoords(tileX, tileY) - 1;
    local modResult = index % BOARD_WIDTH;

    if modResult == 0 or modResult == BOARD_WIDTH - 1 then
        return true;
    end
    
    return false;
end

------------------------------------------------------------
function IsTileCoordsOnLeftEdge(tileX, tileY)

    -- - one cause lua is dumb    
    local index = GetIndexFromCoords(tileX, tileY) - 1;
    local modResult = index % BOARD_WIDTH;

    if modResult == 0 then
        return true;
    end
    
    return false;
end

------------------------------------------------------------
function IsTileCoordsOnRightEdge(tileX, tileY)

    -- - one cause lua is dumb
    local index = GetIndexFromCoords(tileX, tileY) - 1;
    local modResult = index % BOARD_WIDTH;

    if modResult == BOARD_WIDTH - 1 then
        return true;
    end
    
    return false;
end

-- Good luck <3


