local engine = require("Engine")
local gameobject = require("GameObject")

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
    local x, y = gameobject.Position(go)
    local w, h = gameobject.Size(go)
    debugBox.boxes[go]["rect"] = { x = x, y = y, w = w, h = h }
    -- for index, value in ipairs(debugBox.boxes[go].dialog) do
    --     engine.Log.LogWarn("Index is " .. index .. " and value is " .. value)
    -- end
    ::fin::
end

function debugBox.DebugBoxDestroy(go)
    debugBox.boxes[go] = nil
end

function debugBox.RegisterDebugBoxFunctions()
    engine.RegisterGameObjectFunctions(5, {
        debugBox.DebugBoxCreate,
        nil, nil,
        debugBox.DebugBoxDestroy,
    })
end

return debugBox
