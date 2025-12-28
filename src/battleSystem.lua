local battler = require("gameobjects.battler")
local gamestate = require("gameState")
local battleData = require("data.battleData")
local engine = require("Engine")
local battleSystem = {}
local initialized = false

local function loadBattlerById(id, locationId)
    local x, y = battleSystem.GetBattlerLocation(locationId)
    if x == nil or y == nil then return end
    engine.Log.LogWarn("Looking up id " .. id .. " for location " .. locationId)

    local data = battleData[id]
    local debugData = string.format("The battler data is: Name: %s, id %d", data.stats.Name, id)
    engine.Log.LogWarn(debugData)
    battler.New(x, y, data.stats, data.offsetRect, data.sprite, data.idle)
end

local function loadAllBattlers()
    ---Players are id 1-3
    for locationId = 1, #gamestate.battle.players, 1 do
        local battleId = gamestate.battle.players[locationId]
        if battleId == 0 then goto continue end
        loadBattlerById(battleId, locationId)
        ::continue::
    end
    ---Enemies are id 4-10
    for locationId = 1, #gamestate.battle.enemies, 1 do
        --Lets start at the enemy locations
        local battleId = gamestate.battle.enemies[locationId]
        if battleId == 0 then goto continue end
        loadBattlerById(battleId, locationId + 3)
        ::continue::
    end
end

local function initialize()
    initialized = true
    loadAllBattlers()
end


function battleSystem.Start()
    initialized = false
end

function battleSystem.Update()
    if not initialized then initialize() end
    battler.Update()
end

function battleSystem.End()
    initialized = false
end

--- Gets the battler location x and y, or nil if not found
---@return number | nil x
---@return number | nil y
---@param id integer
function battleSystem.GetBattlerLocation(id)
    local battlerLocations = gamestate.battle.battlerLocations
    for i = 1, #battlerLocations, 1 do
        local currentLocation = battlerLocations[i]
        if id == currentLocation.id then
            return currentLocation.x, currentLocation.y
        end
    end
    return nil
end
function battleSystem.InitializeUIVariables()
    engine.Log.LogWarn("Initializing the variables")
end

return battleSystem
