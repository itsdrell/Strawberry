-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Timer = 
{
    length = 1,
    elapsedTime = 0
}
g_allTimers = {}

-------------------------------------------------------------------------
function Timer:Create(length)
    local newInst = {}
    setmetatable(newInst, {__index = Timer})

    newInst.length = length

    table.insert(g_allTimers, newInst)
    return newInst
end

-------------------------------------------------------------------------
function Timer:SetLength(length)
    self.length = length

    -- assume we should reset it as well?
    self.elapsedTime = 0
end

-------------------------------------------------------------------------
function Timer:Update(ds)
    if self.elapsedTime > self.length then return end 

    self.elapsedTime += ds
end

-------------------------------------------------------------------------
function Timer:HasElapsed()
    return self.elapsedTime >= self.length
end

-------------------------------------------------------------------------
function Timer:GetElapsedTime()
    return self.elapsedTime
end

-------------------------------------------------------------------------
function Timer:GetNormalizedElapsedTime()
    return Clamp(self.elapsedTime / self.length, 0 , 1)
end

-------------------------------------------------------------------------
function Timer:Reset()
    self.elapsedTime = 0
end

-------------------------------------------------------------------------
function Timer:CheckAndReset()
    if self.elapsedTime >= self.length then
        self.elapsedTime = 0
        return true
    end
end

-------------------------------------------------------------------------
function Timer:ToString()
    return (self.elapsedTime .. self.length)
end

-------------------------------------------------------------------------
-- Standalone
-------------------------------------------------------------------------
function UpdateAllTimers(ds)
    for timer in all(g_allTimers) do 
        timer:Update(ds)
    end
end