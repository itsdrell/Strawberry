#include "Base.lua"
#include "Turret.lua"
#include "Entity.lua"
#include "Player.lua"
#include "Enemy.lua"
#include "Hud.lua"
#include "SpawnManager.lua"
#include "Heatmap.lua"
#include "Attract.lua"
#include "Tutorial.lua"
#include "Playing.lua"
#include "GameWon.lua"
#include "GameLost.lua"
#include "Explosion.lua"

-------------------------------------------------------------
g_allStates = {}
g_currentState = "attract" -- tutorial, playing, gameLost, gameWon
g_debug = false

-------------------------------------------------------------
-- Called once at the start
function StartUp()
    InitStates()
    PlayMusic("Background.mp3")
end

-------------------------------------------------------------
function InitStates()
    g_allStates["attract"] = g_Attract
    g_allStates["tutorial"] = g_Tutorial
    g_allStates["playing"] = g_Playing
    g_allStates["gameLost"] = g_GameLostScreen
    g_allStates["gameWon"] = g_GameWonScreen
end

-------------------------------------------------------------
function GoToState(state)
    g_currentState = state
    g_allStates[g_currentState]:Enter()
end

------------------------------------------------------------
-- called every frame
function Update(ds)
    g_allStates[g_currentState]:Update(ds) 
    
    if WasKeyJustPressed("m") then
        PlayOneShot("Explosion.aiff")
    end
end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
    Cls()
    g_allStates[g_currentState]:Draw(ds)
end


-- Good luck <3


