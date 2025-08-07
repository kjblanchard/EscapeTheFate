local engine = require("Engine")
local gameState = require("gameState")
local gameobject = require("GameObject")
local player = require("gameobjects.player")

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

    if loadLocation == gameState.loadLocation then
        -- Create player.
        local playerGOPtr = gameobject.CreateGameObjectInCurrentMap()
        PlayerObjectCreate(nil, playerGOPtr, direction)
        engine.Log.LogError("Loaded the thing")
        local rect = startBox.boxes[go]["rect"]
        gameobject.SetPosition(playerGOPtr, rect.x, rect.y)
    end
end

function startBox.DebugBoxDestroy(go)
    startBox.boxes[go] = nil
end

function startBox.RegisterDebugBoxFunctions()
    engine.RegisterGameObjectFunctions(GameObjectTypes.MapStart, {
        startBox.DebugBoxCreate,
        nil, nil,
        startBox.DebugBoxDestroy,
    })
end

return startBox
