require("directions")
---@class playerData
---@field Id integer     -- The id of the player to load from the players
---@field Level   integer -- The level of the player

---@class battler
---@field sprite lightuserdata     -- The id of the player to load from the players
---@field animator   integer -- The level of the player
---@field stats   Stats -- The level of the player
---@field button   lightuserdata -- The level of the player
---@field isPlayer   boolean -- The level of the player
---@field currentATB   number -- The level of the player
---@field maxATB   integer -- The level of the player
local gamestate = {}

gamestate.DeltaTimeSeconds = 0
gamestate.DeltaTimeMS = 0
gamestate.sceneChange = false
gamestate.nextScene = nil
gamestate.transitioningScreens = false
gamestate.loadLocation = 0
gamestate.direction = Directions.down
gamestate.battle = {
    InBattle = false,
    BattleId = 0,
    CurrentStepTime = 0,
    battleUI = {},
    ---@type playerData[] | nil
    players = {
        nil,
        { Id = 1, Level = 1 },
        nil
    },
    ---@type table<lightuserdata,battler>
    currentBattlers = {},
    ---@type battler
    currentTarget = nil
}

return gamestate
