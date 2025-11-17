---@class playerData
---@field Id integer     -- The id of the player to load from the players
---@field Level   integer -- The level of the player
-- -@type table<lightuserdata,battler>

local gamestate = {}
gamestate.DeltaTimeSeconds = 0
gamestate.DeltaTimeMS = 0
---@type table
---Stores info about the interaction table, from the UI
gamestate.interactionImageTable = nil
---type string | nil
gamestate.nextMap = nil
---type int
gamestate.nextLocation = 0
gamestate.loadingMap = false
return gamestate
