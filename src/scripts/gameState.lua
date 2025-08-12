require("directions")
---@class playerData
---@field Id integer     -- The id of the player to load from the players
---@field Level   integer -- The level of the player
local gamestate = {}

gamestate.transitioningScreens = false
gamestate.loadLocation = 0
gamestate.direction = Directions.down
gamestate.battle = {
    InBattle = false,
    BattleId = 0,
    CurrentStepTime = 0,
    ---@type playerData[] | nil
    players = {
        nil,
        { Id = 1, Level = 1 },
        nil
    }
}

return gamestate
