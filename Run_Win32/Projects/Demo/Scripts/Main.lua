
player = 
{
    x = 30;
    y = 0;
}

camera = 
{
    x = 0;
    y = 0;
}

totalTime = 0;

-- modes as "enums"
RenderTests = 
{
   NORMAL_RENDER_TEST = 0;
   PALETTE_RENDER_TEST = 1;
   SPRITE_RENDER_TEST = 2;
   NUM_OF_RENDER_TESTS = 4;
}
eCurrentRenderTest = 0;


bgMusic = "Audio/Music/test.mp3";
errorSound = "Audio/SFX/error.wav";

-------------------------------------------------------------------
function StartUp()
    Print("Start up hit");
    PlayMusic(bgMusic);
    TestMath();
end

-------------------------------------------------------------------
function Update(ds)
    TestAudio();
    TestCamera();
    TestInput(ds);
    
    ChangeRenderingTests();

    totalTime = totalTime + ds;
end

-------------------------------------------------------------------
function Draw()
    Cls()

    if eCurrentRenderTest == RenderTests.NORMAL_RENDER_TEST then TestRendering();
    elseif eCurrentRenderTest == RenderTests.PALETTE_RENDER_TEST then ShowPalette();
    elseif eCurrentRenderTest == RenderTests.SPRITE_RENDER_TEST then TestSprites();
    end
    
end

-------------------------------------------------------------------
function TestAudio()
    if WasKeyJustPressed("space") then
        PlayOneShot(errorSound);
    end

    if WasKeyJustPressed("backspace") then
        StopMusic(bgMusic);
    end
end

-------------------------------------------------------------------
function ChangeRenderingTests()
   
    if WasKeyJustPressed("t") then
        eCurrentRenderTest = eCurrentRenderTest + 1;

        if(eCurrentRenderTest == RenderTests.NUM_OF_RENDER_TESTS - 1) then
            eCurrentRenderTest = 0;
        end
    end

end

-------------------------------------------------------------------
-- Test Input
function TestInput(ds)

    local speed = 20.0;
    
    if IsKeyPressed("a") then
        player.x = player.x - (speed * ds);
    end

    if IsKeyPressed("d") then
        player.x = player.x + (speed * ds);
    end

    if IsKeyPressed("w") then
        player.y = player.y + (speed * ds);
    end

    if IsKeyPressed("s") then
        player.y = player.y - (speed * ds);
    end

end

-------------------------------------------------------------------
function TestCamera()

    if WasKeyJustPressed("c") then
        camera.x = camera.x + 1;
        camera.y = camera.y + 1;
        Camera(camera.x, camera.y);
    end

    rate = Fract(totalTime * .25) * 360;
    camera.x = 0 + CosDegrees(rate) * 2;
    camera.y = 0 + SinDegrees(rate) * 2;
    --Print(rate);
    Camera(camera.x, camera.y);

end

-------------------------------------------------------------------
-- Test all draw functions
function TestRendering()

    Cls("red");
    SetColor("white");
    DrawText("Draw Shapes Test", camera.x + -62, camera.y + 58, 4, white);
    
    DrawLine(0,-10, 0, 10, "white");
    DrawLine(-10, 0, 10, 0, "random");

    
    DrawCircle(player.x, player.y, 5, "random"); -- default colors
    DrawCircleFill(-30, -30, 5);
    
    DrawAABB2Fill(-20, 10, -10, 20, 0,255,255,255);
    DrawAABB2(10, 10, 20, 20, 0, 255, 255, 255);
    
end

-------------------------------------------------------------------
function TestSprites()

    DrawText("Sprite Test", camera.x + -62, camera.y + 58, 4);
    
    local pivot = 30;
    DrawSprite(0, pivot, pivot, 0,1,1, true, false);
    DrawSprite(0, -pivot, pivot,0,1,1);
    DrawSprite(0, -pivot, -pivot,0,1,1, false, true);
    DrawSprite(0, pivot, -pivot, 0,1,1,true, true);

    -- rotation
    rotation = Fract(totalTime * .2) * 360;
    DrawSprite(3, 0,0, rotation, 4, 4);

    pivot = 50;
    -- numbers
    DrawSprite(1, -pivot, pivot, 0, 1,1);
    DrawSprite(1, pivot, pivot, 0, 2,1);
    DrawSprite(1, -pivot, -pivot, 0, 1,2);
    DrawSprite(1, pivot, -pivot, 0, 2,2);
end


-------------------------------------------------------------------
-- Renders the palette in a box
function ShowPalette()
    
    Cls();
    DrawAABB2Fill(-60, -60, 60, 60, "black");
    DrawAABB2Fill(-62, -62, 62, 62, "white");

    -- top row, left to right
    DrawAABB2Fill(-60, 36, -36, 60, "black");
    DrawAABB2Fill(-36, 36, -12, 60, "navy");
    DrawAABB2Fill(-12, 36, 12, 60, "medBlue");
    DrawAABB2Fill(12, 36, 36, 60, "lightBlue");
    DrawAABB2Fill(36, 36, 60, 60, "white");
  
    DrawAABB2Fill(-60, 12, -36, 36, "darkBrown");
    DrawAABB2Fill(-36, 12, -12, 36, "brown");
    DrawAABB2Fill(-12, 12, 12,  36, "lightBrown");
    DrawAABB2Fill(12,  12, 36,  36, "tan");
    DrawAABB2Fill(36,  12, 60,  36, "grey");

    DrawAABB2Fill(-60, -12, -36, 12, "darkGreen");
    DrawAABB2Fill(-36, -12, -12, 12, "green");
    DrawAABB2Fill(-12, -12, 12,  12, "lightGreen");
    DrawAABB2Fill(12,  -12, 36,  12, "yellow");
    DrawAABB2Fill(36,  -12, 60,  12, "darkGrey");

    DrawAABB2Fill(-60, -36, -36, -12, "darkRed");
    DrawAABB2Fill(-36, -36, -12, -12, "orangeRed");
    DrawAABB2Fill(-12, -36, 12,  -12, "orange");
    DrawAABB2Fill(12,  -36, 36,  -12, "mustard");
    DrawAABB2Fill(36,  -36, 60,  -12, "red");

    DrawAABB2Fill(-60, -60, -36, -36, "cyan");
    DrawAABB2Fill(-36, -60, -12, -36, "blue");
    DrawAABB2Fill(-12, -60, 12,  -36, "indigo");
    DrawAABB2Fill(12,  -60, 36,  -36, "purple");
    DrawAABB2Fill(36,  -60, 60,  -36, "magenta");

end

-------------------------------------------------------------------
-- Test Math
function TestMath()

    Print("\n-----------------------------------------------\n")
    Print("Running math tests");

    -- Abs
    absTest = Abs(-5);
    Print("\nabs Test: " .. absTest);

    -- ATan2 
    atan2 = ATan2(.5,.5);
    Print("\nATan2 Test: " .. atan2);

    -- Cos 
    cosTest = CosDegrees(360);
    Print("\ncos Test: " .. cosTest);

    -- Sin
    sinTest = SinDegrees(90);
    Print("\nSin Test: " .. sinTest);

    -- Floor
    flrTest = Floor(-5.5);
    Print("\nFloor Test: " .. flrTest);

    -- Ceil
    ceilTest = Ceil(-5.5);
    Print("\n Ceil Test: " .. ceilTest);

    -- Min
    minTest = Min(-1, 10);
    Print("\n Min Test: " .. minTest);

    -- Max
    maxTest = Max(-2, 10);
    Print("\n Max Test: " .. maxTest);

    -- Sqrt
    squareRootTest = Sqrt(9);
    Print("\n Square Root Test: " .. squareRootTest);

    -- Clamp 
    clampTest = Clamp(100, 0, 10);
    Print("\n Clamp Test: " .. clampTest);

    -- RandomRange
    for i = 0, 10 do
        randomRange = RandomRange(0, 100);
        Print("\n Random Range Test: " .. randomRange);
    end

    -- Dot
    dotTest = Dot(0,0,1,1);
    Print("\n Dot Test: " .. dotTest);

    -- Lerp
    lerpTest = Lerp(0,1,.5);
    Print("\n Lerp Test: " .. lerpTest);

    -- Chance
    for i = 0, 10 do
        chanceTest = Chance(50);
        Print("\n Chance Test: " .. chanceTest);
    end

    Print("\n Done with Math");
    Print("\n-----------------------------------------------\n")
end