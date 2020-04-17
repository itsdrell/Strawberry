Berry = {}

-------------------------------------------------------------------------
-- Utils functions that wrap exposed c++ functions with 
-- util data structures
-------------------------------------------------------------------------
-------------------------------------------------------------------------
-- ternary?? select/Pick/iif
-- I think some people call it iff
-- if valueIfTrue is false or nil, the expression will fall through and choose valueIfFalse.
function Pick(condition, valueIfTrue, valueIfFalse)
    return condition and valueIfTrue or valueIfFalse
end
Berry.Pick = Pick

-------------------------------------------------------------------------
-- shallow copy
Berry.Copy = function (oldTable)
    local newTable = {}
    for k,v in pairs(oldTable) do
        newTable[k] = v
    end
    return newTable
end

-------------------------------------------------------------------------
Berry.BoolToString = function(bool)
    if bool then return "True" end 
    return 'False'
end

-------------------------------------------------------------------------
-- Rendering
-------------------------------------------------------------------------
function Berry.DrawLine(vec2startPos, vec2endPos, r, g, b, a)
    DrawLine(vec2startPos.x, vec2startPos.y, vec2endPos.x, vec2endPos.y, r, g, b, a)
end

-------------------------------------------------------------------------
function Berry.DrawCircle(disc, r,g,b,a)
    DrawCircle(disc.center.x, disc.center.y, disc.radius, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawCircleFill(disc, r,g,b,a)
    DrawCircleFill(disc.center.x, disc.center.y, disc.radius, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawAABB2(box, r,g,b,a)
    DrawAABB2(box.mins.x, box.mins.y, box.maxs.x, box.maxs.y, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawAABB2Fill(box, r,g,b,a)
    DrawAABB2Fill(box.mins.x, box.mins.y, box.maxs.x, box.maxs.y, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawSprite(index, pos, rotation, width, height, flipX, flipY, pixelsPerUnit)
    DrawSprite(index, pos.x, pos.y, rotation, width, height, flipX, flipY, pixelsPerUnit)
end

-------------------------------------------------------------------------
function Berry.DrawText(string, pos, height, r,g,b,a)
    DrawText(string, pos.x, pos.y, height, r,g,b,a)
end

-------------------------------------------------------------------------
function Berry.DrawTextWrapped(string, box, height, r,g,b,a)
    DrawTextWrapped(string, box.mins.x, box.mins.y, box.maxs.x, box.maxs.y, height, r,g,b,a)
end

-------------------------------------------------------------------------
-- Misc things added to namespace
-------------------------------------------------------------------------
Berry.Screenshake = Screenshake