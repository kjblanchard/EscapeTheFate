local engine = require("Engine")
local gameobject = require("GameObject")
local dialog = require("gameobjects.dialogBox")
local debugh = require("debugh")
local tools = require("Tools")
local dialogSystem = require("dialog")
local directions = {
    down = 0,
    right = 1,
    up = 2,
    left = 3
}
local playerCollisionOffsetAndSizeRect = { x = 8, y = 8, w = 16, h = 22 }

local player = {}
player.players = {}
player.moveSpeed = 100
function PlayerObjectCreate(userdata, go)
    player.players[go] = {}
    engine.Log.LogWarn("Player spawning")
    player.players[go]["sprite"] = engine.NewSprite("player1", go, { 0, 0, 32, 32 }, { 0, 0, 32, 32 })
    player.players[go]["animator"] = engine.CreateAnimator("player1", player.players[go]["sprite"])
    player.players[go]["direction"] = directions.down
    player.players[go]["interactionRect"] = { x = 0, y = 0, w = 0, h = 0 }
    player.players[go]["textInteracting"] = false
end

function PlayerStart(go)
    engine.Log.LogWarn("Starting player")
    engine.PlayAnimation(player.players[go]["animator"], "walkD")
    engine.SetCameraFollowTarget(go)
end

local function updateInteractionRect(playerTable, collisionRect)
    local ewWH = { x = 26, y = 8 }
    local nsWH = { x = 8, y = 26 }
    local interactionRect = playerTable["interactionRect"]

    if playerTable.direction == directions.right then
        interactionRect.x = collisionRect.x + collisionRect.w
        interactionRect.y = collisionRect.y + (collisionRect.h / 2) - (ewWH.y / 2)
        interactionRect.w = ewWH.x
        interactionRect.h = ewWH.y
    elseif playerTable.direction == directions.left then
        interactionRect.x = collisionRect.x - ewWH.x
        interactionRect.y = collisionRect.y + (collisionRect.h / 2) - (ewWH.y / 2)
        interactionRect.w = ewWH.x
        interactionRect.h = ewWH.y
    elseif playerTable.direction == directions.up then
        interactionRect.x = collisionRect.x + (collisionRect.w / 2) - (nsWH.x / 2)
        interactionRect.y = collisionRect.y - nsWH.y
        interactionRect.w = nsWH.x
        interactionRect.h = nsWH.y
    elseif playerTable.direction == directions.down then
        interactionRect.x = collisionRect.x + (collisionRect.w / 2) - (nsWH.x / 2)
        interactionRect.y = collisionRect.y + collisionRect.h
        interactionRect.w = nsWH.x
        interactionRect.h = nsWH.y
    end
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
    end
    if isMoving then
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
        -- gameobject.SetPosition(go, posX + velocity.x * player.moveSpeed * delta, posY + velocity.y * player.moveSpeed * delta)
        local newX = posX + velocity.x * player.moveSpeed * delta
        local newY = posY + velocity.y * player.moveSpeed * delta
        local collisionRect = { newX + playerCollisionOffsetAndSizeRect.x, newY + playerCollisionOffsetAndSizeRect.y,
            playerCollisionOffsetAndSizeRect.w, playerCollisionOffsetAndSizeRect.h }
        collisionRect = engine.CheckRectForCollision(collisionRect)
        if collisionRect ~= nil then
            debugh.DrawRects[go] = collisionRect
            gameobject.SetPosition(go,
                tools.round(collisionRect.x - playerCollisionOffsetAndSizeRect.x, 1),
                tools.round(collisionRect.y - playerCollisionOffsetAndSizeRect.y, 1))
        end
        posX, posY = gameobject.Position(go)
        updateInteractionRect(playerData, collisionRect)
        debugh.DrawRects[tostring(go) .. "interaction"] = playerData["interactionRect"]
    end
    if engine.Input.KeyboardKeyJustPressed(engine.Buttons.A) then
        -- If we are already interacting, then we should progress the text if needed
        if playerData["textInteracting"] then
        else
        end
        -- Check for overlap with dialog boxes
        for key, value in pairs(dialog.boxes) do
            if engine.CheckForCollision(value["rect"], playerData["interactionRect"]) then
                dialogSystem.DialogInteraction(1, tostring(value["dialog"]))
                engine.Log.LogError("Interaction and " .. tostring(value["dialog"]))
            end
        end
    end
end

function PlayerDestroy(go)
    engine.Log.LogInfo("Destroying player")
    if player.players[go] == nil then
        engine.Log.LogWarn("Trying to destroy a player that doesnt exist in lua")
        return
    end
    -- engine.DestroySprite(player.players[go]["sprite"])
    engine.DestroyAnimator(player.players[go]["animator"])
    player.players[go]["sprite"] = nil
    player.players[go]["animator"] = nil
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
