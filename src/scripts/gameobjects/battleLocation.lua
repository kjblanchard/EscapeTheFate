local engine = require("Engine")

local battleLocation = {}
battleLocation.locations = {}
function battleLocation.BattleLocationCreate(userdata, go)
    local location = {}
    for key, value in pairs(userdata.properties) do
        if key == "id" then
            location.battleId = value
            break
        end
    end
    local x, y = engine.Gameobject.Position(go)
    location.pos = { x = x, y = y }
    battleLocation.locations[location.battleId] = location
end

function battleLocation.BattleZoneDestroy(go)
    if battleLocation.locations and #battleLocation.locations > 0 then
        for key in pairs(battleLocation.locations) do
            battleLocation.locations[key] = nil
        end
    end
end

function battleLocation.RegisterDebugBoxFunctions()
    engine.Gameobject.RegisterGameObjectFunctions(GameObjectTypes.BattleLocation, {
        battleLocation.BattleLocationCreate,
        nil, nil,
        battleLocation.BattleZoneDestroy,
    })
end

return battleLocation
