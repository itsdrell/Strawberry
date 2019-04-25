

-------------------------------------------------------------
-- Called once at the start
function StartUp()

end


------------------------------------------------------------
-- called every frame
function Update(ds)


end

------------------------------------------------------------
-- called after update, does all drawing
function Draw()
    Cls();
    DrawAABB2Fill(-62, -62, 62, 62, "red");
    DrawAABB2Fill(-62, -62, 62, 62, "yellow");

end


-- Good luck <3


