
mouse_x = 0;
mouse_y = 0;

camera_x = 0;
camera_y = 0;

-------------------------------------------------------------
-- Called once at the start
function StartUp()

end


------------------------------------------------------------
-- called every frame
function Update(ds)
    CheckPressed();
    ScrollCheck();
    MousePosCheck();
    MoveCamera()
end

function MoveCamera()
    if(IsKeyPressed("w")) then
        camera_y += (10)
    end

    if(IsKeyPressed("a")) then
        camera_x -= (10)
    end

    if(IsKeyPressed("d")) then
        camera_x += (10)
    end

    if(IsKeyPressed("s")) then
        camera_y -= (10)
    end

    Camera(camera_x, camera_y)
end

function MousePosCheck()
    mouse_x, mouse_y = GetMousePosition()
    dPrint("x pos: " .. Floor(mouse_x) .. " ypos: " .. Floor(mouse_y))

    mousePos = Berry.GetMousePosition()
    dPrint(mousePos:ToString())
end

function CheckPressed()
    if WasMouseButtonJustPressed("left") then
        dPrint("Left Mouse Button pressed", 3)
    end

    if WasMouseButtonJustPressed("right") then
        dPrint("right Mouse Button pressed", 3)
    end

    if WasMouseButtonJustReleased("left") then
        dPrint("Left Mouse Button released", 3)
    end

    if WasMouseButtonJustReleased("right") then
        dPrint("right Mouse Button released", 3)
    end

    if IsMouseButtonPressed("l") then
        dPrint("Left is down")
    end

    if IsMouseButtonPressed("r") then
        dPrint("right is down")
    end
end

function ScrollCheck()

    if DidMouseWheelScrollDown() then
        dPrint("Mouse whell scrolled down", 3)
    end

    if DidMouseWheelScrollUp() then
        dPrint("Mouse wheel scrolled up",3)
    end

end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
    Cls()

    DrawAABB2Fill(0,0,256,256, "red");
    DrawAABB2Fill(256,0, 256 * 2, 256, "green"); -- right
    DrawAABB2Fill(0,256,256,256 * 2, "blue"); -- top l
    DrawAABB2Fill(256,256,256 * 2,256 * 2, "white");
    
    DrawCircle(mouse_x, mouse_y, 6, "random")
end


-- Good luck <3


