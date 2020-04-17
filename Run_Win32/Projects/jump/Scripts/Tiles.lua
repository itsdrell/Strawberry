-------------------------------------------------------------------------
-- Tile Globals
-------------------------------------------------------------------------
g_tiles = {}

-------------------------------------------------------------------------
-- Tile Class
-------------------------------------------------------------------------
Tile = class(nil,
{
    x = 0,
    y = 0,
    id = 0,
    width = 8,
    height = 24,
    color = "black",
    currentHealth = 1,
    definition = nil
})

-------------------------------------------------------------------------
function Tile:New(x,y,def)
    local newTile = Tile:Create()
    newTile.x = x
    newTile.y = y 
    newTile.definition = def

    newTile.currentHealth = def.startHealth
    
    table.insert(g_tiles, newTile)
    newTile.id = #g_tiles
    
    return newTile
end

-------------------------------------------------------------------------
function Tile:Delete()
    self:RemoveFromList()
end

-------------------------------------------------------------------------
function Tile:RemoveFromList()
    for k,v in ipairs(g_tiles) do
        if v.id == self.id then
            table.remove(g_tiles, k)
        end
    end
end
    
-------------------------------------------------------------------------
function Tile:Hit()
    -- do sound or score or something
    Screenshake(.1)
    self.currentHealth -= 1

    if(self.currentHealth <= 0) then
        
        if self.definition.deathFunction ~= nil then
            self.definition.deathFunction(self)
        end
        
        self:Delete();
        AddScore(100)
    end

end

-------------------------------------------------------------------------
function Tile:Render()
    
    if(self.definition ~= nil) then
        self.definition.drawFunction(self)
    end
    
    DrawAABB2(self.x, self.y, self.x + self.width, self.y + self.height, self.color)
end

-------------------------------------------------------------------------
-- Standalone Tile Functions
-------------------------------------------------------------------------
function RenderTiles()
    for tile in all(g_tiles) do
        tile:Render()
    end
end
