require("gameobjects.battler")
local engine = require("Engine")
local gameobject = require("GameObject")
local playerData = require("battle.data.players")
local battleZoneData = require("battle.data.battleZones")
local enemyGroupData = require("battle.data.enemyGroups")
local enemyData = require("battle.data.enemies")
local gamestate = require("gameState")
local battleLocations = require("gameobjects.battleLocation")
local battleStart = {}
local gameState = require("gameState")

local MaxNumPlayerBattlers = 3

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
    -- local battleZoneData = GetBattleZone(gamestate.battle.BattleId)
    local enemyGroupId = GetEnemyGroupRandom(gamestate.battle.BattleId)
    if not enemyGroupId then
        engine.Log.LogWarn("Enemy group id is nil, probably the bg is nil somehow.. setting enemy groupid to 1.")
        enemyGroupId = 1
    end
    for index, enemyId in ipairs(enemyGroupData.groups[enemyGroupId]) do
        if not enemyId then
            engine.Log.LogWarn("Enemy id is nil or 0, not loading")
            goto continue
        end
        local enemy = enemyData.enemies[enemyId]
        -- Add for the player offset.
        local loadLocation = battleLocations.locations[index + MaxNumPlayerBattlers]
        if loadLocation == nil then goto continue end
        local battlerGOPtr = gameobject.CreateGameObjectInCurrentMap()
        BattlerEnemyCreate(battlerGOPtr, enemy)
        gameobject.SetPosition(battlerGOPtr, loadLocation.pos.x, loadLocation.pos.y)
        ::continue::
    end
end

return battleStart
