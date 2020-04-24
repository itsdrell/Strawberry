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
    self.pos += (self.direction * self.speed * ds)
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
        self.animationTimer += ds
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

