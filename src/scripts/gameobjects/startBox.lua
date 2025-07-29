local engine = require("Engine")
local gameobject = require("GameObject")

local startBox = {}
startBox.boxes = {}
function startBox.DebugBoxCreate(userdata, go)
    startBox.boxes[go] = {}
    local loadLocation = 0
    local direction = 0
    for key, value in pairs(userdata.properties) do
        if key == "loadLocation" then
            loadLocation = value
        elseif key == 'direction' then
            direction = value
        end
    end
    startBox.boxes[go].loadLocation = loadLocation
    startBox.boxes[go].direction = direction
    local x, y = gameobject.Position(go)
    local w, h = gameobject.Size(go)
    startBox.boxes[go]["rect"] = { x = x, y = y, w = w, h = h }
end

function startBox.DebugBoxDestroy(go)
    startBox.boxes[go] = nil
end

function startBox.RegisterDebugBoxFunctions()
    engine.RegisterGameObjectFunctions(4, {
        startBox.DebugBoxCreate,
        nil, nil,
        startBox.DebugBoxDestroy,
    })
end

return startBox
