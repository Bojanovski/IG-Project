A car game where the controls are the BCI inputs form the Leap Motion device. Mouse and keyboard can also be used.

Video link:

https://www.youtube.com/watch?v=DXP-05L71n0

--------------------------------------------------------------------

Made by:
-Bojan Lovrovic (Team leader, Physics)
-Branimir Klaric (Sound)
-Bruno Pregun (2D graphics, Modeling, Igor Kramaric, Model loading)
-Jure Ratkovic (3D graphics)
-Tomislav Tunkovic (Third party software manager)

--------------------------------------------------------------------

Key bindings:
    - C :switch between car and free camera
    - W/A/S/D/Q/E + mouse: move the free camera
    - 9/0: speed up/down the free camera
    
    - I/J/K/L: control car
    - 1/2/3: switch gears

--------------------------------------------------------------------

You can 'draw' the track in ../Resources/TrackDescription.txt. First two numbers represent height and width of the track.
Then there is height * width grid of characters representing the track. Meaning of each character:
    - '.' this grid cell is empty
    - 'R' there is a road segment on this grid cell
    - 'C' there is a road segment on this grid cell, and it is the cars starting point

--------------------------------------------------------------------

You can edit the graphics settings in ../Resources/GraphicsSettings.txt
