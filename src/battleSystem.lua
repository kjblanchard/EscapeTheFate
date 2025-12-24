local battler = require("gameobjects.battler")
local gamestate = require("gameState")
local stats = require("data.hero")
local battleSystem = {}
local initialized = false
local engine = require("Engine")

local function loadAllBattlers()
    ---Players are id 1-3
    -- for i = 1, #gamestate.battle.players, 1 do
    for i = 1, 1, 1 do
        local x, y = battleSystem.GetBattlerLocation(i)
        battler.New(x, y, stats)
    end
    ---Enemies are id 4-10
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

return battleSystem
