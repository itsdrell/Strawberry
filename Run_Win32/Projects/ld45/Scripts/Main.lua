

-------------------------------------------------------------
-- globals
-------------------------------------------------------------
g_allGhosts = {}
g_totalTime = 0;
deltaTime = 0;

-------------------------------------------------------------
-- Called once at the start
function StartUp()
    MakePlayer();
    CreateAllGhosts();
end

-------------------------------------------------------------
function MakePlayer()
    player = {}
    player.x = 32;
    player.y = 32;
    player.sprite = 4;
    player.speed = 100;
    player.sprintSpeed = player.speed * 2.5;

end

------------------------------------------------------------
function MakeGhost()
    local ghost = {}
    ghost.x = 64;
    ghost.y = 64;
    ghost.sprite = 19;
    ghost.speed = 100;

    ghost.rotation = 330;
    ghost.rotationSpeed = 40;
    ghost.maxRotationAngle = 390;
    ghost.minRotationAngle = 330;
    ghost.rotationLeft = true;

    table.insert(g_allGhosts, ghost);
end

------------------------------------------------------------
function CreateAllGhosts()
    --for i=0,10 do
    --    MakeGhost()
    --end
    MakeGhost()
end


------------------------------------------------------------
-- called every frame
function Update(ds)
    deltaTime = ds;

    MovePlayer(ds);
    MoveCamera()
    
    for i,ghost in ipairs(g_allGhosts) do
        UpdateGhost(ghost);
    end

    g_totalTime = g_totalTime + ds;
end

------------------------------------------------------------
function MovePlayer(ds)
    
    speed = player.speed;
    if IsKeyPressed("lshift") then
        speed = player.sprintSpeed;
    end
    
    if IsKeyPressed( "a" ) then
        player.x = player.x - ds * speed;
    end

    if IsKeyPressed( "d" ) then
        player.x = player.x + ds * speed;
    end

    if IsKeyPressed( "w" ) then
        player.y = player.y + ds * speed;
    end

    if IsKeyPressed( "s" ) then
        player.y = player.y - ds * speed;
    end
    
end

function UpdateGhost(ghost)
    --ghost.x
    RotateGhost(ghost)
end

------------------------------------------------------------
function RotateGhost(ghost)
    
    step = ghost.rotationSpeed * deltaTime;
    if ghost.rotationLeft == false then
        step = step * -1;
    end
    
    ghost.rotation = ghost.rotation + step;

    if(ghost.rotation >= ghost.maxRotationAngle) then
        ghost.rotationLeft = false;
    end

    if(ghost.rotation <= ghost.minRotationAngle) then
        ghost.rotationLeft = true;
    end
end

------------------------------------------------------------
function MoveCamera()
    cameraX = player.x - 256 * .5;
    camerY = player.y - 256 * .5;
    Camera(cameraX, camerY);
    
end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
    Cls()

    DrawMap();
    DrawPlayer();

    for i,ghost in ipairs(g_allGhosts) do
        DrawGhost(ghost);
    end

end

------------------------------------------------------------
function DrawGhost(ghost)
    DrawSprite(ghost.sprite, ghost.x, ghost.y, ghost.rotation,1,1);
end

------------------------------------------------------------
function DrawPlayer()
    DrawAABB2(player.x - 8, player.y - 8, player.x + 8, player.y + 8, 4);
    DrawSprite(player.sprite, player.x, player.y, 0,1,1);
end


-- Good luck <3


