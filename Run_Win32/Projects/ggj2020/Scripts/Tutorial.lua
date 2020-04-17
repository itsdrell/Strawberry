-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_seenTutorial = false

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Tutorial = 
{
    pivotx = 256 * .05,
    pivoty = 256 * .95,
    currentSlideIndex = 1
}
g_Tutorial = Tutorial

-------------------------------------------------------------------------
function Tutorial:Enter() 
    if g_seenTutorial == true then
        GoToState("playing")
    end

    g_seenTutorial = true
end

-------------------------------------------------------------------------
function Tutorial:Update(ds)
    if ShouldWeMoveToNextSlide() then 
        if self.currentSlideIndex == 3 then 
            GoToState("playing")
        else
            self.currentSlideIndex++
        end
        
    end
end

-------------------------------------------------------------------------
function ShouldWeMoveToNextSlide()

    return WasKeyJustReleased("enter") or WasKeyJustReleased("space") or WasKeyJustReleased("right") or WasKeyJustReleased("d")

end

-------------------------------------------------------------------------
function Tutorial:Draw()
    DrawAABB2Fill(0,0,1000,1000,"black")
    DrawMap();
    HideMapTiles()

    min = 256 * .05
    max = 256 * .95
    DrawAABB2Fill(min,min,max,max, "black")
    DrawAABB2(min,min,max,max,"random")

    self:DrawCurrentSlide()
    self:DrawProgress()

end

-------------------------------------------------------------------------
function Tutorial:DrawCurrentSlide()
    local index = self.currentSlideIndex
    if index == 1 then
        self:DrawSummary()
    elseif index == 2 then
        self:DrawEnemyAndBaseInfo()
    else
        self:DrawIconSlide()
    end
end

-------------------------------------------------------------------------
function Tutorial:DrawEnemyAndBaseInfo()
    min = 256 * .05
    max = 256 * .9

    local enemies = "Enemies will slowly move toward the base and attack once in range \n \n"
    local turrets = "\n \n Turrets will shoot at enemies once they are inside range, but will use part of their power to do so. \n You will need to watch their power levels and repair when needed!"

    local fulltext = enemies .. turrets
    DrawTextWrapped(fulltext, min, min, max, max, 6)
end

-------------------------------------------------------------------------
function Tutorial:DrawIconSlide()

    stepsize = 256 / 9
    spritex = self.pivotx + 24
    textx = 140

    -- player
    playery = self.pivoty - (stepsize * 1)
    DrawSprite(0,spritex + 32, playery) 
    DrawText("Player", textx, playery - 4,6)

    -- Enemy
    enemyy = self.pivoty - (stepsize * 3)
    DrawSprite(2,spritex + 32, enemyy) 
    DrawText("Enemy", textx, enemyy - 4,6)

    -- Base
    basey = self.pivoty - (stepsize * 5)
    DrawSprite(1,spritex + 32, basey)
    DrawText("Base", textx, basey - 4,6) 

    -- turrets
    turrety = self.pivoty - (stepsize * 7)
    DrawSprite(16,spritex + 32,turrety,0,5) 
    DrawText("Turrets", textx, turrety  - 4,6)

end

-------------------------------------------------------------------------
function Tutorial:DrawSummary()
    min = 256 * .05
    max = 256 * .85

    local summary = "Goal: Help repair turrets to defend your base from the onlsaught of enemies until help arrives! \n \n"
    local controls = "\n \n Controls: Use WASD to move the player around and space to repair turrets when close to them"

    local fulltext = summary .. controls
    DrawTextWrapped(fulltext, min, min, max, max, 6)
end

-------------------------------------------------------------------------
function Tutorial:DrawProgress()

    local startLocation = 80
    local height = 24
    local stepSize = 24

    for i=1,3 do 
        
        if i == self.currentSlideIndex then
            DrawCircleFill(startLocation + (i * stepSize), height, 2, "white")
        else
            DrawCircle(startLocation + (i * stepSize), height, 2, "white")
        end
    end

end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

