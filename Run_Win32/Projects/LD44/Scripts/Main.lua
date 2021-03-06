
-------------------------------------------------------------
-- Constants
-------------------------------------------------------------
BOARD_WIDTH = 7;
BOARD_HEIGHT = 6;
TILE_DIMENSION = 16;
CURSOR_COOLDOWN_LENGTH = .1;
START_POP_COOLDOWN_LENGTH = 0;
GAME_POP_COOLDOWN_LENGTH = .3;
POP_COOLDOWN_LENGTH = START_POP_COOLDOWN_LENGTH; -- start at zero for tiles to fall
CULUMN_COOLDOWN_LENGTH = .2;

-- These are in percent
COST_PER_MOVE = 1;
COST_PER_SWAP = 5 * COST_PER_MOVE;
HEALTH_SCORE = (2 * COST_PER_SWAP) * .3;
HEALTH_FOOD_SCORE = (COST_PER_SWAP + COST_PER_MOVE) * .3;
JUNK_FOOD_SCORE = (HEALTH_FOOD_SCORE * 2);
FILLER_SPACE_SCORE = COST_PER_SWAP - (3 * COST_PER_MOVE);

KID_THRESHOLD = 10;
TEEN_THRESHOLD = 20;
ADULT_THRESHOLD = 30;
OLD_THRESHOLD = 50;

-------------------------------------------------------------
-- GLOBAL Variables
-------------------------------------------------------------
maxHealth = 100;
currentHealth = maxHealth;
currentPopCooldown = POP_COOLDOWN_LENGTH;
currentColumnCooldown = CULUMN_COOLDOWN_LENGTH;
justFinishedPopping = true;
deltaSeconds = 0;
currentDay = 1;


bgMusic = "bg.wav";
cursorMovementSFX = "cursor.wav"
swapSFX = "swap.wav"
popSFX = "pop.wav"
ggSFX = "gg.wav"

-------------------------------------------------------------
-- Enums
-------------------------------------------------------------
TypesOfTiles =
{
    NOTHING = -1;
    HEALTH = 0;
    HEALTHY_FOOD = 1;
    JUNK = 2;
    FILLER = 3;
    
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
combos = {}

-------------------------------------------------------------
-- Called once at the start
function StartUp()
    PlayMusic(bgMusic, .05);   
    InitGame(); 
end

-------------------------------------------------------------
function InitGame()
    CreateTileBoard();
    POP_COOLDOWN_LENGTH = START_POP_COOLDOWN_LENGTH;
    currentHealth = maxHealth;
    currentDay = 1;
    cursor.x = 0;
    cursor.y = 0;
    currentDay = 0;
    combos = {};
    
end

------------------------------------------------------------
-- called every frame
function Update(ds)
    deltaSeconds = ds;
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
    if #combos > 0 then 
        justFinishedPopping = false;
        CheckToPopCombo(ds);
    else
        if justFinishedPopping == false then
            FillEmptySpotsInBoard();
        else
            CheckForCombos();
            UpdateCursor(ds);
            CheckForDefeat();
        end

        if GetNumberOfBlankTilesOnBoard() == 0 then
            justFinishedPopping = true;
            POP_COOLDOWN_LENGTH = GAME_POP_COOLDOWN_LENGTH;
        end
        
    end

    if WasKeyJustPressed("z") then
        currentDay = currentDay + 1;
    end

    if WasKeyJustPressed("x") then
        currentHealth = 0;
    end
end

------------------------------------------------------------
function UpdateDeath()

    if WasKeyJustReleased("space") then
        eCurrentMode = GameStates.ATTRACT;
        PlayMusic(bgMusic, .05);
    end
end


------------------------------------------------------------
function PopCombos(ds)
    for i = 0, #combos do
       local poppedCombo = table.remove(combos);
       poppedCombo.tile.tileType = TypesOfTiles.TILE_TYPE_ERROR;
    end
end

------------------------------------------------------------
function CheckToPopCombo(ds)
    if currentPopCooldown < 0 then
        local poppedCombo = table.remove(combos);
        AddComboToScore(poppedCombo.tile.tileType);
        
        poppedCombo.tile.tileType = TypesOfTiles.NOTHING;
        currentPopCooldown = POP_COOLDOWN_LENGTH;

        if POP_COOLDOWN_LENGTH ~= 0 then PlayOneShot(popSFX) end
    else
        currentPopCooldown = currentPopCooldown - ds;
    end
end

------------------------------------------------------------
function AddComboToScore(tileType)

    if tileType == TypesOfTiles.HEALTH then ChangeScore(HEALTH_SCORE) end
    if tileType == TypesOfTiles.HEALTHY_FOOD then ChangeScore(-HEALTH_FOOD_SCORE) end
    if tileType == TypesOfTiles.JUNK then ChangeScore(JUNK_FOOD_SCORE) end
    --if tileType == TypesOfTiles.FILLER then ChangeScore(HEALTH_SCORE) end
end

------------------------------------------------------------
function FillEmptySpotsInBoard()
    
    -- do this once per frame
    UpdateColumns();
    
end

------------------------------------------------------------
function UpdateColumns()
    
    if currentColumnCooldown <= 0 then
        local columsAreDone = false;
        for i = 0, BOARD_WIDTH do
            columsAreDone = UpdateColumn(i);
        end

        if columsAreDone == true then
            justFinishedPopping = true;
        else
            PlayOneShot(swapSFX)
        end

        currentColumnCooldown = CULUMN_COOLDOWN_LENGTH;
    else
        currentColumnCooldown = currentColumnCooldown - deltaSeconds;
    end

end

------------------------------------------------------------
function UpdateColumn(x)
    local didSomething = false;

    for currentHeight = 1, BOARD_HEIGHT + 1 do
        local currentType = GetTileType(x, currentHeight);
        local belowType = GetTileType(x, currentHeight - 1);

        if belowType == TypesOfTiles.NOTHING and currentType ~= TypesOfTiles.NOTHING then
            SetTileType(x, currentHeight, TypesOfTiles.NOTHING);
            SetTileType(x, currentHeight - 1, currentType);
            
            didSomething = true;

        elseif currentType == TypesOfTiles.TILE_TYPE_ERROR then
            SetTileType(x, currentHeight, TypesOfTiles.NOTHING);
            didSomething = true;
        end

        -- add a tile to the top!
        if currentHeight == BOARD_HEIGHT - 1 and didSomething == true then
            SetTileType(x, currentHeight, GetRandomTileType());
        end
    end

    -- fix for getting error tiles sometimes : (
    for currentHeight = 1, BOARD_HEIGHT do
        if GetTileType(x, currentHeight) == TypesOfTiles.TILE_TYPE_ERROR then
            SetTileType(x, currentHeight, GetRandomTileType());
        end
    end

    return not didSomething;
end

------------------------------------------------------------
function CheckForDefeat()
    if currentHealth <= 0 then
        eCurrentMode = GameStates.DEATH;
        StopMusic(bgMusic);
        PlayOneShot(ggSFX);
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
    PlayOneShot(cursorMovementSFX);
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
    CheckForCombos();
    ChangeScore(-COST_PER_SWAP);
    currentDay = currentDay + 1;
    PlayOneShot(swapSFX);
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
    tiles = {} -- empty board
    -- starts empty, lets them F A L L
    for tileY = 0, BOARD_HEIGHT do
        for tileX = 0, BOARD_WIDTH do
            local tile = {}
            tile.tileType = TypesOfTiles.NOTHING;
            local index = (tileY * BOARD_WIDTH) + tileX;
            tiles[index] = tile;
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
    DrawAABB2Fill(-100, -100, 100, 100, "navy");    
    DrawText("Untitled 1", -68, 48, 8, "white");
    
    DrawText("Try and match good tiles and", -80, 32, 3, "white");
    DrawText("see how many days you can live!", -80, 28, 3, "white");
    
    DrawText("Controls: ", -80, 16, 4, "white");
    DrawText("WASD cursor movement ", -60, 8, 4, "white");
    DrawText("Enter to start swap ", -60, 2, 4, "white");

    DrawText("Rules: ", -80, -12, 4, "white");
    DrawText("Movement cost life", -60, -18, 4, "white");
    DrawText("0 life == game over", -60, -24, 4, "white");



    DrawText("Hit Space to Start", -92, -50, 6, "white");
end

------------------------------------------------------------
function RenderGame()
    DrawBackground();
    DrawGrid();
    DrawPlayer();

    RenderCursor();
    DrawUI();

end

------------------------------------------------------------
function RenderDeath()
    DrawAABB2Fill(-100, -100, 100, 100, "darkGrey");
    DrawText("D E A T H", -64, 32, 8, "white");
    DrawText("You lasted: " .. currentDay .. " day!", -64, 0, 4, "white");
    DrawText("Hit Space to Continue", -72, -32, 4, "white");
end

------------------------------------------------------------
function DrawBackground()
    local maxScreenHeight = 62;
    local currentScreenHeight = Lerp(-maxScreenHeight, maxScreenHeight, (currentHealth / maxHealth));

    DrawAABB2Fill(-96, -64, 96, 64, "darkGrey");
    DrawAABB2Fill(-94 , -maxScreenHeight, 94, maxScreenHeight, "black");
    DrawAABB2Fill(-94 , -maxScreenHeight, 94, currentScreenHeight, "orangeRed");
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

    --DrawAABB2Fill(startX, startY, startX + width, startY + height, "Red");

    if currentDay <= KID_THRESHOLD then 
        DrawSprite(32, -64, -32 - 4, 0, 2,3) 
        DrawText("Baby", -77, -56, 4);
    end

    if currentDay > KID_THRESHOLD and currentDay <= TEEN_THRESHOLD then 
        DrawSprite(34, -64, -32 + 4, 0, 3,5) 
        DrawText("Kid", -73, -56, 4);
    end

    if currentDay > TEEN_THRESHOLD and currentDay <= ADULT_THRESHOLD then 
        DrawSprite(37, -64, -32 + 8, 0, 4,6) 
        DrawText("Teen", -77, -56, 4);
    end
    
    if currentDay > ADULT_THRESHOLD and currentDay <= OLD_THRESHOLD then 
        DrawSprite(41, -64, -32 + 12, 0, 5,7) 
        DrawText("Adult", -81, -56, 4);
    end
    
    if currentDay > OLD_THRESHOLD then 
        DrawSprite(112, -64, -32 + 8, 0, 4,6) 
        DrawText("Old", -73, -56, 4);
    end


end

------------------------------------------------------------
function DrawUI()

    DrawText("Day:" .. currentDay, 8 , 52, 4, "white");
    if #combos > 0 and POP_COOLDOWN_LENGTH ~= START_POP_COOLDOWN_LENGTH then DrawText("Combos: " .. #combos, -8 , -56, 4, "random") end

end

------------------------------------------------------------
-- General Functions
------------------------------------------------------------
function CheckForCombos()
    for gridY = 0, BOARD_HEIGHT - 1 do
        for gridX = 0, BOARD_WIDTH - 1 do
            
            -- Get amount in direction
            local leftAmount = GetHowManyCombosFromTileInDirection(gridX, gridY, -1, 0);
            local rightAmount = GetHowManyCombosFromTileInDirection(gridX, gridY, 1, 0);
            local upAmount = GetHowManyCombosFromTileInDirection(gridX, gridY, 0, 1);
            local downAmount = GetHowManyCombosFromTileInDirection(gridX, gridY, 0, -1);

            -- Create Combos
            if leftAmount > 2 then CreateCombo(gridX, gridY, -1, 0, leftAmount) end
            if rightAmount > 2 then CreateCombo(gridX, gridY, 1, 0, rightAmount) end
            if upAmount > 2 then CreateCombo(gridX, gridY, 0, 1, upAmount) end
            if downAmount > 2 then CreateCombo(gridX, gridY, 0, -1, downAmount) end

        end
    end
end

------------------------------------------------------------
function GetHowManyCombosFromTileInDirection(startX, startY, dirX, dirY)

    local count = 1; -- cause we start with 1
    local type = GetTileType(startX, startY);
    
    for i=1, BOARD_WIDTH do

        local theX = startX + (dirX * i);
        local theY = startY + (dirY * i);
        local currentTileType = GetTileType(theX, theY);

        if currentTileType ~= type or type == TypesOfTiles.TILE_TYPE_ERROR then
            return count;
        end

        -- valid type ++
        count = count + 1;

        -- if we are moving up see if we hit top or bottom
        if dirY ~= 0 then
            local index = GetIndexFromCoords(theX, theY);
            if index == 0 or index == BOARD_HEIGHT then
                return count;
            end
        end

        -- if we are moving horizontal see if we hit a side
        if dirX ~= 0 then
            if IsTileCoordsOnLeftEdge(theX, theY) or IsTileCoordsOnRightEdge(theX, theY) then
                return count;
            end
        end
    end

    return count;

end

------------------------------------------------------------
function CreateCombo(startX, startY, dirX, dirY, count)

    local type = GetTileType(startX, startY);
    
    for i = 0, count - 1 do
        local theX = startX + (dirX * i);
        local theY = startY + (dirY * i);
        
        combo = {};
        combo.tile = GetTile(theX, theY);
        combo.type = type;

        if CheckIfComboIsAlreadyListed(combo) == false then
            table.insert(combos, combo);
        end
    end

end

------------------------------------------------------------
function CheckIfComboIsAlreadyListed(theCombo)

    for i=1, #combos do
        if combos[i].tile == theCombo.tile then
            return true;
        end
    end

    return false;
end


------------------------------------------------------------
function GetTileRepresentationForType(tileType)

    if(tileType == TypesOfTiles.HEALTH) then return 2 end
    if(tileType == TypesOfTiles.JUNK) then return 4 end
    if(tileType == TypesOfTiles.HEALTHY_FOOD) then return 6 end
    if(tileType == TypesOfTiles.FILLER) then return 8 end
    if(tileType == TypesOfTiles.NOTHING) then return 12 end

    return 10;

end

------------------------------------------------------------
function GetTileType(x,y)
    return GetTile(x,y).tileType;
end

------------------------------------------------------------
function SetTileType(x,y, theType)
    GetTile(x,y).tileType = theType;
end

------------------------------------------------------------
function GetTile(x,y)
    local index = GetIndexFromCoords(x,y);
    local maxIndex = BOARD_WIDTH * BOARD_HEIGHT

    -- Rizzo says this is gonna be a bug, but she is wrong! (she was right)
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

    -- left and right
    if modResult == 0 or modResult == BOARD_WIDTH - 1 then
        return true;
    end

    -- top and bottom
    if tileY == 0 or tileY == BOARD_HEIGHT then
      return true
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

------------------------------------------------------------
function GetRandomTileType()
    return Floor(RandomRange(0, TypesOfTiles.NUMBER_OF_TILES));
end

------------------------------------------------------------
function GetNumberOfBlankTilesOnBoard()
    local count = 0;
    for gridY = 0, BOARD_HEIGHT - 1 do
        for gridX = 0, BOARD_WIDTH - 1 do
            local type = GetTileType(gridX, gridY);

            if type == TypesOfTiles.NOTHING then
                count = count + 1;
            end
        end       
    end

    return count;
end

------------------------------------------------------------
function ChangeScore(amount)
    
    currentHealth = currentHealth + amount;
    currentHealth = Clamp(currentHealth, 0, maxHealth);
end

-- Good luck <3


