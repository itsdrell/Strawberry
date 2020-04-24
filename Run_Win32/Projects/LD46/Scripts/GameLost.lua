-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
GameLostScreen = 
{

}
g_GameLostScreen = GameLostScreen

-------------------------------------------------------------------------
function GameLostScreen:Enter()
    self.timer = 0
    self.timeToWait = 1.5
    self.percent = 0
    self.percentGrowth = 200
    self.radius = 0

    PlayOneShot("gameOver.wav")
end

-------------------------------------------------------------------------
function GameLostScreen:Update(ds)
    if(WasKeyJustReleased("enter")) then GoToState("attract") end

    if self.timer >= self.timeToWait then

        if self.radius > 500 then 
            GoToState("attract")
        else
            self.radius += (self.percentGrowth * ds)
        end

    else
        self.timer += ds
    end
end

-------------------------------------------------------------------------
function GameLostScreen:Draw()
    DrawMap()
    DrawAllEntities()

    if self.timer >= self.timeToWait then
        DrawCircleFill(128,128, self.radius + 1, "darkGreen")
        DrawCircleFill(128,128, self.radius, "lightGreen")
    end
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

