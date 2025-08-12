require("gameobjects.battler")
local engine = require("Engine")
local gameobject = require("GameObject")
local playerData = require("battle.data.players")
local gamestate = require("gameState")
local battleLocations = require("gameobjects.battleLocation")
local battleStart = {}

function battleStart.LoadAllBattlers()
    -- engine.Log.LogWarn("Starting all battlers!")

    for index, player in pairs(gamestate.battle.players) do
        if player == nil then goto continue end
        -- local level = player.Level
        local id = player.Id
        if id < 1 or id > #playerData then goto continue end
        local currentPlayerData = playerData[id]
        -- Load the player onto the field
        local loadLocation = battleLocations.locations[index]
        if loadLocation == nil then goto continue end
        local battlerGOPtr = gameobject.CreateGameObjectInCurrentMap()
        BattlerObjectCreate(battlerGOPtr, currentPlayerData)
        gameobject.SetPosition(battlerGOPtr, loadLocation.pos.x, loadLocation.pos.y)
        ::continue::
    end
end

return battleStart
