-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Player = {}
g_player = {}

function Player:New(x,y)
    local newinst = {}

    -- this makes player an enity
    setmetatable( Player, { __index = Entity } )

    local newinst = {}
    -- this makes the newinst a player thats also an entity
    setmetatable( newinst, { __index = Player } )

    newinst.sprite = 0
    newinst.x = x
    newinst.y = y
    newinst.repairAmount = 50

    return newinst
end

function Player:Update(ds)
    self:MovePlayer()

    if WasKeyJustReleased("space") then self:Repair() end
end

function Player:Repair()
    for turret in all(g_allTurrets) do
        if GetDistance(turret.x, turret.y, self.x, self.y) <= 16 then
            turret:Repair(self.repairAmount)
        end
    end
end

function Player:MovePlayer()
    if(WasKeyJustReleased("d")) then
        self.x += 16
    end

    if(WasKeyJustReleased("a")) then
        self.x -= 16
    end

    if(WasKeyJustReleased("w")) then
        self.y += 16
    end

    if(WasKeyJustReleased("s")) then
        self.y -= 16
    end
end


function Player:Render()
    DrawSprite (self.sprite, self.x, self.y)

end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

