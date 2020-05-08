#include "Berry"



fallingBall = Disc:Create(64,224,6)
acc = Vector2:Create(0, -9.8 * 3)

groundBox = AABB2:Create(0,64,256,72)
solidDisc = Disc:Create(64, 180, 6)

textPos = 0

Player = 
{
	pos = Vector2:Create(72,72),
	collision = Disc:Create(64,64,6),
	
	bounds = 
	{
		center = AABB2:Create(0,0,0,0),
		tl = AABB2:Create(0,0,0,0),
		t = AABB2:Create(0,0,0,0),
		tr = AABB2:Create(0,0,0,0),
		r = AABB2:Create(0,0,0,0),
		br = AABB2:Create(0,0,0,0),
		b = AABB2:Create(0,0,0,0),
		bl = AABB2:Create(0,0,0,0),
		l = AABB2:Create(0,0,0,0),
	},
}
g_player = Player

-------------------------------------------------------------
-- Called once at the start
function StartUp()
	test = Vector2(10,10)
	--dPrint(test:ToString(), 10)

	test = Disc(10,10,5)
	--dPrint(test:ToString(),10)

	test = AABB2(1,2,3,4)
	--dPrint(test:ToString(),10)

end


------------------------------------------------------------
-- called every frame
function Update(ds)

	--g_player.pos -= (Vector2:Create(0,9.8) * ds)
	g_player.collision.center = g_player.pos

	UpdatePos(ds)
	UpdateBounds()
	--Collision()
	CollisionByTiles()
	CollisionByChannel()

	textPos += (ds * .1)
end

------------------------------------------------------------
function UpdatePos(ds)

	speed = 100
	if IsKeyPressed("w") then
		g_player.pos += (Vector2:Create(0,1) * speed * ds)
	end

	if IsKeyPressed("s") then
		g_player.pos += (Vector2:Create(0,-1) * speed * ds)
	end

	if IsKeyPressed("d") then
		g_player.pos += (Vector2:Create(1,0) * speed * ds)
	end

	if IsKeyPressed("a") then
		g_player.pos += (Vector2:Create(-1,0) * speed * ds)
	end
end

------------------------------------------------------------
function CollisionByChannel()
	currentTile = Vector2:Create(Floor(g_player.pos.x / 16), Floor(g_player.pos.y / 16))

	collisions = Berry.GetNeighborHoodCollisions(g_player.collision, currentTile)
	for collision in all(collisions) do 
		if Berry.DoesTileHaveCollision(collision, 1) then 
			g_player.pos = Berry.DiscTileCorrective(g_player.collision, collision)
		end
	end
end

------------------------------------------------------------
function CollisionByTiles()
	currentTile = Vector2:Create(Floor(g_player.pos.x / 16), Floor(g_player.pos.y / 16))
	
	collisions = Berry.GetNeighborHoodCollisions(g_player.collision, currentTile)
	for collision in all(collisions) do 
		if Berry.DoesTileHaveSprite(collision, 81) then 
			g_player.pos = Berry.DiscTileCorrective(g_player.collision, collision)
		end
	end
end

------------------------------------------------------------
function CollisionByBounds()
	
	bound = g_player.bounds.tl
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)

	end

	bound = g_player.bounds.t
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.tr
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.r
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.br
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.b
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.bl
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	bound = g_player.bounds.l
	if Berry.DiscvsAABB2(g_player.collision, bound) then 
		g_player.pos = Berry.DiscAABB2Corrective(g_player.collision, bound)
	end

	--g_player.pos = g_player.collision.center
end

------------------------------------------------------------
function UpdateBounds()

	currentTile = Vector2:Create(Floor(g_player.pos.x / 16), Floor(g_player.pos.y / 16))

	g_player.bounds.center = Berry.GetBoundsFromTilePos(currentTile)
	g_player.bounds.tl = Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(-1,1))
	g_player.bounds.t 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(0,1))
	g_player.bounds.tr 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(1,1))
	g_player.bounds.r 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(1,0))
	g_player.bounds.br 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(1,-1))
	g_player.bounds.b 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(0,-1))
	g_player.bounds.bl 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(-1,-1))
	g_player.bounds.l 	= Berry.GetBoundsFromTilePos(currentTile + Vector2:Create(-1,0))

end

------------------------------------------------------------
function BounceUpdate(ds)
	fallingBall.center += (acc * ds)

	if Berry.DiscvsAABB2(fallingBall, groundBox) then
		dPrint("colliding")
		Berry.DiscBoxCorrective(fallingBall,groundBox)
	end

	Berry.DiscCorrective(fallingBall, solidDisc)
end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
	Cls()
	DrawMap()

	Berry.DrawCircle(g_player.collision, "green")
	Berry.DrawAABB2(g_player.bounds.center, "red")

	Berry.DrawAABB2(g_player.bounds.tl  , Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.tl), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.t 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.t ), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.tr 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.tr), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.r 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.r ), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.br 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.br), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.b 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.b ), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.bl 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.bl), "red", "white"))
	Berry.DrawAABB2(g_player.bounds.l 	, Pick( Berry.DiscvsAABB2(g_player.collision, g_player.bounds.l ), "red", "white"))

end

------------------------------------------------------------
function BounceDraws()
	Berry.DrawCircle(fallingBall, "red")
	Berry.DrawAABB2(groundBox, "blue")
	Berry.DrawCircle(solidDisc, "green")
end

------------------------------------------------------------
function OverlapsDraws()
	boxA = AABB2:Create(32,32,48,48)
	boxB = AABB2:Create(50,50,64,64)
	result = Berry.AABB2vsAABB2(boxA, boxB)

	dPrint(Berry.BoolToString(result))
	Berry.DrawAABB2(boxA, "blue")
	Berry.DrawAABB2(boxB, "red")

	boxA = AABB2:Create(70,32,82,48)
	boxB = AABB2:Create(76,42,90,64)
	result = Berry.AABB2vsAABB2(boxA, boxB)

	dPrint(Berry.BoolToString(result))
	Berry.DrawAABB2(boxA, "blue")
	Berry.DrawAABB2(boxB, "red")

	boxA = AABB2:Create(96,32,106,48)
	discA = Disc:Create(112,40,6)
	result = Berry.DiscvsAABB2(discA, boxA)

	dPrint(Berry.BoolToString(result))
	Berry.DrawAABB2(boxA, "blue")
	Berry.DrawCircle(discA, "red")

	boxA = AABB2:Create(112,32,124,48)
	discA = Disc:Create(116,40,6)
	result = Berry.DiscvsAABB2(discA, boxA)

	dPrint(Berry.BoolToString(result))
	Berry.DrawAABB2(boxA, "blue")
	Berry.DrawCircle(discA, "red")
end


-- Good luck <3


