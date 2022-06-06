# BurgerTime

The controls are WASD and you can press L to throw salt (the animation is there but you can't kill enemies)

I use the component pattern defining behaviours, states.

I use the observer to connect the state machine with the player controller.

then the event queue for more global things like reset level, killed player,..

my game supports keyboard, I tried to implement controller but i still have to find some bugs there.

The AI is very simple could definitely be improved.

I mapped the levels with Inkscape, then I use raycasts to see if the object hits a floor, or wall, or ladder.

I hardcoded the burgers in the level but I could later on read the information from a file for each level.
