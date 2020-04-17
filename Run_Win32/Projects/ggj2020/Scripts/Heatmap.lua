-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_theHeatMap = {}

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
HeatMap = {}

-------------------------------------------------------------------------
function HeatMap:New(x,y,width)

    g_theHeatMap = HeatMap
    g_theHeatMap.width = 16
    g_theHeatMap.mountain = 420
    
    for i=1,256 do 
        g_theHeatMap[i] = 999
    end

    local index = TileCoordToIndex(x,y,width)
    g_theHeatMap:DropHeat(index)

end

-------------------------------------------------------------------------
function HeatMap:DropHeat(startIndex)
    self[startIndex] = 0
    self.queue = List.new()
    List.pushright(self.queue, startIndex)
end

-------------------------------------------------------------------------
function HeatMap:SpreadHeat()    
    -- get one out of the queue
    local index = List.popleft(self.queue)
    local currentHeat = self[index]
    
    -- get neightbors
    aboveNeighbor = index + self.width
    belowNeighbor = index - self.width
    rightNeighbor = index + 1
    leftNeightbor = index - 1

    aboveNeighborCost = self:GetCostOfTile(aboveNeighbor)
    belowNeighborCost = self:GetCostOfTile(belowNeighbor)
    rightNeighborCost = self:GetCostOfTile(rightNeighbor)
    leftNeightborCost = self:GetCostOfTile(leftNeightbor)
    
    -- check neighbors, if their heat is greater, change it to mine + 1
    if (not IsIndexOnTopSide(index, self.width)) and g_theHeatMap[aboveNeighbor] > (currentHeat + aboveNeighborCost) then
        g_theHeatMap[aboveNeighbor] = currentHeat + aboveNeighborCost
        List.pushright(self.queue, aboveNeighbor)
    end
    
    if (not IsIndexOnBottomSide(index, self.width)) and g_theHeatMap[belowNeighbor] > (currentHeat + belowNeighborCost) then
        g_theHeatMap[belowNeighbor] = currentHeat + belowNeighborCost
        List.pushright(self.queue, belowNeighbor)
    end
    
    if (not IsIndexOnRightSide(index, self.width)) and g_theHeatMap[rightNeighbor] > (currentHeat + rightNeighborCost) then
        g_theHeatMap[rightNeighbor] = currentHeat + rightNeighborCost
        List.pushright(self.queue, rightNeighbor)
    end
    
    if (not IsIndexOnLeftSide(index, self.width)) and g_theHeatMap[leftNeightbor] > (currentHeat + leftNeightborCost) then
        g_theHeatMap[leftNeightbor] = currentHeat + leftNeightborCost
        List.pushright(self.queue, leftNeightbor)
    end
end

-------------------------------------------------------------------------
-- If you wanted to make the heatmap and engine thing, this function should be 
-- provided by the game so the heatmap can look up costs, but be independent from
-- game code
function HeatMap:GetCostOfTile(index)
    tileX, tileY = IndexToTileCoords(index, self.width)
    spriteSheetIndex = GetTileSprite(tileX * 16, tileY * 16)
    if(spriteSheetIndex == 6 or spriteSheetIndex == 16 or spriteSheetIndex == 17 
    or spriteSheetIndex == 18 or spriteSheetIndex == 19 or spriteSheetIndex == 20) then
        return 420
    end

    return 1
end

-------------------------------------------------------------------------
function HeatMap:SpreadHeatIfNeeded()
    --if self.queue.size > 0 then
    while self.queue.size > 0 do
       self:SpreadHeat()
    end
end

-------------------------------------------------------------------------
function HeatMap:Render()
    for x = 0, g_theHeatMap.width do
        for y = 0, g_theHeatMap.width do
            local index = TileCoordToIndex(x,y,g_theHeatMap.width)
            local heat = g_theHeatMap[index]

            DrawText(heat, (x * 16) + 24, (y *16) + 8, 2.5)
        end
    end

    if self.queue.size > 0 then
        local peekIndex = List.peekleft(g_theHeatMap.queue)
        tilex,tiley = IndexToTileCoords(peekIndex, g_theHeatMap.width)
        DrawAABB2((tilex - 1) * 16, (tiley) * 16, (tilex ) * 16, (tiley + 1) * 16, "red")
    end
end

-------------------------------------------------------------------------
function HeatMap:GetHeatFromIndex(index)
    return self[index]
end

-------------------------------------------------------------------------
function HeatMap:GetHeatFromTilePos(x,y)
    if(x > 0 and x < 16 and y > 0 and y < 16) then

        index = TileCoordToIndex(x,y,self.width)
        return self[index]
    end
    return 999
end

-------------------------------------------------------------------------
function HeatMap:GetHeatFromWorldPosition(x,y)
    local xx = Floor(x / width)
    local yy = Floor(y / width)

    return GetHeatFromTilePos(xx,yy)
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function TileCoordToIndex(x,y,width)
    return ((y * width) + x) + 1
end

-------------------------------------------------------------------------
function IndexToTileCoords(index, width)
    indexX = index % width;
    indexY = Floor((index / width));
    
    return indexX, indexY
end

-------------------------------------------------------------------------
function IsIndexInTableValid(index, table)
    return (index <= 256) and (index > 0) and (table[index] ~= nil)
end

-------------------------------------------------------------------------
function IsIndexOnLeftSide(index,width)
    return index % width == 1
end

-------------------------------------------------------------------------
function IsIndexOnRightSide(index, width)
    return index % width == 0
end

-------------------------------------------------------------------------
function IsIndexOnTopSide(index, width)
    return index >= (256 - width)
end

-------------------------------------------------------------------------
function IsIndexOnBottomSide(index, width)
    return index <= width
end
-------------------------------------------------------------------------
-- Queue
-------------------------------------------------------------------------
List = {}

function List.new ()
  return {first = 0, last = -1, size = 0}
end

function List.pushleft (list, value)
  local first = list.first - 1
  list.first = first
  list[first] = value
  list.size = list.size + 1
end

function List.pushright (list, value)
  local last = list.last + 1
  list.last = last
  list[last] = value
  list.size = list.size + 1
end

function List.popleft (list)
  local first = list.first
  if first > list.last then error("list is empty") end
  local value = list[first]
  list[first] = nil        -- to allow garbage collection
  list.first = first + 1
  list.size = list.size - 1
  return value
end

function List.popright (list)
  local last = list.last
  if list.first > last then error("list is empty") end
  local value = list[last]
  list[last] = nil         -- to allow garbage collection
  list.last = last - 1
  list.size = list.size - 1
  return value
end

function List.peekleft(list)
    return list[list.first]
end
