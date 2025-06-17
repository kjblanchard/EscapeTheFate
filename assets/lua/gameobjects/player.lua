local engine = require("Engine")
local gameobject = require("GameObject")
local directions = {
    down = 0,
    right = 1,
    up = 2,
    left = 3
}
local player = {}
player.players = {}
player.moveSpeed = 100
function PlayerObjectCreate(userdata, go)
    player.players[go] = {}
    player.players[go]["sprite"] = engine.NewSprite("player1", go, { 0, 0, 32, 32 }, { 0, 0, 32, 32 })
    player.players[go]["animator"] = engine.CreateAnimator("player1", player.players[go]["sprite"])
    player.players[go]["direction"] = directions.down
end

function PlayerStart(go)
    engine.Log.LogWarn("Starting player")
    engine.PlayAnimation(player.players[go]["animator"], "walkD")
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
    local isMoving = velocity.x ~= 0 or velocity.y ~= 0
    if not isMoving then
        engine.SetAnimatorSpeed(playerData["animator"], 0.0)
        return
    end
    -- Normalize diagonal speed
    local magnitude = math.sqrt(velocity.x * velocity.x + velocity.y * velocity.y)
    velocity.x = velocity.x / magnitude
    velocity.y = velocity.y / magnitude
    local direction = playerData["direction"]
    -- Determine animation based on direction priority: Y axis first
    if velocity.y > 0 then
        if direction ~= directions.down then
            engine.PlayAnimation(playerData["animator"], "walkD")
            playerData["direction"] = directions.down
        end
    elseif velocity.y < 0 then
        if direction ~= directions.up then
            engine.PlayAnimation(playerData["animator"], "walkU")
            playerData["direction"] = directions.up
        end
    elseif velocity.x > 0 then
        if direction ~= directions.right then
            engine.PlayAnimation(playerData["animator"], "walkR")
            playerData["direction"] = directions.right
        end
    elseif velocity.x < 0 then
        if direction ~= directions.left then
            engine.PlayAnimation(playerData["animator"], "walkL")
            playerData["direction"] = directions.left
        end
    end
    engine.SetAnimatorSpeed(playerData["animator"], 1.0)
    local posX, posY = gameobject.Position(go)
    local delta = engine.DeltaTimeInSeconds()
    gameobject.SetPosition(go, posX + velocity.x * player.moveSpeed * delta, posY + velocity.y * player.moveSpeed * delta)
end

function PlayerDestroy(go)
    engine.Log.LogWarn("Destroying player")
    if player.players[go] == nil then
        engine.Log.LogWarn("Trying to destroy a player that doesnt exist in lua")
        return
    end
    -- engine.DestroySprite(player.players[go]["sprite"])
    engine.DestroyAnimator(player.players[go]["animator"])
    player.players[go]["sprite"] = nil
    player.players[go]["animator"] = nil
    player.players[go] = nil
    if 0 == 1 then

    end
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
