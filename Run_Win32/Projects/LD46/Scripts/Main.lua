#include "Berry"
#include "Entity.lua"
#include "Player.lua"
#include "SlimeOrb.lua"
#include "Boss.lua"
#include "Hud.lua"
#include "Spell.lua"
#include "HealParticle.lua"
#include "SpellCaster.lua"
#include "PartyMember.lua"
#include "Attract.lua"
#include "Tutorial.lua"
#include "Playing.lua"
#include "GameWon.lua"
#include "GameLost.lua"

------------------------------------------------------------
-- Globals
------------------------------------------------------------
g_allStates = {}
g_currentState = "attract" -- tutorial, playing, gameLost, gameWon

g_totalTime = 0

-------------------------------------------------------------
-- Called once at the start
function StartUp()
    InitStates()
    SpellDefinition:CreateAll()
    PartyMemberDefinition:CreateAllDefinitions()
    
    g_allStates[g_currentState]:Enter()

    PlayMusic("bg.mp3", .1)
end

-------------------------------------------------------------
function GoToState(state)
    g_currentState = state
    g_allStates[g_currentState]:Enter()
end

-------------------------------------------------------------
function InitStates()
    g_allStates["attract"] = g_Attract
    g_allStates["tutorial"] = g_Tutorial
    g_allStates["playing"] = g_Playing
    g_allStates["gameLost"] = g_GameLostScreen
    g_allStates["gameWon"] = g_GameWonScreen
end

------------------------------------------------------------
-- called every frame
function Update(ds)
    g_totalTime += ds
	g_allStates[g_currentState]:Update(ds) 
end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
	Cls()
    g_allStates[g_currentState]:Draw(ds)
    
    --Berry.DrawAABB2(nil, "blue")
end


------------------------------------------------------------
function DoesDiscOverlapDisc(discA, discB)
    distance = Vector2:GetDistance(discA.center, discB.center)
    radiusCombined = discA.radius + discB.radius
    return distance < radiusCombined
end

-- Good luck <3


