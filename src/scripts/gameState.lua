local gamestate = {}
local engine = require("Engine")
local directions = require("directions")

gamestate.transitioningScreens = false
gamestate.loadLocation = 0
gamestate.direction = Directions.down

return gamestate
