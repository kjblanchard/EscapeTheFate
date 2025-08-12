local battleStert = require("battle.battleStart")
local engine = require("Engine")

local battle = {}
function battle.BattleCreate(userdata, go)
    battle[go] = {}
end

function battle.BattleStart(go)
    engine.Log.LogWarn("Starting battle manager?")
    battleStert.LoadAllBattlers()
end

function battle.BattleUpdate(go)
end

function battle.BattleZoneDestroy(go)
    battle[go] = nil
end

function battle.RegisterDebugBoxFunctions()
    engine.RegisterGameObjectFunctions(GameObjectTypes.Battle, {
        battle.BattleCreate,
        battle.BattleStart,
        battle.BattleUpdate,
        battle.BattleZoneDestroy,
    })
end

return battle
