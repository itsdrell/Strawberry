-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
ScoreManager = 
{
    playerLives = 3,
    score = 0
}
g_theScoreManager = ScoreManager

-------------------------------------------------------------------------
function ScoreManager:Render()
    -- todo make this a cute ball sprite
    local startX = 30
    for i=1,self.playerLives do
        DrawCircle(startX, 240, 6)
        startX += 18
    end
    
    DrawText("Score: " .. self.score, 128, 236, 6)
end
-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function IsGameOver()
    return g_theScoreManager.playerLives <= 0
end

-------------------------------------------------------------------------
function AddScore(scoreAmount)
    g_theScoreManager.score += scoreAmount
end

-------------------------------------------------------------------------
function LoseLife()
    g_theScoreManager.playerLives -= 1
end
