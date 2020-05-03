-------------------------------------------------------------------------
-- Adding to the namespace berry
-------------------------------------------------------------------------
Berry.Abs = Abs
Berry.ATan2 = ATan2
Berry.CosDegrees = CosDegrees
Berry.SinDegrees = SinDegrees
Berry.Floor = Floor
Berry.Ceil = Ceil
Berry.Min = Min
Berry.Max = Max
Berry.Sqrt = Sqrt
Berry.Clamp = Clamp
Berry.RandomRange = RandomRange
Berry.Lerp = Lerp
Berry.Chance = Chance
Berry.Fract = Fract
Berry.RangeMap = RangeMap

-------------------------------------------------------------------------
-- Overlap functions for AABB2 and Disc pls
-------------------------------------------------------------------------
Berry.AABB2vsDisc = function(aabb2, disc)
    closestx = Clamp(disc.center.x, aabb2.mins.x, aabb2.maxs.x)
    closesty = Clamp(disc.center.y, aabb2.mins.y, aabb2.maxs.y)

    distance = GetDistance(closestx, closesty, disc.center.x, disc.center.y)

    if distance < disc.radius then
        return true
    end

    return false
end

-------------------------------------------------------------------------
Berry.DiscvsAABB2 = function(disc, aabb2)
    return Berry.AABB2vsDisc(aabb2, disc)
end

-------------------------------------------------------------------------
Berry.AABB2vsAABB2 = function(aabb2A, aabb2B)
	if(aabb2A.maxs.x < aabb2B.mins.x or aabb2A.mins.x > aabb2B.maxs.x) then return false end
	if(aabb2A.maxs.y < aabb2B.mins.y or aabb2A.mins.y > aabb2B.maxs.y) then return false end

	return true;
end

-------------------------------------------------------------------------
Berry.DiscvsDisc = function(discA, discB)
    distance = Vector2:GetDistance(discA.center, discB.center)
    radiusCombined = discA.radius + discB.radius
    return distance < radiusCombined
end

-------------------------------------------------------------------------
Berry.DiscvsTile = function(disc, tilePos)
    return Berry.DiscvsAABB2(disc, Berry.GetBoundsFromTilePos(tilePos))
end

-------------------------------------------------------------------------
Berry.AABB2vsTile = function(disc, tilePos)
    return Berry.DiscvsAABB2(disc, Berry.GetBoundsFromTilePos(tilePos))
end

-------------------------------------------------------------------------
-- Collision math
-------------------------------------------------------------------------
-- This updates the disc as well as returns the position so you can 
-- set a player position to the resulting value
Berry.DiscAABB2Corrective = function(disc, aabb2)
    closestx = Clamp(disc.center.x, aabb2.mins.x, aabb2.maxs.x)
    closesty = Clamp(disc.center.y, aabb2.mins.y, aabb2.maxs.y)

    distance = GetDistance(closestx, closesty, disc.center.x, disc.center.y)

    displacementx = disc.center.x - closestx
    displacementy = disc.center.y - closesty

    length = disc.radius - distance

    dirX, dirY = Normalize(displacementx, displacementy)

    amountX = dirX * length
    amountY = dirY * length

    amount = Vector2:Create(amountX, amountY)
    disc.center += amount
    return disc.center
end

-------------------------------------------------------------------------
Berry.DiscCorrective = function(discA, discB, normalizedSecondWeight)
    weight = normalizedSecondWeight or .5

    distance = GetDistance(discA.center.x, discA.center.y, discB.center.x, discB.center.y)
    radiusCombined = discA.radius + discB.radius

    if distance > radiusCombined then return end

    disp = Vector2:Create(discB.center.x - discA.center.x, discB.center.y - discA.center.y)
    disp:Normalize()
    oppDir = disp * -1

    totalPushAmount = radiusCombined - distance
    aAmount = totalPushAmount * weight
    bAmount = totalPushAmount * (1-weight)

    discA.center += (oppDir * aAmount)
    discB.center += (disp * bAmount)
    return discA.center, discB.center
end

-------------------------------------------------------------------------
Berry.DiscTileCorrective = function(disc, tilePos)
    return Berry.DiscAABB2Corrective(disc, Berry.GetBoundsFromTilePos(tilePos))
end

-------------------------------------------------------------------------
Berry.Reflect = function(incoming, surfaceNormal)
        
    dot = Dot(incoming.x, incoming.y, surfaceNormal.x,surfaceNormal.y)

    surfaceNormalLength = Sqrt((surfaceNormal.x*surfaceNormal.x) + (surfaceNormal.y*surfaceNormal.y))

    dotTimesSurfaceNormal = dot / surfaceNormalLength
    vnx = dotTimesSurfaceNormal * (surfaceNormal.x / surfaceNormalLength) -- surfaceNormalize.x
    vny = dotTimesSurfaceNormal * (surfaceNormal.y / surfaceNormalLength) -- surfaceNormalize.y

    resultx = incoming.x - (2 * vnx)
    resulty = incoming.y - (2 * vny)
    
    return Vector2:Create(resultx, resulty)
end