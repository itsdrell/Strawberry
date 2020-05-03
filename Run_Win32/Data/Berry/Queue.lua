-------------------------------------------------------------------------
-- Queue
-------------------------------------------------------------------------
Queue = 
{
    front = 0, 
    back = -1, 
    size = 0
}

-- allows us to do Queue(table)
setmetatable(Queue, 
{
    __call = function(theTable, table) return Queue:Create(table) end
})

-------------------------------------------------------------------------
-- note it defaults to Pushing Left
function Queue:Create(table)
    local newQ = {}
    setmetatable(newQ, {__index = Queue})
  
    newQ.front = 1
    newQ.back = 0
    newQ.size = 0

    if table ~= nil then 
        for entry in all(table) do
            newQ:Push(entry)
        end
    end

    return newQ
end

-------------------------------------------------------------------------
function Queue:Clear()
    for k,v in pairs(self) do
        self[k] = nil
    end

    self = Queue:Create()
end

-------------------------------------------------------------------------
function Queue:Push(value)
  self.back++
  self[self.back] = value
  self.size++
end

-------------------------------------------------------------------------
function Queue:Pop()
    if self.front > self.back then error("Tried to pop a queue when it was empty!") end 
    local value = self[self.front]
    self[self.front] = nil
    self.front++
    self.size--
    return value
end

-------------------------------------------------------------------------
function Queue:Peek() return self[self.front] end

-------------------------------------------------------------------------
function Queue:IsEmpty()
    return self.size == 0
end

-------------------------------------------------------------------------
function Queue:Copy()
    local copy = Berry.Copy(self)
    setmetatable(copy, {__index = Queue })

    return copy
end

-------------------------------------------------------------------------
function Queue:ToString()
    local copy = self:Copy()
    
    if copy:IsEmpty() then return "Empty" end

    local theString = ""
    while copy.size ~= 0 do  
        value = copy:Pop()
        theString = theString .. value .. ","
    end

    return theString
end