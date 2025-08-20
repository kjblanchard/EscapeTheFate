local engine = require("Engine")

local debugBox = {}
debugBox.boxes = {}
function debugBox.DebugBoxCreate(userdata, go)
    debugBox.boxes[go] = {}
    local name = ""
    for key, value in pairs(userdata.properties) do
        if key == "filename" then
            name = value
            break
        end
    end
    if name == "" then goto fin end
    local fileName = engine.MapName() .. "." .. name
    debugBox.boxes[go].dialog = require(fileName)
    local x, y = engine.Gameobject.Position(go)
    local w, h = engine.Gameobject.Size(go)
    debugBox.boxes[go]["rect"] = { x = x, y = y, w = w, h = h }
    ::fin::
end

function debugBox.DebugBoxDestroy(go)
    debugBox.boxes[go] = nil
end

function debugBox.RegisterDebugBoxFunctions()
    engine.Gameobject.RegisterGameObjectFunctions(GameObjectTypes.TextInteract, {
        debugBox.DebugBoxCreate,
        nil, nil,
        debugBox.DebugBoxDestroy,
    })
end

return debugBox
