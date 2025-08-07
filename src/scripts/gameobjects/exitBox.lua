local engine = require("Engine")
local gameobject = require("GameObject")
local defaultMap = "debugTown"

local exitBox = {}
exitBox.boxes = {}
function exitBox.DebugBoxCreate(userdata, go)
    exitBox.boxes[go] = {}
    local loadLocation = 0
    local loadMap = defaultMap
    for key, value in pairs(userdata.properties) do
        if key == "loadLocation" then
            loadLocation = value
        elseif key == 'loadMap' then
            loadMap = value
        end
    end
    exitBox.boxes[go].loadLocation = loadLocation
    exitBox.boxes[go].loadMap = loadMap
    local x, y = gameobject.Position(go)
    local w, h = gameobject.Size(go)
    exitBox.boxes[go]["rect"] = { x = x, y = y, w = w, h = h }
end

function exitBox.DebugBoxDestroy(go)
    exitBox.boxes[go] = nil
end

function exitBox.RegisterDebugBoxFunctions()
    engine.RegisterGameObjectFunctions(GameObjectTypes.MapExit, {
        exitBox.DebugBoxCreate,
        nil, nil,
        exitBox.DebugBoxDestroy,
    })
end

return exitBox
