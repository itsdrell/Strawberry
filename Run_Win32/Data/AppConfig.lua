startupGame='heartboom'
--release=true

resolution1="600,400"
resolution2="900,600"
resolution3="1200,800"

gifLength=8

showShell=false

--[[

Here is a list of ways you can config strawberry!

startupGame = "theNameOfProjectYouWantToLoad"
    - This takes a string of the name of the project you want to auto load and run
    - Good for quick iteration so you dont have to go through the console each time

release = bool 
    - If set to true, disables the editor and console and auto loads the startupGame
    - Good for when you want to give out your game as a windows exe

resolution1="600,400"
resolution2="900,600"
resolution3="width,height"
    - Allows you to change to three different types of resolutions
    - Resolution1 is what the app window will start as (the default)

gifLength=float
    - in seconds

hideMouse=bool
    - Wether we actually show the mouse cursor or not
    - defaults to true

showShell=bool
    - draw the image (Data/GameShell.png) instead of a black border around the game
--]]
