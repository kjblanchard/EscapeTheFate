require("directions")
local gamestate = {}

gamestate.transitioningScreens = false
gamestate.loadLocation = 0
gamestate.direction = Directions.down
gamestate.battle = {
    InBattle = false,
    BattleId = 0,
    CurrentStepTime = 0
}

return gamestate
