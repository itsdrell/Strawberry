
Berry = {}

-------------------------------------------------------------------------
-- Utils functions that wrap exposed c = c + 1 functions with 
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

--------------------------------------------------------------------------
function all(t)
	local i = 0
	local n = #t
	return function()
	         i = i + 1
	         if i <= n then return t[i] end
	       end
end

--------------------------------------------------------------------------
-- helper for making classes
-- taken from Game Coding Complete v4
function Class(baseClass, body)
    local ret = body or {};
    
    -- if there's a base class, attach our new class to it
    if (baseClass ~= nil) then
        setmetatable(ret, ret);
        ret.__index = baseClass;
    end
    
    -- Add the Create() function (third param must be nil)
    ret.Create = function(self, constructionData, originalSubClass)
        local obj;
        if (self.__index ~= nil) then
            if (originalSubClass ~= nil) then
                obj = self.__index:Create(constructionData, originalSubClass);
            else
                obj = self.__index:Create(constructionData, self);
            end
        else
            obj = constructionData or {};
        end

        setmetatable(obj, obj);
        obj.__index = self;
        
        -- copy any operators over
        if (self.__operators ~= nil) then
            for key, val in pairs(self.__operators) do
                obj[key] = val;
            end
        end
        
        return obj;
    end
    
    return ret;
end

-------------------------------------------------------------------------
-- Input
-------------------------------------------------------------------------
function Berry.GetMousePosition()
	xpos,ypos = GetMousePosition()
	
	return Vector2:Create(xpos,ypos) 
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
-- todo 
-------------------------------------------------------------------------
--  Vector Class
-------------------------------------------------------------------------
Vector2 = 
{
    x = 0,
    y = 0,   
}

-------------------------------------------------------------------------
function Vector2:Create(x,y)
    local newInstance = {}
    setmetatable( newInstance, 
    { 
        __index = Vector2,
        __add = AddTwoVectors,
        __sub = SubtractTwoVectors,
        __mul = MultiplyVector, -- Does either dot, or uniform scale based off of the other variable
        __eq = DoTwoVectorsEqual,
        __len = LengthOfVector,
        __concat = Vector2ConcatWithString
    })

    -- if you add a copy constructor, dont just set them equal to another
    -- since thats a reference and not a copy

    newInstance.x = x
    newInstance.y = y

    return newInstance
end

-------------------------------------------------------------------------
function Vector2:Normalize()
    return Normalize(self.x, self.y)
end

-------------------------------------------------------------------------
function Vector2:GetLength()
    return GetLength(self.x, self.y)
end

-------------------------------------------------------------------------
function Vector2:GetLengthSquared()
    return GetLengthSquared(self.x, self.y)
end

-------------------------------------------------------------------------
function Vector2:ToString()
    return "X is: " .. self.x .. " Y is: " .. self.y
end

-------------------------------------------------------------------------
function Vector2:Dot(vectorB)
    return Dot(self.x, self.y, vectorB.x, vectorB.y)
end

-------------------------------------------------------------------------
function Vector2:GetDistance(startVec, endVec)
    return GetDistance(startVec.x, startVec.y, endVec.x, endVec.y)
end

-------------------------------------------------------------------------
function Vector2:GetDistanceSquared(startVec, endVec)
    return GetDistanceSquared(startVec.x, startVec.y, endVec.x, endVec.y)
end

-------------------------------------------------------------------------
-- Operators
-------------------------------------------------------------------------
function AddTwoVectors(vecA, vecB)
    return Vector2:Create(vecA.x + vecB.x, vecA.y + vecB.y)
end

-------------------------------------------------------------------------
function SubtractTwoVectors(vecA, vecB)
    return Vector2:Create(vecA.x - vecB.x, vecA.y - vecB.y)
end

-------------------------------------------------------------------------
function Vector2ConcatWithString(theString, vec)
    return theString .. vec:ToString()
end

-------------------------------------------------------------------------
function DotProduct(vecA, vecB)
    return vecA:Dot(vecB)
end

-------------------------------------------------------------------------
function UniformScaleVector(vecA, scale)
    local result = Vector2:Create(0,0)
    result.x = vecA.x * scale
    result.y = vecA.y * scale 
    return result
end

-------------------------------------------------------------------------
-- Note this only works with the vector one the right. Ex: Vector * <another vector or scale>
function MultiplyVector(vecA, other)
    if type(other) == "table" then 
        return DotProduct(vecA, other)
    else
        return UniformScaleVector(vecA, other)
    end
end

-------------------------------------------------------------------------
function DoTwoVectorsEqual(vecA, vecB)
    return (vecA.x == vecB.x) and (vecA.y == vecB.y)
end

-------------------------------------------------------------------------
function LengthOfVector(vec)
    return vec:GetLength()
end
 
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Disc =
{
    center = {x = 0, y = 0},
    radius = 1
}

-------------------------------------------------------------------------
function Disc:Create(input1, input2, input3)
    local newInstance = {}
    setmetatable( newInstance, 
    { 
        __index = Disc,
        __eq = DoTwoDiscsEqual,
        __concat = Vector2ConcatWithString
    })

    -- if you add a copy constructor, dont just set them equal to another
    -- since thats a reference and not a copy
    
    -- figure out if we are using a vec2 to create or x,y,r
    -- gotta do this cause lua doesn't support overloading
    if type(input1) == "table" then
        newInstance.center = Vector2:Create(input1.x, input1.y)
        newInstance.radius = input2
    else
        newInstance.center = Vector2:Create(input1,input2)
        newInstance.radius = input3
    end

    return newInstance
end

-------------------------------------------------------------------------
function Disc:IsPointInside(input1,input2)
    if type(input1) == "table" then
        return IsPointInsideDisc(self.center.x, self.center.y, self.radius, input1.x, input1.y)
    else
        return IsPointInsideDisc(self.center.x, self.center.y, self.radius, input1, input2)
    end
end

-------------------------------------------------------------------------
function Disc:ToString()
    return "Center.x is: " .. self.center.x .. " Center.y is: " .. self.center.y .. " Radius is: " .. self.radius
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function DiscConcactWithString(theString, disc)
    return theString .. disc:ToString()
end

-------------------------------------------------------------------------
function DoTwoDiscsEqual(discA, discB)
    return ((discA.center == discB.center) and (discA.radius == discB.radius))
end
 
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
AABB2 = 
{
    mins = {x = 0, y = 0},
    maxs = {x = 1, y = 1}
}

-------------------------------------------------------------------------
-- YOU CAN NOT JUST ASSIGN THEM TO MINS,MAXS IF ITS A VECTOR
-- BECAUSE ITS NOT A COPY, ITS A REFERENCE AND IF YOU CHANGE THE MINS
-- YOU WILL CHANGE THE VALUES YOU PASSED IN DURING CONSTRUCTION AS WELL
-- think of them as needing to be const
function AABB2:Create(input1, input2, input3, input4)
    local newInstance = {}
    setmetatable( newInstance, 
    { 
        __index = AABB2,
        __eq = DoesAABB2EqualAABB2,
        __concat = AABB2ConcactWithString
    })

    -- if you add a copy constructor, dont just set them equal to another
    -- since thats a reference and not a copy

    if type(input1) == "table" and type(input2) == "table" then 
        newInstance.mins = Vector2:Create(input1.x, input1.y) 
        newInstance.maxs = Vector2:Create(input2.x, input2.y)
    
    elseif type(input1) == "table" and type(input2) ~= "table" then
        newInstance.mins = Vector2:Create(input1.x, input1.y)
        newInstance.maxs = Vector2:Create(input2, input3)

    elseif type(input1) ~= "table" and type(input3) == "table" then
        newInstance.mins = Vector2:Create(input1, input2)
        newInstance.maxs = Vector2:Create(input3.x, input3.y)

    else
        newInstance.mins = Vector2:Create(input1, input2)
        newInstance.maxs = Vector2:Create(input3,input4)
    end

    return newInstance
end

-------------------------------------------------------------------------
function AABB2:GetWidth()
    return Abs(self.maxs.x - self.mins.x)
end

-------------------------------------------------------------------------
function AABB2:GetHeight()
    return Abs(self.maxs.y - self.mins.y)
end

-------------------------------------------------------------------------
function AABB2:GetDimensions()
    return Vector2:Create(self:GetWidth(), self:GetHeight())
end

-------------------------------------------------------------------------
function AABB2:GetPositionInside(vec2Percent)
    posx,posy = AABB2GetPositionInside(self.mins.x, self.mins.y, self.maxs.x, self.maxs.y, vec2Percent.x, vec2Percent.y)

    return Vector2:Create(posx, posy)
end

-------------------------------------------------------------------------
function AABB2:IsPointInside(vec2Point)
    return AABB2IsPointInside(self.mins.x, self.mins.y, self.maxs.x, self.maxs.y, vec2Point.x, vec2Point.y)
end

-------------------------------------------------------------------------
function AABB2:GrowToSquare()
    self.mins.x, self.mins.y, self.maxs.x, self.maxs.y = AABB2GrowToSquare(self.mins.x, self.mins.y, self.maxs.x, self.maxs.y)
end

-------------------------------------------------------------------------
function AABB2:GetBiggerBox()
    copyOfSelf = AABB2:Create(self.mins, self.maxs)
    copyOfSelf:GrowToSquare()
    return copyOfSelf
end

-------------------------------------------------------------------------
function AABB2:ShrinkToSquare()
    self.mins.x, self.mins.y, self.maxs.x, self.maxs.y = AABB2ShrinkToSquare(self.mins.x, self.mins.y, self.maxs.x, self.maxs.y)
end

-------------------------------------------------------------------------
function AABB2:GetSmallerBox()
    copyOfSelf = AABB2:Create(self.mins, self.maxs)
    copyOfSelf:ShrinkToSquare()
    return copyOfSelf
end

-------------------------------------------------------------------------
function AABB2:GetAABB2FromSelf(vec2MinPercentage, vec2MaxPercentage)
    newminx, newminy, newmaxx, newmaxy = AABB2GetAABB2FromAABB2(vec2MinPercentage.x, vec2MinPercentage.y, vec2MaxPercentage.x, vec2MaxPercentage.y, self.mins.x, self.mins.y, self.maxs.x, self.maxs.y)

    return AABB2:Create(newminx, newminy,newmaxx, newmaxy)
end

-------------------------------------------------------------------------
function AABB2:ToString() 
    local stringmin = "Mins.x is: " .. self.mins.x .. " Mins.y is: " .. self.mins.y
    local stringmax = "Maxs.x is: " .. self.maxs.x .. " Maxs.y is: " .. self.maxs.x
    return stringmin .. " " .. stringmax
end

-------------------------------------------------------------------------
-- Operators
-------------------------------------------------------------------------
function DoesAABB2EqualAABB2(boxA, boxB)
    return (boxA.mins == boxB.mins) and (boxA.maxs == boxB.maxs)
end

-------------------------------------------------------------------------
function AABB2ConcactWithString(theString, box)
    return theString .. box:ToString()
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function GetAABB2FromAABB2(vec2MinPercentage, vec2MaxPercentage, theBounds)
    return theBounds:GetAABB2FromSelf(vec2MinPercentage,vec2MaxPercentage)
end

-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_allEntities = {}
-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Entity = 
{
    pos = Vector2:Create(0,0),
    maxHealth = 100,
    health = 100,
    sprite = 0,
    speed = 60,
    name = "idk",
    collisionRadius = 6,
    hasCollision = true,
    isDead = false,
    rotationAmount = 0,
    attackDamage = 30
}

-------------------------------------------------------------------------
function Entity:Add(anEntity, index)
    if index ~= nil then
        g_allEntities[index] = anEntity
    else
        table.insert(g_allEntities, anEntity)
    end
end

-------------------------------------------------------------------------
function Entity:Remove()
    for k,v in ipairs(g_allEntities) do
        if self == v then
            table.remove(g_allEntities, k)
            self = nil
        end
    end
end

-------------------------------------------------------------------------
function Entity:Heal(amount)
    self.health = Clamp(self.health + amount,0, self.maxHealth)
    HealParticle:Create(self)
end

-------------------------------------------------------------------------
function Entity:TakeDamage(amount)
    if self.isDead == true then return end
    
    self.health = self.health - amount

    if self.name ~= "boss" then
        PlayOneShot("damage.wav", .5)
    end

    if self.name == "player" then
        self:Interrupt()
    end

    if self.health <= 0 then 
        self.isDead = true 
        PlayOneShot("death.wav")
    end
end

-------------------------------------------------------------------------
function Entity:Attack()
    self.target:TakeDamage(self.attackDamage)
    self.rotationAmount = 1
end

-------------------------------------------------------------------------
function Entity:AnimateMovement(didWeMove, ds)
    if(didWeMove) then
        self.rotationAmount = Clamp(self.rotationAmount + ds *3, 0, 1)
    else
        self.rotationAmount = Clamp(self.rotationAmount - ds * 3, 0, 1)
    end
end

-------------------------------------------------------------------------
function Entity:CalculateRotationAmount()
    amount = (SinDegrees(g_totalTime * 1000) * self.rotationAmount) * 30
    return amount
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function UpdateAllEntities(ds)
    for e in all(g_allEntities) do
        e:Update(ds)
    end
end

-------------------------------------------------------------------------
function DrawAllEntities()
    if g_currentState == "playing" then g_boss:Draw() end
    
    for e in all(g_allEntities) do
        if e.name ~= "boss" then 
            e:Draw()
        end
    end
    
end

-------------------------------------------------------------------------
function ClearEntityTable()
    for k,v in pairs(g_allEntities) do g_allEntities[k]=nil end
end
 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Player = 
{
}
g_player = {}

-------------------------------------------------------------------------
function Player:Create(x,y)
    local newinst = {}
    
    setmetatable( Player, { __index = Entity } )
    setmetatable( newinst, { __index = Player } )

    newinst.sprite = 0
    newinst.deathSprite = 25
    newinst.pos = Vector2:Create(x,y)
    newinst.maxHealth = 100
    newinst.health = newinst.maxHealth
    
    newinst.maxMana = 100
    newinst.mana = newinst.maxMana
    newinst.manaRegen = .1
    newinst.name = "player"

    g_player = newinst
    Entity:Add(g_player, #g_allEntities + 1)
    return newinst
end

-------------------------------------------------------------------------
function Player:Update(ds)
    if self.isDead == true then return end

    didWeMove = self:UpdateMovement(ds)

    if g_castingSpell ~= nil then
        if didWeMove or self:DidWeCancelSpell() then
            self:Interrupt()
        else
            g_castingSpell:Update(ds)
        end
    elseif WasMouseButtonJustReleased("l") then
        if self:CanCast() then
            g_castingSpell = SpellCaster:Create(g_theHud:GetSelectedSpellDef().name, g_theHud:GetSelectedPartyMember(), self)
            g_theHud:ClearInterrupt()
        elseif g_theHud:IsMouseOnHealthBar() == true then
            self:DisplayWhyWeCantCast()
        end
    end

    self.mana = Clamp(self.mana + self.manaRegen, 0, self.maxMana)

    self:AnimateMovement(didWeMove, ds)
end

-------------------------------------------------------------------------
function Player:DisplayWhyWeCantCast()
    if self:HasEnoughManaToCast() == false then
        g_theHud:ShowCastErrorMessage("Not enough mana!")
    end
    
    if g_theHud:IsSelectedSpellOnCooldown() then
        g_theHud:ShowCastErrorMessage("On Cooldown!")
    end

    if g_theHud:IsTargetDead() then
        g_theHud:ShowCastErrorMessage("they ded")
    end

    -- todo dead (LOS)
end

-------------------------------------------------------------------------
function Player:CanCast()
    return self:HasEnoughManaToCast() and g_theHud:IsMouseOnHealthBar() and (not g_theHud:IsSelectedSpellOnCooldown()) and (not g_theHud:IsTargetDead())
end

-------------------------------------------------------------------------
function Player:HasEnoughManaToCast()
    spellDef = g_theHud:GetSelectedSpellDef()
    return spellDef.manaCost <= self.mana
end

-------------------------------------------------------------------------
function Player:DidWeCancelSpell()
    return WasMouseButtonJustReleased("l") or WasMouseButtonJustReleased("r") or WasKeyJustReleased("space")
end

-------------------------------------------------------------------------
function Player:UpdateMovement(ds)
    
    dir = Vector2:Create(0,0)
    if IsKeyPressed("w") or IsKeyPressed("up") then
        dir = dir + Vector2:Create(0,1)
    end

    if IsKeyPressed("s") or IsKeyPressed("down") then
        dir = dir + Vector2:Create(0,-1)
    end

    if IsKeyPressed("a") or IsKeyPressed("left") then
        dir = dir + Vector2:Create(-1,0)
    end
    
    if IsKeyPressed("d") or IsKeyPressed("right") then
        dir = dir + Vector2:Create(1,0)
    end

    self.pos = self.pos + (dir * self.speed * ds)

    if dir == Vector2:Create(0,0) then return false end
    return true
end

-------------------------------------------------------------------------
function Player:Interrupt()
    g_castingSpell = nil
    g_theHud:Interrupt()
    PlayOneShot("ErrorCast.wav")
end

-------------------------------------------------------------------------
function Player:GetPercentLeftOfMana()
    return self.mana / self.maxMana
end

-------------------------------------------------------------------------
function Player:Draw()
    if self.isDead == false then
        DrawSprite(self.sprite, self.pos.x, self.pos.y, self:CalculateRotationAmount())
        DrawSprite(self.sprite + 16, self.pos.x, self.pos.y + 16)
    else
        DrawSprite(self.deathSprite, self.pos.x, self.pos.y)
    end

    
    --DrawCircle(self.pos.x, self.pos.y, self.collisionRadius, "green")
end
-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
SlimeOrb = 
{
    outOfBounds = AABB2:Create(-20,-20,280,280),
    damage = 10
}

-------------------------------------------------------------------------
function SlimeOrb:Create(x,y, dirx, diry)
    local newInstance = {}
    setmetatable(SlimeOrb, {__index = Entity})
    setmetatable(newInstance, {__index = SlimeOrb})

    newInstance.pos = Vector2:Create(x,y)
    newInstance.direction = Vector2:Create(dirx, diry)
    newInstance.speed = 50

    newInstance.sprite = 5
    newInstance.currentSprite = newInstance.sprite
    newInstance.animationLength = .5
    newInstance.animationTimer = 0

    newInstance.collisionRadius = 4
    newInstance.hasCollision = false -- they dont collide with each other

    Entity:Add(newInstance)

    return newInstance
end

-------------------------------------------------------------------------
function SlimeOrb:Destroy(wasPopped)
    -- P O P
    
    if wasPopped then
        PlayOneShot("pop.wav")
    end

    self:Remove()
end

-------------------------------------------------------------------------
function SlimeOrb:Update(ds)
    self.pos = self.pos + (self.direction * self.speed * ds)
    self:UpdateAnimation(ds)

    self:CheckForCollisions()
    self:CheckOutOfBounds()
end

-------------------------------------------------------------------------
function SlimeOrb:CheckOutOfBounds()
    if not self.outOfBounds:IsPointInside(Vector2:Create(self.pos.x, self.pos.y)) then
        self:Destroy(false)
    end
end

-------------------------------------------------------------------------
function SlimeOrb:CheckForCollisions()
    slimeDisc = Disc:Create(self.pos, self.collisionRadius)

    -- maybe need to use a different table that is just the players
    for entity in all(g_allEntities) do
        if entity.hasCollision == true and entity.isDead == false then
            entityDisc = Disc:Create(entity.pos, entity.collisionRadius)

            if DoesDiscOverlapDisc(slimeDisc, entityDisc) then
                entity:TakeDamage(self.damage)
                self:Destroy(true)
            end
        end
    end
end

-------------------------------------------------------------------------
function SlimeOrb:UpdateAnimation(ds)
    if self.animationTimer > self.animationLength then
        if self.currentSprite == self.sprite then
            self.currentSprite = self.sprite + 1
        else
            self.currentSprite = self.sprite
        end
        self.animationTimer = 0
    else
        self.animationTimer = self.animationTimer + ds
    end
end

-------------------------------------------------------------------------
function SlimeOrb:Draw()
    DrawSprite(self.currentSprite, self.pos.x, self.pos.y)

    --DrawCircle(self.pos.x, self.pos.y, self.collisionRadius, "white")
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Boss = {}
g_boss = {}

-- normal, start_circle_oozes_1, circle_oozes_1, start_beam_oozes_1, beam_oozes_1
g_combatState = "normal"

-------------------------------------------------------------------------
function Boss:Create(x,y)
    local newinst = {}
    
    setmetatable( Boss, { __index = Entity } )
    setmetatable( newinst, { __index = Boss } )

    newinst.sprite = 0
    newinst.pos = Vector2:Create(x,y)
    newinst.maxhealth = 1000
    newinst.health = newinst.maxhealth
    newinst.hasCollision = false
    newinst.name = "boss"

    newinst.normalAttackCooldown = 2
    newinst.normalAttackcooldownTimer = 0
    
    newinst.beamAngle = 0
    newinst.beamAttackCooldown = .5
    newinst.beamAttackTimer = 0

    newinst.circleAttackCooldown = 2
    newinst.circleAttackTimer = newinst.circleAttackCooldown

    newinst.casting = false
    newinst.castTimer = 0
    newinst.castLength = 3

    newinst.phaseTimer = 0
    newinst.finishedSpell = false

    newinst.collisionBounds = AABB2:Create(x - 40, y - 32, x + 40, y + 32)

    newinst.updateFunctions =
    {
        normal = BossDefaultCombatUpdate,
        start_circle_oozes_1 = BossStartCircleOozes1, 
        circle_oozes_1 = BossCircleOozes1,
        start_beam_oozes_1 = BossStartBeamOozes,
        beam_oozes_1 = BossBeamOozes
    }

    -- 1 = 2, 2 = 3, 3 = 4, 4 = 5, 5 = 1
    newinst.attackPriority = {}
    newinst.attackPriority[1] = 2
    newinst.attackPriority[2] = 3
    newinst.attackPriority[3] = 4
    newinst.attackPriority[4] = 5
    newinst.attackPriority[5] = 1

    g_combatState = "normal"

    g_boss = newinst
    Entity:Add(g_boss)
    
    return newinst
end

-------------------------------------------------------------------------
function Boss:Update(ds)
    --self.health = Clamp(self.health - 1, 0, self.maxhealth)
    self.updateFunctions[g_combatState](ds)

    if self.casting then self:UpdateCasting(ds) end
    self:AnimateMovement(false, ds)
    
    --dPrint(g_combatState)
end

-------------------------------------------------------------------------
function Boss:UpdateCasting(ds)
    if self.castTimer >= self.castLength then 
        self:Cast()
        self.castTimer = 0
        self.casting = false
    else
        self.castTimer = self.castTimer + ds
    end
end

-------------------------------------------------------------------------
function Boss:Cast()
    if g_combatState == "start_circle_oozes_1" then g_combatState = "circle_oozes_1" end
    if g_combatState == "start_beam_oozes_1" then g_combatState = "beam_oozes_1" end

    g_boss.finishedSpell = false
end

-------------------------------------------------------------------------
function Boss:SelectAttack()
    chance = RandomRange(0,100)
    if chance < 50 then 
        g_combatState = "start_circle_oozes_1"
    else
        g_combatState = "start_beam_oozes_1"
    end

end

-------------------------------------------------------------------------
function Boss:GetPercentHealthLeft()
    return self.health / self.maxhealth
end

-------------------------------------------------------------------------
function Boss:Draw()
    if self.health <= 0 then return end
    
    --DrawCircleFill(self.pos.x, self.pos.y, 24, "red")
    DrawSprite(32, self.pos.x, self.pos.y, self:CalculateRotationAmount(),4,3)

    if self.casting then self:DrawCastBar() end

    if g_combatState == "normal" then
        -- Draw who we are attacking?
        --DrawCircle(self.target.pos.x, self.target.pos.y, 12, "red")
        --DrawCircleFill(self.target.pos.x, self.target.pos.y - 12, 4, "red")
    end

    --Berry.DrawAABB2(self.collisionBounds, "white")
end

-------------------------------------------------------------------------
function Boss:DrawCastBar()
    percent = self.castTimer / self.castLength

    g_theHud:DrawBossCastBar(self.spellName, percent)
end

-------------------------------------------------------------------------
function Boss:GetTarget()
    for i=1, 5 do 
        whoDoWeWant = self.attackPriority[i]
        if g_theHud.party[whoDoWeWant].isDead == false then 
            return g_theHud.party[whoDoWeWant]
        end
    end

    return nil
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function BossDefaultCombatUpdate(ds)
    g_boss.target = g_boss:GetTarget()
    if g_boss.target == nil then return end

    if g_boss.normalAttackcooldownTimer > g_boss.normalAttackCooldown then
        g_boss:Attack()
        --PlayOneShot("slimeAttack.wav", .5)
        g_boss.normalAttackcooldownTimer = 0
    else
        g_boss.normalAttackcooldownTimer = g_boss.normalAttackcooldownTimer + ds
    end


    if g_boss.phaseTimer >= 10 then
        g_boss:SelectAttack()
        g_boss.phaseTimer = 0
    else
        g_boss.phaseTimer = g_boss.phaseTimer + ds
    end
end

-------------------------------------------------------------------------
function BossStartCircleOozes1(ds)
    if g_boss.casting == false then
        g_boss.casting = true
        g_boss.spellName = "Circle Of Oozes"
        g_boss.castTimer = 0
        g_boss.circleAttackTimer = g_boss.circleAttackCooldown
    end
end

-------------------------------------------------------------------------
function BossCircleOozes1(ds)
    if g_boss.circleAttackTimer >= g_boss.circleAttackCooldown then
        SpawnCircleOfOrbs()
        g_boss.circleAttackTimer = 0
    else
        g_boss.circleAttackTimer = g_boss.circleAttackTimer + ds
    end


    if g_boss.phaseTimer >= 10 then
        g_combatState = "normal"
        g_boss.phaseTimer = 0
    else
        g_boss.phaseTimer = g_boss.phaseTimer + ds
    end
    
end

-------------------------------------------------------------------------
function SpawnCircleOfOrbs()
    currentAngle = RandomRange(0,360)
    for i=0, 12 do 
        direction = Vector2:Create(CosDegrees(currentAngle), SinDegrees(currentAngle))

        SlimeOrb:Create(128,128, direction.x, direction.y)

        currentAngle = currentAngle + 30
    end
end

-------------------------------------------------------------------------
function BossStartBeamOozes(ds)
    if g_boss.casting == false then
        g_boss.casting = true
        g_boss.spellName = "Beam Of Oozes"
        g_boss.castTimer = 0
        g_boss.beamAngle = 0
    end
end

-------------------------------------------------------------------------
function BossBeamOozes(ds)
    if g_boss.beamAttackTimer >= g_boss.beamAttackCooldown then
        g_boss.beamAngle = g_boss.beamAngle + (ds * 500)

        direction = Vector2:Create(CosDegrees(g_boss.beamAngle), SinDegrees(g_boss.beamAngle))
        SlimeOrb:Create(128,128, direction.x, direction.y)

        direction = Vector2:Create(CosDegrees(g_boss.beamAngle + 90), SinDegrees(g_boss.beamAngle + 90))
        SlimeOrb:Create(128,128, direction.x, direction.y)

        direction = Vector2:Create(CosDegrees(g_boss.beamAngle + 180), SinDegrees(g_boss.beamAngle + 180))
        SlimeOrb:Create(128,128, direction.x, direction.y)

        direction = Vector2:Create(CosDegrees(g_boss.beamAngle + 270), SinDegrees(g_boss.beamAngle + 270))
        SlimeOrb:Create(128,128, direction.x, direction.y)

        g_boss.beamAttackTimer = 0
    else
        g_boss.beamAttackTimer = g_boss.beamAttackTimer + ds
    end


    if g_boss.beamAngle > 360 then
        g_combatState = "normal"
    end
end
 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
HUD = 
{
    bounds = GetAABB2FromAABB2(Vector2:Create(.2,.0), Vector2:Create(.8,.2), AABB2:Create(0,0,256,256)),
    castBounds = GetAABB2FromAABB2(Vector2:Create(.3,.21), Vector2:Create(.7,.25), AABB2:Create(0,0,256,256)),
    manaBounds = GetAABB2FromAABB2(Vector2:Create(.15,0), Vector2:Create(.85,.2), AABB2:Create(0,0,256,256)),
    enemyBounds = GetAABB2FromAABB2(Vector2:Create(.2,.93), Vector2:Create(.8,.98), AABB2:Create(0,0,256,256)),
    enemyCastBounds = GetAABB2FromAABB2(Vector2:Create(.25,.88), Vector2:Create(.75,.92), AABB2:Create(0,0,256,256)),

    spells = {},
    party = {},
    spellSlots = {},

    selectedSpellIndex = 1,
    selectedPartyMember = 1,

    showErrorMessage = false,
    popUpCurrentTimer = 0,
    popUpLength = 1,
    errorText = "idk"
}
g_theHud = {}

-------------------------------------------------------------------------
function HUD:Create(party, spellTable)
    local newInst = {}
    setmetatable( newInst, { __index = HUD } )

    newInst:CreateAllBounds()

    newInst:AddParty(party)
    newInst:AddSpells(spellTable)

    g_theHud = newInst
    return newInst
end

-------------------------------------------------------------------------
function HUD:CreateAllBounds()
    self.hpBars = {}
    self.spellBars = {}
    
    for i=1,5 do
        minx = (i - 1) * .2
        maxx = i * .2

        self.hpBars[i] = GetAABB2FromAABB2(Vector2:Create(minx,.5), Vector2:Create(maxx,1), self.bounds)
        self.spellBars[i] = GetAABB2FromAABB2(Vector2:Create(minx,0), Vector2:Create(maxx,.5), self.bounds)
    end
    
end

-------------------------------------------------------------------------
function HUD:AddParty(party)
    for k,v in ipairs(party) do
        self.party[k] = v
    end
end

-------------------------------------------------------------------------
function HUD:AddSpells(spells)
    for k,v in ipairs(spells) do
        self.spells[k] = v
    end

    -- Maybe lets create a struct that holds a CD and the spell def
    -- that way we have a place to store a CD for each slot
    self.spellSlots[1] = { cooldown = 0, spellDef = self.spells[1]}
    self.spellSlots[2] = { cooldown = 0, spellDef = self.spells[2]}
    self.spellSlots[3] = { cooldown = 0, spellDef = self.spells[3]}
    self.spellSlots[4] = { cooldown = 0, spellDef = self.spells[4]}
    self.spellSlots[5] = { cooldown = 0, spellDef = self.spells[5]}

end

-------------------------------------------------------------------------
function HUD:GetSelectedSpellDef()
    return self.spells[self.selectedSpellIndex]
end

-------------------------------------------------------------------------
function HUD:GetSelectedPartyMember()
    return self.party[self.selectedPartyMember]
end

-------------------------------------------------------------------------
function HUD:IsSelectedSpellOnCooldown()
    return self.spellSlots[self.selectedSpellIndex].cooldown > 0
end

-------------------------------------------------------------------------
function HUD:IsTargetDead()
    return self.party[self.selectedPartyMember].isDead
end

-------------------------------------------------------------------------
function HUD:Update(ds)

    if WasMouseButtonJustReleased("l") then
        self:LeftClick()
    end

    if self.showErrorMessage then
        if self.popUpCurrentTimer >= self.popUpLength then
            self.showErrorMessage = false
        else
            self.popUpCurrentTimer = self.popUpCurrentTimer + ds 
        end
    end

    self:HandleCooldowns(ds)

    --dPrint(self:GetSelectedSpellDef().name)
end

-------------------------------------------------------------------------
function HUD:HandleCooldowns(ds)
    for slot in all(self.spellSlots) do
        if slot.cooldown > 0 then
            slot.cooldown = slot.cooldown - ds
        end
    end
end

-------------------------------------------------------------------------
function HUD:SetCooldownForSelectedSpell(cooldown)
    self.spellSlots[self.selectedSpellIndex].cooldown = cooldown
end

-------------------------------------------------------------------------
function HUD:Interrupt()
    self:ShowCastErrorMessage("Failed :(")
end

-------------------------------------------------------------------------
function HUD:ClearInterrupt()
    self:ClearErrorMessage()
end

-------------------------------------------------------------------------
function HUD:ShowCastErrorMessage(message)
    self.showErrorMessage = true
    self.popUpCurrentTimer = 0
    self.errorText = message
end

-------------------------------------------------------------------------
function HUD:ClearErrorMessage()
    self.showErrorMessage = false
    self.popUpCurrentTimer = 0
    self.errorText = "idk"
end

-------------------------------------------------------------------------
function HUD:IsMouseOnHealthBar()
    allHealthBarsBounds = GetAABB2FromAABB2(Vector2:Create(0,.5), Vector2:Create(1,1), self.bounds)
    return allHealthBarsBounds:IsPointInside(Berry.GetMousePosition())
end

-------------------------------------------------------------------------
function HUD:LeftClick()
    mousePos = Berry.GetMousePosition()
    for i=1,5 do
        if self.hpBars[i]:IsPointInside(mousePos) then
            PlayOneShot("ui.wav")
            self.selectedPartyMember = i
            return
        end

        if self.spellBars[i]:IsPointInside(mousePos) and g_castingSpell == nil then
            PlayOneShot("ui.wav")
            self.selectedSpellIndex = i
            return
        end
    end
end

-------------------------------------------------------------------------
function HUD:Render()
    self:DrawManaBar()
    Berry.DrawAABB2(self.bounds, "white")
    Berry.DrawAABB2Fill(self.bounds, "black")
    
    for i=1,5 do
        self:DrawSpellBox(i, self.spellBars[i])
        self:DrawHPBar(i, self.hpBars[i])
    end
    self:DrawHightlightSelection()
    self:DrawCastBar()
    self:DrawEnemyHealthBar()

    if self.showErrorMessage then
        self:DrawSpellErrorBar()
    end

    --self:DrawBossCastBar("spell", Fract(g_totalTime))
end

-------------------------------------------------------------------------
function HUD:DrawHightlightSelection()
    Berry.DrawAABB2(self.hpBars[self.selectedPartyMember], "random")
    Berry.DrawAABB2(self.spellBars[self.selectedSpellIndex], "random")
end

-------------------------------------------------------------------------
function HUD:DrawHPBar(index, bounds)
    member = self.party[index]
    
    if member ~= nil and member.isDead == false then
        width = Lerp(0,1,(member.health / member.maxHealth))
        healthBar = bounds:GetAABB2FromSelf(Vector2:Create(0,0), Vector2:Create(width, 1))
        
        Berry.DrawAABB2Fill(healthBar,255,0,0,150)
        DrawSprite(member.sprite + 16, healthBar.mins.x + 16, healthBar.mins.y + 12)
    end
    Berry.DrawAABB2(bounds, "white") 
end

-------------------------------------------------------------------------
function HUD:DrawManaBar()
    percent = g_player:GetPercentLeftOfMana()

    manaBar = self.manaBounds:GetAABB2FromSelf(Vector2:Create(0,0), Vector2:Create(1, percent))

    Berry.DrawAABB2Fill(manaBar, "blue")
    Berry.DrawAABB2(self.manaBounds, "white")
end

-------------------------------------------------------------------------
function HUD:DrawSpellBox(index, bounds)
    Berry.DrawAABB2(bounds, "white")

    DrawSprite(self.spellSlots[index].spellDef.sprite, bounds.mins.x + 16, bounds.mins.y + 12)
    
    currentCD = self.spellSlots[index].cooldown
    if currentCD > 0 then
        Berry.DrawAABB2Fill(bounds, 149,165,166, 200)

        if currentCD >= 9 then 
            DrawText(math.floor(currentCD + 1), bounds.mins.x + 7, bounds.mins.y + 10, 5)
        else
            DrawText(math.floor(currentCD + 1), bounds.mins.x + 12, bounds.mins.y + 10, 5)
        end
    end
end

-------------------------------------------------------------------------
function HUD:DrawCastBar()
    if g_castingSpell == nil then return end
    if g_castingSpell.spellDef.castTime == 0 then return end

    percentDone = g_castingSpell:GetPercentDone()
    progressBar = self.castBounds:GetAABB2FromSelf(Vector2:Create(0,0), Vector2:Create(percentDone, 1))

    Berry.DrawAABB2Fill(self.castBounds, "black")
    Berry.DrawAABB2Fill(progressBar, "blue")
    Berry.DrawAABB2(self.castBounds, "white")
    Berry.DrawTextWrapped(g_castingSpell:GetSpellName(), self.castBounds, 4)
end

-------------------------------------------------------------------------
function HUD:DrawSpellErrorBar()
    Berry.DrawAABB2Fill(self.castBounds, "red")
    Berry.DrawAABB2(self.castBounds, "white")
    Berry.DrawTextWrapped(self.errorText, self.castBounds, 3)
end

-------------------------------------------------------------------------
function HUD:DrawEnemyHealthBar()
    Berry.DrawAABB2Fill(self.enemyBounds, "black")
    
    percent = g_boss:GetPercentHealthLeft()
    healthbar = self.enemyBounds:GetAABB2FromSelf(Vector2:Create(0,0), Vector2:Create(percent, 1))
    Berry.DrawAABB2Fill(healthbar, "red")
    
    Berry.DrawAABB2(self.enemyBounds, "white")
    pos = self.enemyBounds:GetPositionInside(Vector2:Create(.35,.25))
    DrawText("Boss", pos.x, pos.y, 6)
end

-------------------------------------------------------------------------
function HUD:DrawBossCastBar(text, percent)
    Berry.DrawAABB2Fill(self.enemyCastBounds, "black")

    fillBar = self.enemyCastBounds:GetAABB2FromSelf(Vector2:Create(0,0), Vector2:Create(percent, 1))
    Berry.DrawAABB2Fill(fillBar, "blue")

    pos = self.enemyCastBounds:GetPositionInside(Vector2:Create(.05, .20))
    DrawText(text, pos.x, pos.y, 4)

    Berry.DrawAABB2(self.enemyCastBounds, "white")
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
SpellDefinition = 
{
    name = "idk",
    healingAmount = 10,
    castTime = 0,
    lifeTime = 1,
    onCastFunction = nil,
    onUpdateFunction = nil,
    drawFunction = nil,
}
g_allSpellDefinitions = {}
g_selectedSpells = {}

-------------------------------------------------------------------------
function SpellDefinition:Create(name)
    local newInstance = {}
    setmetatable(newInstance, { __index = SpellDefinition })

    newInstance.name = name

    g_allSpellDefinitions[name] = newInstance
    return newInstance
end

-------------------------------------------------------------------------
function SpellDefinition:CreateAll()
    newInstance = SpellDefinition:Create("Renew")
    newInstance.amount = 1.2
    newInstance.manaCost = 5
    newInstance.castTime = 0
    newInstance.cooldown = 2
    newInstance.lifetime = 5
    newInstance.onCastFunction = nil
    newInstance.onUpdateFunction = HealingOverTime
    newInstance.drawFunction = nil
    newInstance.sprite = 11
    g_selectedSpells[1] = newInstance

    newInstance = SpellDefinition:Create("Heal")
    newInstance.amount = 5
    newInstance.manaCost = 5
    newInstance.castTime = 2
    newInstance.cooldown = 0
    newInstance.lifetime = 0
    newInstance.onCastFunction = SingleTargetHeal
    newInstance.onUpdateFunction = nil
    newInstance.drawFunction = nil
    newInstance.sprite = 12
    g_selectedSpells[2] = newInstance

    newInstance = SpellDefinition:Create("Quick")
    newInstance.amount = 60
    newInstance.manaCost = 10
    newInstance.castTime = 1
    newInstance.cooldown = 2
    newInstance.lifetime = 0
    newInstance.onCastFunction = SingleTargetHeal
    newInstance.onUpdateFunction = nil
    newInstance.drawFunction = nil
    newInstance.sprite = 13
    g_selectedSpells[3] = newInstance

    newInstance = SpellDefinition:Create("Big Heal")
    newInstance.amount = 100
    newInstance.manaCost = 8
    newInstance.castTime = 3.5
    newInstance.cooldown = 0
    newInstance.lifetime = 0
    newInstance.onCastFunction = SingleTargetHeal
    newInstance.onUpdateFunction = nil
    newInstance.drawFunction = nil
    newInstance.sprite = 14
    g_selectedSpells[4] = newInstance

    newInstance = SpellDefinition:Create("AOE")
    newInstance.amount = 30
    newInstance.manaCost = 12
    newInstance.castTime = 2.5
    newInstance.cooldown = 10
    newInstance.lifetime = 0
    newInstance.onCastFunction = AOEHeal
    newInstance.onUpdateFunction = nil
    newInstance.drawFunction = nil
    newInstance.sprite = 15
    g_selectedSpells[5] = newInstance
end

-------------------------------------------------------------------------
-- Spell
-------------------------------------------------------------------------
Spell = 
{
    currentLifetime = 0,
    pos = Vector2:Create(0,0)
}

-------------------------------------------------------------------------
function Spell:Create(definition, target)
    local newInstance = {}
    
    setmetatable(Spell, { __index = Entity})
    setmetatable(newInstance, { __index = Spell})
   
    newInstance.target = target
    newInstance.definition = definition
    newInstance.hasCollision = false

    newInstance.healTimer = 0
    newInstance.hotCooldown = .2

    if newInstance.definition == nil then
        dPrint("fuck",3)
    end

    PlayOneShot("heal.wav")

    Entity:Add(newInstance)
    return newInstance;
end

-------------------------------------------------------------------------
function Spell:Destroy()
    self:Remove()
end

-------------------------------------------------------------------------
function Spell:OnCast()
    --dPrint("cast:" .. self.definition.name, 1)
    if self.definition.onCastFunction then
        self.definition.onCastFunction(self)
    end
end

-------------------------------------------------------------------------
function Spell:Update(ds)
    if self.currentLifetime >= self.definition.lifetime then
        self:Destroy()
        return 
    end
    
    if self.definition.onUpdateFunction then
        self.definition.onUpdateFunction(self, ds)
    end
    
    self.currentLifetime = self.currentLifetime + ds
end

-------------------------------------------------------------------------
function Spell:Draw()
    if self.definition.drawFunction then
        self.definition.drawFunction(self)
    end
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function SingleTargetHeal(spell, ds)
    spell.target:Heal(spell.definition.amount)
end

-------------------------------------------------------------------------
function HealingOverTime(spell, ds)

    if spell.healTimer >= spell.hotCooldown then
        SingleTargetHeal(spell)
        spell.healTimer = 0
    else
        spell.healTimer = spell.healTimer + ds
    end

end

-------------------------------------------------------------------------
function AOEHeal(spell, ds)
    -- picks the three lowest and heals them
    for member in all(g_theHud.party) do 
        member:Heal(spell.definition.amount) 
    end
end

-------------------------------------------------------------------------
function SetSpellLocationToTarget(spell, ds)
    spell.pos = Vector2:Create(spell.target.pos.x, spell.target.pos.y)
end

-------------------------------------------------------------------------
function DrawAOECircle(spell, ds)
    DrawCircleFill(spell.pos.x, spell.pos.y, 12, 0,128,225,200)
end 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
HealParticle = {}
g_allParticles = {}


-------------------------------------------------------------------------
function HealParticle:Create(target)
    local newinst = {}
    setmetatable(newinst, {__index = HealParticle})

    newinst.sprite = 7
    newinst.currentSprite = newinst.sprite
    newinst.animationLength = .1
    newinst.animationTimer = 0

    newinst.lifetime = .3
    newinst.lifetimeTimer = 0

    newinst.target = target

    table.insert(g_allParticles, newinst)
end

-------------------------------------------------------------------------
function HealParticle:Add(particle)
    table.insert(g_allParticles, particle)
end

-------------------------------------------------------------------------
function HealParticle:Remove()
    for k,v in ipairs(g_allParticles) do
        if self == v then
            table.remove(g_allParticles, k)
            self = nil
        end
    end
end

-------------------------------------------------------------------------
function HealParticle:Update(ds)
    if self.animationTimer >= self.animationLength then
        self:UpdateCurrentSprite()
        self.animationTimer = 0
    else
        self.animationTimer = self.animationTimer + ds
    end

    if self.lifetimeTimer >= self.lifetime then
        self:Remove()
    else
        self.lifetimeTimer = self.lifetimeTimer + ds
    end
end

-------------------------------------------------------------------------
function HealParticle:UpdateCurrentSprite()
    if self.currentSprite == self.sprite then self.currentSprite = self.sprite + 1 return end
    if self.currentSprite == self.sprite + 1 then self.currentSprite = self.sprite + 2 return end
    if self.currentSprite == self.sprite + 2 then self.currentSprite = self.sprite return end
end

-------------------------------------------------------------------------
function HealParticle:Draw()
    DrawSprite(self.currentSprite, self.target.pos.x, self.target.pos.y + 8)
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function UpdateAllParticles(ds)
    for e in all(g_allParticles) do
        e:Update(ds)
    end
end

-------------------------------------------------------------------------
function DrawAllParticles()    
    for e in all(g_allParticles) do
        e:Draw()
    end  
end

 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
SpellCaster = 
{
    currentCastTime = 0,
}
g_castingSpell = nil

-------------------------------------------------------------------------
function SpellCaster:Create(spellDefName, target, caster)
    local newInstance = {}
    setmetatable(newInstance, { __index = SpellCaster})

    newInstance.target = target
    newInstance.caster = caster
    newInstance.spellDef = g_allSpellDefinitions[spellDefName]
    newInstance.hasCollision = false

    if newInstance.spellDef == nil then
        dPrint("Can't find spell with name:" .. spellDefName, 3)
    end

    return newInstance
end

-------------------------------------------------------------------------
function SpellCaster:Update(ds)
    if self.spellDef.castTime == 0 or self.currentCastTime >= self.spellDef.castTime then
        self:Cast()
    else
        self.currentCastTime = self.currentCastTime + ds
    end
end

-------------------------------------------------------------------------
function SpellCaster:Cast()
    castSpell = Spell:Create(self.spellDef, self.target)
    castSpell:OnCast()

    self.caster.mana = self.caster.mana - self.spellDef.manaCost
    g_theHud:SetCooldownForSelectedSpell(self.spellDef.cooldown)
    
    g_castingSpell = nil
end

-------------------------------------------------------------------------
function SpellCaster:GetPercentDone()
    return (self.currentCastTime / self.spellDef.castTime)
end

-------------------------------------------------------------------------
function SpellCaster:GetSpellName()
    return self.spellDef.name
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_theTank = {}

-------------------------------------------------------------------------
-- PartyMemberDef
-------------------------------------------------------------------------
PartyMemberDefinition =
{
    name = "idk",
    maxHealth = 100,
    health = 100,
}
g_allPartyMemberDefinitions = {}

-------------------------------------------------------------------------
function PartyMemberDefinition:Create(name)
    local newInstance = {}
    setmetatable(newInstance, {__index = PartyMemberDefinition})

    newInstance.name = name

    g_allPartyMemberDefinitions[name] = newInstance
    return newInstance
end

-------------------------------------------------------------------------
function PartyMemberDefinition:CreateAllDefinitions()

    newDef = PartyMemberDefinition:Create("tank")
    newDef.maxHealth = 500
    newDef.attackCooldown = 3
    newDef.attackDamage = 5
    newDef.updateFunctions =
    {
        normal = DefaultCombatUpdate
    }

    newDef = PartyMemberDefinition:Create("melee")
    newDef.maxHealth = 250
    newDef.attackCooldown = 1
    newDef.attackDamage = 5
    newDef.updateFunctions =
    {
        normal = DefaultCombatUpdate
    }

    newDef = PartyMemberDefinition:Create("ranged")
    newDef.maxHealth = 100
    newDef.attackCooldown = 2
    newDef.attackDamage = 7
    newDef.updateFunctions =
    {
        normal = DefaultCombatUpdate
    }
end

-------------------------------------------------------------------------
-- PartyMember
-------------------------------------------------------------------------
PartyMember = 
{
    cooldownTimer = RandomRange(0,1)
}
g_allPartyMembers = {} -- maybe allies and not player?

-------------------------------------------------------------------------
function PartyMember:Create(x,y, sprite, deathSprite, defAsString)
    local newinst = {}
    
    setmetatable( PartyMember, { __index = Entity } )
    setmetatable( newinst, { __index = PartyMember } )

    newinst.pos = Vector2:Create(x,y)
    newinst.originalSpot = Vector2:Create(x,y)
    newinst.sprite = sprite
    newinst.deathSprite = deathSprite
    newinst.definition = g_allPartyMemberDefinitions[defAsString] 
    newinst.maxHealth = newinst.definition.maxHealth
    newinst.health = newinst.definition.maxHealth

    newinst.attackDamage = newinst.definition.attackDamage

    newinst.randomDir = Vector2:Create(RandomRange(-1,1), RandomRange(-1,1))
    newinst.changeDirectionTimer = 0
    newinst.changeDirectionLength = 2
    
    newinst.target = g_boss

    Entity:Add(newinst, #g_allEntities + 1)
    return newinst
end

-------------------------------------------------------------------------
function PartyMember:CreateAllMembers()
    g_theTank = PartyMember:Create(64,128, 1, 24, "tank")
    PartyMember:Create(172,128, 2, 23, "melee")
    PartyMember:Create(126,200, 3,22, "ranged")
    PartyMember:Create(200,200, 4,21, "ranged")

end

-------------------------------------------------------------------------
function PartyMember:Update(ds)
    if self.isDead == true then return end

    --func = self.definition.updateFunctions[g_combatState]
    --if func ~= nil then func(self, ds) end

    if g_combatState == "normal" then
        DefaultCombatUpdate(self,ds)
    else
        MemberBossCastingSpellUpdate(self, ds)
    end

end

-------------------------------------------------------------------------
function PartyMember:Draw()
    if self.isDead == false then
        DrawSprite(self.sprite, self.pos.x, self.pos.y, self:CalculateRotationAmount())
        DrawSprite(self.sprite + 16, self.pos.x, self.pos.y + 16)
    else
        DrawSprite(self.deathSprite, self.pos.x, self.pos.y)
    end

    
    --DrawCircle(self.pos.x, self.pos.y, self.collisionRadius, "random")
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
function DefaultCombatUpdate(member, ds)
    member.target = g_boss
    
    if member.cooldownTimer > member.definition.attackCooldown then
        member:Attack()
        member.cooldownTimer = 0
    else
        member.cooldownTimer = member.cooldownTimer + ds
    end

    distance = Vector2:GetDistance(member.pos, member.originalSpot)
    if distance >= 1 then
        dir = member.originalSpot - member.pos 
        member.pos = member.pos + (dir * ds)

        member:AnimateMovement(true, ds)
    else
        member:AnimateMovement(false, ds)
    end

end

-------------------------------------------------------------------------
function MemberBossCastingSpellUpdate(member, ds)
    previousPos = member.pos
    
    if member.changeDirectionTimer >= member.changeDirectionLength then
        member.randomDir = Vector2:Create(RandomRange(-1,1), RandomRange(-1,1))
        member.changeDirectionTimer = 0
    else
        member.changeDirectionTimer = member.changeDirectionTimer + ds
    end

    member.pos = member.pos + (member.randomDir * 10 * ds)
        
    -- dont walk off map
    member.pos.x = Clamp(member.pos.x, 32, 220)
    member.pos.y = Clamp(member.pos.y, 32, 220)

    -- dont walk in boss
    if g_boss.collisionBounds:IsPointInside(member.pos) then 
        member.pos = previousPos 
        member.changeDirectionTimer = member.changeDirectionLength
    end

    member:AnimateMovement(true, ds)

end 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Attract = {}
g_Attract = Attract

-------------------------------------------------------------------------
function Attract:Enter() 
    ClearEntityTable()
    self.spawnTimer = 0
    self.spawnDelay = .5

end

-------------------------------------------------------------------------
function Attract:Update(ds)
    if(WasKeyJustReleased("enter")) then GoToState("playing") end

    if self.spawnTimer >= self.spawnDelay then 
        SlimeOrb:Create(RandomRange(16,240), 256, 0, -1)
        self.spawnTimer = 0
    else
        self.spawnTimer = self.spawnTimer + ds
    end

    UpdateAllEntities(ds)
end

-------------------------------------------------------------------------
function Attract:Draw()
    DrawAABB2Fill(0,0,256,256,"medBlue")
    DrawAABB2(0,0,256,256,"lightGreen")
    
    DrawAllEntities()
    
    --DrawAABB2Fill(8,190,250,210,"medBlue")
    DrawAABB2(8,190,250,210,"lightGreen")
    DrawText("Untitled Game Jam", 11,196,8,"medBlue")
    DrawText("Untitled Game Jam", 10,196,8)
    
    -- aka
    --DrawAABB2Fill(88, 124 - 14,108 + 62, 124 + 28,"medBlue")
    DrawAABB2(88, 124 - 14,108 + 62, 124 + 28,"lightGreen")
    DrawText("#3",108,128,12)
    DrawText("aka LD46", 88,114, 6)
    
    --DrawAABB2Fill(24, 28, 234, 42,"medBlue")
    DrawAABB2(24,28,234,42,"lightGreen")
    DrawText("Press Enter to Start", 28,32,6)
    --DrawText("Attract", 108,128,6, "green")
    
end


-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------
 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------
g_seenTutorial = false

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Tutorial = 
{
}
g_Tutorial = Tutorial

-------------------------------------------------------------------------
function Tutorial:Enter() 
    g_seenTutorial = true
end

-------------------------------------------------------------------------
function Tutorial:Update(ds)
    if WasKeyJustPressed("enter") then GoToState("playing") end
end

-------------------------------------------------------------------------
function Tutorial:Draw()
    DrawText("Tutorial", 108,128,6, "white")
end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
Playing = {}
g_Playing = Playing

-------------------------------------------------------------------------
function Playing:Enter()
    ClearEntityTable()
    self:StartUp()
end

-------------------------------------------------------------------------
function Playing:StartUp()
    Player:Create(64,64)
    PartyMember:CreateAllMembers()
    
    HUD:Create(g_allEntities, g_selectedSpells)
    
    Boss:Create(128,128)
end

-------------------------------------------------------------------------
function Playing:Update(ds)
    g_theHud:Update(ds)
    UpdateAllEntities(ds)
    UpdateAllParticles(ds)

    if WasKeyJustPressed("p") then
        dir = Vector2:Create(RandomRange(-1,1), RandomRange(-1,1))
        SlimeOrb:Create(128,128, dir:Normalize())
    end

    if WasKeyJustPressed("o") then
        HealParticle:Create(g_player)
    end


    if WasKeyJustPressed("k") or g_boss.health <= 0 then GoToState("gameWon") end
    if WasKeyJustPressed("l") or DidWeLoseTheGame() then GoToState("gameLost") end
end

-------------------------------------------------------------------------
function DidWeLoseTheGame()
    if g_player.isDead == true then return true end

    for i=2,5 do 
        if g_allEntities[i].isDead == false then return false end
    end

    return true
end

-------------------------------------------------------------------------
function Playing:Draw()
    DrawAABB2Fill(0,0,256,256,"grey")
    DrawAABB2(0,0,256,256,"grey")
    DrawMap();
    
    DrawAllEntities()
    DrawAllParticles()

    g_theHud:Render()
end
-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

 
-------------------------------------------------------------------------
-- Globals
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Class
-------------------------------------------------------------------------
GameWonScreen = {}
g_GameWonScreen = GameWonScreen

-------------------------------------------------------------------------
function GameWonScreen:Enter()
    for i=1, 5 do 
        xPos = 32 + (i * 32)
        g_allEntities[i].pos = Vector2:Create(xPos, 64)
        g_allEntities[i].rotationAmount = 1
    end

    PlayOneShot("victory.mp3")
end

-------------------------------------------------------------------------
function GameWonScreen:Update(ds)
    if(WasKeyJustReleased("enter")) then GoToState("attract") end
end

-------------------------------------------------------------------------
function GameWonScreen:Draw()
    DrawMap()
    --DrawAllEntities()

    for i=1, 5 do 
        g_allEntities[i]:Draw()
    end

    bounds = GetAABB2FromAABB2(Vector2:Create(.2,.4), Vector2:Create(.8,.6), AABB2:Create(0,0,256,256))
    Berry.DrawAABB2Fill(bounds, "lightGreen")
    Berry.DrawAABB2(bounds, "white")
    DrawText("Victory!", bounds.mins.x + 12, bounds.mins.y + 20, 10)


    bounds = GetAABB2FromAABB2(Vector2:Create(.1,.1), Vector2:Create(.9,.15), AABB2:Create(0,0,256,256))
    Berry.DrawAABB2Fill(bounds, "lightGreen")
    Berry.DrawAABB2(bounds, "white")
    DrawText("Hit Enter to Continue!", bounds.mins.x + 8, bounds.mins.y + 4, 5)

end

-------------------------------------------------------------------------
-- Standalone Functions
-------------------------------------------------------------------------

 
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
            self.radius = self.radius + (self.percentGrowth * ds)
        end

    else
        self.timer = self.timer + ds
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

    PlayMusic("bg.mp3")
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
    g_totalTime = g_totalTime + ds
	g_allStates[g_currentState]:Update(ds) 
end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
	Cls()
	g_allStates[g_currentState]:Draw(ds)
end


------------------------------------------------------------
function DoesDiscOverlapDisc(discA, discB)
    distance = Vector2:GetDistance(discA.center, discB.center)
    radiusCombined = discA.radius + discB.radius
    return distance < radiusCombined
end

-- Good luck <3



