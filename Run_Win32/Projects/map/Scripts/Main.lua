xpos = 0;
ypos = 0;

-------------------------------------------------------------
-- Called once at the start
function StartUp()

end


------------------------------------------------------------
-- called every frame
function Update(ds)
    
    if IsKeyPressed( "a" ) then
        xpos = xpos - ds * 200;
    end

    if IsKeyPressed( "d" ) then
        xpos = xpos + ds * 200;
    end

    if IsKeyPressed( "w" ) then
        ypos = ypos + ds * 200;
    end

    if IsKeyPressed( "s" ) then
        ypos = ypos - ds * 200;
    end
    Camera(xpos, ypos);

    if WasKeyJustPressed("t") then
        SetTileSprite(0,0,68);
    end

    --Print(GetTileSprite(0,0));

    if DoesTileHaveCollision(0,0,1) then
    end
end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
    Cls()

    DrawMap();
    
    DrawAABB2Fill(-20, 10, -10, 20, 0,255,255,255);
    DrawAABB2(10, 10, 20, 20, 0, 255, 255, 255);

end


-- Good luck <3


