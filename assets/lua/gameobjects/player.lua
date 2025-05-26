local engine = require("Engine")
local gameobject = require("GameObject")
local player = {}
player.players = {}
player.moveSpeed = 100
function PlayerObjectCreate(userdata, go)
    player.players[go] = {}
    player.players[go]["sprite"] = engine.NewSprite("player1", go, { 0, 0, 32, 32 }, { 0, 0, 32, 32 })
    engine.Log.LogWarn("Just created" .. tostring(go) .. " with userdata " .. tostring(userdata))
end

function PlayerStart(go)
    engine.Log.LogWarn("Starting player")
end

function PlayerUpdate(go)
    local playerData = player.players[go]
    if playerData == nil then
        engine.Log.LogWarn("Trying to update player with no userdata")
        return
    end

    local velocity = { x = 0, y = 0 }
    if engine.Input.KeyboardKeyDown(engine.Buttons.UP) then
        velocity.y = velocity.y - 1
    end
    if engine.Input.KeyboardKeyDown(engine.Buttons.DOWN) then
        velocity.y = velocity.y + 1
    end
    if engine.Input.KeyboardKeyDown(engine.Buttons.RIGHT) then
        velocity.x = velocity.x + 1
    end
    if engine.Input.KeyboardKeyDown(engine.Buttons.LEFT) then
        velocity.x = velocity.x - 1
    end
    local newPosX, newPosY = gameobject.Position(go)
    velocity.x = velocity.x * player.moveSpeed * engine.DeltaTimeInSeconds()
    velocity.y = velocity.y * player.moveSpeed * engine.DeltaTimeInSeconds()
    gameobject.SetPosition(go, newPosX + velocity.x, newPosY + velocity.y)
end

function PlayerDestroy(go)
    engine.Log.LogWarn("Destroying player")
    engine.DestroySprite(player.players[go]["sprite"])
    player.players[go]["sprite"] = nil
    player.players[go] = nil
end

function player.RegisterPlayerFunctions()
    engine.RegisterGameObjectFunctions(1, {
        PlayerObjectCreate, -- index 1 = create
        PlayerStart,
        PlayerUpdate,       -- index 2 = start
        PlayerDestroy,
    })
end

return player
