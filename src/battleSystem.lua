local battlerSystem = require("gameobjects.battler")
local gamestate = require("gameState")
local battleLocations = require("gameobjects.battlerLocation")
local battleSystem = {}
local initialized = false

local function loadAllBattlers()
    ---Players are id 1-3
    for i = 1, #gamestate.battle.players, 1 do
        local x, y = battleLocations.GetBattlerLocation(i)
        battlerSystem.New(x, y, nil)
    end
    ---Enemies are id 4-10
end

local function initialize()
    loadAllBattlers()
    initialized = true
end

function battleSystem.Start()
    initialized = false
end

function battleSystem.Update()
    if not initialized then initialize() end
    battlerSystem.Update()
end

function battleSystem.End()
    initialized = false
end

return battleSystem
