require("battle.data.battleZones")
local engine = require("Engine")
local gameobject = require("GameObject")
local player = require("gameobjects.player")
local gamestate = require("gameState")

local battleZone = {}
battleZone.zones = {}
function battleZone.BattleZoneCreate(userdata, go)
    battleZone.zones[go] = {}
    local zone = battleZone.zones[go]
    for key, value in pairs(userdata.properties) do
        if key == "battleId" then
            zone.battleId = value
            break
        end
    end
    local x, y = gameobject.Position(go)
    local w, h = gameobject.Size(go)
    zone.rect = { x = x, y = y, w = w, h = h }
    -- ::fin::
end

function battleZone.BattleZoneUpdate(go)
    if gamestate.battle.InBattle or gamestate.transitioningScreens then goto continue end
    local zone = battleZone.zones[go]
    for key, value in pairs(player.players) do
        if not value.isMoving then goto continue end
        local playerRect = player.GetPlayerCollisionBox(key)
        if engine.CheckForCollision(playerRect, zone.rect) then
            gamestate.battle.CurrentStepTime = gamestate.battle.CurrentStepTime + engine.DeltaTimeInSeconds()
            -- TODO should check if this exists.
            local battleZoneData = GetBattleZone(zone.battleId)
            if battleZoneData and gamestate.battle.CurrentStepTime >= battleZoneData.stepCount then
                engine.Log.LogError("Should start battle!")
                gamestate.battle.InBattle = true
                gamestate.transitioningScreens = true
                gamestate.battle.CurrentStepTime = 0.0
                engine.LoadScene("forest")
            end
        end
    end
    ::continue::
end

function battleZone.BattleZoneDestroy(go)
    battleZone.zones[go] = nil
end

function battleZone.RegisterDebugBoxFunctions()
    engine.RegisterGameObjectFunctions(1, {
        battleZone.BattleZoneCreate,
        nil, battleZone.BattleZoneUpdate,
        battleZone.BattleZoneDestroy,
    })
end

return battleZone
