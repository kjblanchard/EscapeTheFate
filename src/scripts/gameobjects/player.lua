local engine = require("Engine")
local gameobject = require("GameObject")
local debugh = require("debugh")
local dialogSystem = require("dialog")
local gameState = require("gameState")
require("directions")
require("gameobjects.gameobjectTypes")
local exitBox = require("gameobjects.exitBox")
local dialog = require("gameobjects.dialogBox")
---@class PlayerGameObject
---@field sprite lightuserdata     -- Name to be displayed in various UI elements
---@field animator lightuserdata     -- Name to be displayed in various UI elements
---@field direction integer     -- Name to be displayed in various UI elements
---@field isMoving boolean     -- Name to be displayed in various UI elements
---@field interactionRect table     -- Name to be displayed in various UI elements
---@field textInteracting boolean     -- Name to be displayed in various UI elements
---@type  table{PlayerGameObject}
local player = {}
player.players = {}
player.moveSpeed = 100


local playerCollisionOffsetAndSizeRect <const> = { x = 8, y = 8, w = 16, h = 22 }

local function setPlayerDirection(playerData)
    local animation = "walkD"
    if playerData.direction == Directions.up then
        animation = "walkU"
    elseif playerData.direction == Directions.right then
        animation = "walkR"
    elseif playerData.direction == Directions.left then
        animation = "walkL"
    end
    engine.PlayAnimation(playerData.animator, animation)
end


local function updateInteractionRect(playerTable, collisionRect)
    local ewWH = { x = 26, y = 8 }
    local nsWH = { x = 8, y = 26 }
    local interactionRect = playerTable["interactionRect"]
    if playerTable.direction == Directions.right then
        interactionRect.x = collisionRect.x + collisionRect.w
        interactionRect.y = collisionRect.y + (collisionRect.h / 2) - (ewWH.y / 2)
        interactionRect.w = ewWH.x
        interactionRect.h = ewWH.y
    elseif playerTable.direction == Directions.left then
        interactionRect.x = collisionRect.x - ewWH.x
        interactionRect.y = collisionRect.y + (collisionRect.h / 2) - (ewWH.y / 2)
        interactionRect.w = ewWH.x
        interactionRect.h = ewWH.y
    elseif playerTable.direction == Directions.up then
        interactionRect.x = collisionRect.x + (collisionRect.w / 2) - (nsWH.x / 2)
        interactionRect.y = collisionRect.y - nsWH.y
        interactionRect.w = nsWH.x
        interactionRect.h = nsWH.y
    elseif playerTable.direction == Directions.down then
        interactionRect.x = collisionRect.x + (collisionRect.w / 2) - (nsWH.x / 2)
        interactionRect.y = collisionRect.y + collisionRect.h
        interactionRect.w = nsWH.x
        interactionRect.h = nsWH.y
    end
end

---Returns true if player can move
---@param playerData PlayerGameObject
---@return boolean
local function canPlayerMove(playerData)
    return not playerData.textInteracting
end

---@param playerData PlayerGameObject
---@return table velocity x and y that will be used.
local function handlePlayerMovement(playerData)
    local velocity = { x = 0, y = 0 }
    if not canPlayerMove(playerData) then return velocity end
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
    return velocity
end


function player.GetPlayerCollisionBox(ptr)
    local x, y = gameobject.Position(ptr)
    local collisionRect = {
        x = x + playerCollisionOffsetAndSizeRect.x,
        y = y + playerCollisionOffsetAndSizeRect.y,
        w = playerCollisionOffsetAndSizeRect.w,
        h = playerCollisionOffsetAndSizeRect.h
    }
    return collisionRect
end

--#region Public
function PlayerObjectCreate(_, go, direction)
    gameobject.SetType(go, GameObjectTypes.Player)
    player.players[go] = {}
    local playerObj = player.players[go]
    playerObj.sprite = engine.Sprite.NewSprite("player1", go, { 0, 0, 32, 32 }, { 0, 0, 32, 32 })
    playerObj.animator = engine.CreateAnimator("player1", player.players[go]["sprite"])
    playerObj.direction = direction
    playerObj.isMoving = false
    playerObj.interactionRect = { x = 0, y = 0, w = 0, h = 0 }
    playerObj.textInteracting = false
end

local function PlayerStart(go)
    setPlayerDirection(player.players[go])
    -- TODO set the follow target to the only player, should handle multiple players better.
    engine.SetCameraFollowTarget(go)
    gameState.transitioningScreens = false
end

local function PlayerUpdate(go)
    if gameState.transitioningScreens then goto fin end
    local playerData = player.players[go]
    if playerData == nil then
        engine.Log.LogWarn("Trying to update player with no userdata")
        return
    end
    local velocity = handlePlayerMovement(playerData)
    local isMoving = velocity.x ~= 0 or velocity.y ~= 0
    playerData.isMoving = isMoving
    if not isMoving then
        engine.SetAnimatorSpeed(playerData["animator"], 0.0)
        -- engine.Log.LogWarn("Animator speed is 0")
    end
    if isMoving then
        -- Normalize diagonal speed
        local magnitude = math.sqrt(velocity.x * velocity.x + velocity.y * velocity.y)
        velocity.x = velocity.x / magnitude
        velocity.y = velocity.y / magnitude
        local direction = playerData["direction"]
        -- Determine animation based on direction priority: Y axis first
        if velocity.y > 0 then
            if direction ~= Directions.down then
                engine.PlayAnimation(playerData["animator"], "walkD")
                playerData["direction"] = Directions.down
            end
        elseif velocity.y < 0 then
            if direction ~= Directions.up then
                engine.PlayAnimation(playerData["animator"], "walkU")
                playerData["direction"] = Directions.up
            end
        elseif velocity.x > 0 then
            if direction ~= Directions.right then
                engine.PlayAnimation(playerData["animator"], "walkR")
                playerData["direction"] = Directions.right
            end
        elseif velocity.x < 0 then
            if direction ~= Directions.left then
                engine.PlayAnimation(playerData["animator"], "walkL")
                playerData["direction"] = Directions.left
            end
        end
        engine.SetAnimatorSpeed(playerData["animator"], 1.0)
        -- engine.Log.LogWarn("Animator speed is 1")
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
                engine.Tools.Round(collisionRect.x - playerCollisionOffsetAndSizeRect.x, 1),
                engine.Tools.Round(collisionRect.y - playerCollisionOffsetAndSizeRect.y, 1))
        end
        posX, posY = gameobject.Position(go)
        updateInteractionRect(playerData, collisionRect)
        debugh.DrawRects[tostring(go) .. "interaction"] = playerData["interactionRect"]

        -- Handle overlap with an exit
        for _, value in pairs(exitBox.boxes) do
            if engine.CheckForCollision(value["rect"], collisionRect) then
                gameState.loadLocation = value.loadLocation
                gameState.transitioningScreens = true
                engine.LoadScene(value.loadMap)
                goto fin
            end
        end
    end
    -- Handle A button pressed.
    if engine.Input.KeyboardKeyJustPressed(engine.Buttons.A) then
        -- If we are already interacting, then we should progress the text if needed, or end
        if playerData["textInteracting"] then
            if dialogSystem.DialogInteractionUpdate(1) then
                playerData["textInteracting"] = false
            end
        else
            -- Check for overlap with dialog boxes and start interacting.
            for _, value in pairs(dialog.boxes) do
                if engine.CheckForCollision(value["rect"], playerData["interactionRect"]) then
                    dialogSystem.DialogInteractionStart(1, tostring(value["dialog"]))
                    playerData["textInteracting"] = true
                end
            end
        end
    end
    ::fin::
end

local function PlayerDestroy(go)
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
    engine.RegisterGameObjectFunctions(GameObjectTypes.Player, {
        nil,
        PlayerStart,
        PlayerUpdate,
        PlayerDestroy,
    })
end

return player
