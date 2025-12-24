local engine = require("Engine")
local gamestate = require("gameState")
local player = {}
local dialogSystem = require("dialogSystem")
local playerExit = require("gameobjects.playerExit")

player.moveSpeed = 100
player.players = {}
player.player1Name = "player1"

local Directions = {
    down = 0,
    right = 1,
    up = 2,
    left = 3
}

local playerCollisionOffsetAndSizeRect <const> = { x = 8, y = 8, w = 16, h = 22 }

local function updateInteractionRect(playerTable)
    local ewWH = { x = 26, y = 8 }
    local nsWH = { x = 8, y = 26 }
    local interactionRect = playerTable.interactionRect

    if playerTable.direction == Directions.right then
        interactionRect.x = playerTable.collisionRect.x + playerTable.collisionRect.w
        interactionRect.y = playerTable.collisionRect.y + (playerTable.collisionRect.h / 2) - (ewWH.y / 2)
        interactionRect.w = ewWH.x
        interactionRect.h = ewWH.y
    elseif playerTable.direction == Directions.left then
        interactionRect.x = playerTable.collisionRect.x - ewWH.x
        interactionRect.y = playerTable.collisionRect.y + (playerTable.collisionRect.h / 2) - (ewWH.y / 2)
        interactionRect.w = ewWH.x
        interactionRect.h = ewWH.y
    elseif playerTable.direction == Directions.up then
        interactionRect.x = playerTable.collisionRect.x + (playerTable.collisionRect.w / 2) - (nsWH.x / 2)
        interactionRect.y = playerTable.collisionRect.y - nsWH.y
        interactionRect.w = nsWH.x
        interactionRect.h = nsWH.y
    elseif playerTable.direction == Directions.down then
        interactionRect.x = playerTable.collisionRect.x + (playerTable.collisionRect.w / 2) - (nsWH.x / 2)
        interactionRect.y = playerTable.collisionRect.y + playerTable.collisionRect.h
        interactionRect.w = nsWH.x
        interactionRect.h = nsWH.y
    end
end

local function setPlayerDirection(playerData)
    local animation = "walkD"
    if playerData.direction == Directions.up then
        animation = "walkU"
    elseif playerData.direction == Directions.right then
        animation = "walkR"
    elseif playerData.direction == Directions.left then
        animation = "walkL"
    end
    engine.Animation.PlayAnimation(playerData.playerAnimator, animation)
end

local function playerHandleMovement(playerMan)
    if playerMan.interacting then return false end
    local moved = false
    local velocityX = 0
    local velocityY = 0
    local direction = playerMan.direction

    if engine.Input.KeyboardKeyDown(engine.Input.Buttons.UP) then
        velocityY = -1
        moved = true
        direction = Directions.up
    end
    if engine.Input.KeyboardKeyDown(engine.Input.Buttons.RIGHT) then
        velocityX = 1
        moved = true
        direction = Directions.right
    end
    if engine.Input.KeyboardKeyDown(engine.Input.Buttons.LEFT) then
        velocityX = -1
        moved = true
        direction = Directions.left
    end
    if engine.Input.KeyboardKeyDown(engine.Input.Buttons.DOWN) then
        velocityY = 1
        moved = true
        direction = Directions.down
    end

    local animatorSpeed = 0
    if moved then
        local delta = gamestate.DeltaTimeSeconds or 0
        playerMan.x = playerMan.x + velocityX * player.moveSpeed * delta
        playerMan.y = playerMan.y + velocityY * player.moveSpeed * delta
        engine.Gameobject.SetPosition(playerMan.playerGO, playerMan.x, playerMan.y)

        if direction ~= playerMan.direction then
            playerMan.direction = direction
            setPlayerDirection(playerMan)
        end

        animatorSpeed = 1.0
    end

    engine.Animation.SetAnimatorSpeed(playerMan.playerAnimator, animatorSpeed)
    return moved
end

local function setPlayerInteractionIndicatorLocation(playerMan)
    if gamestate.interactionImageTable then
        gamestate.interactionImageTable.visible = true
        gamestate.interactionImageTable.rect.x = playerMan.x + 20
        gamestate.interactionImageTable.rect.y = playerMan.y - 5
    end
end

local function handleInteractions(playerMan)
    if playerMan.interacting then
        if engine.Input.KeyboardKeyJustPressed(engine.Input.Buttons.A) and dialogSystem.ProgressDialog() then
            playerMan.interacting = false
        end
        return
    end

    updateInteractionRect(playerMan)

    if dialogSystem.CheckForTextInteractions(playerMan.interactionRect) then
        setPlayerInteractionIndicatorLocation(playerMan)
        if engine.Input.KeyboardKeyJustPressed(engine.Input.Buttons.A) then
            playerMan.interacting = true
            engine.Animation.SetAnimatorSpeed(playerMan.playerAnimator, 0.0)
            dialogSystem.Start()
            if gamestate.interactionImageTable then
                gamestate.interactionImageTable.visible = false
            end
        end
    else
        if gamestate.interactionImageTable then
            gamestate.interactionImageTable.visible = false
        end
    end
end

local function handleCollisions(playerMan)
    local collisionRect = playerMan.collisionRect
    -- update collisionRect's position from player's coords + offset
    collisionRect.x = playerMan.x + playerCollisionOffsetAndSizeRect.x
    collisionRect.y = playerMan.y + playerCollisionOffsetAndSizeRect.y

    local x, y = engine.Map.ResolveCollisionWithSolids(collisionRect)

    -- ResolveCollisionWithSolids should return resolved x,y; if nil, nothing to do
    if x == nil or y == nil then
        return
    end

    -- If engine returned corrected coords, move player to them
    if collisionRect.x ~= x or collisionRect.y ~= y then
        playerMan.x = x - playerCollisionOffsetAndSizeRect.x
        playerMan.y = y - playerCollisionOffsetAndSizeRect.y
        collisionRect.x = x
        collisionRect.y = y

        engine.Gameobject.SetPosition(playerMan.playerGO,
            engine.Tools.Round(playerMan.x, 1),
            engine.Tools.Round(playerMan.y, 1))
    end
end

local function handleExit(playerMan)
    if playerExit.CheckForExits(playerMan.collisionRect) then
        return true
    end
    return false
end

local function playerInput(playerMan)
    if playerHandleMovement(playerMan) then
        handleCollisions(playerMan)
    end

    if handleExit(playerMan) then return end

    handleInteractions(playerMan)
end

function player.Start(playerData)
    if playerData.properties.loadLocation ~= gamestate.nextLocation then
        return
    end

    local newPlayer = {
        playerGO = nil,
        playerAnimator = nil,
        x = playerData.x,
        y = playerData.y,
        playerSprite = nil,
        direction = playerData.properties.direction or Directions.down,
        interacting = false,
        interactionRect = { x = 0, y = 0, w = 0, h = 0 },
        collisionRect = {
            x = playerData.x + playerCollisionOffsetAndSizeRect.x,
            y = playerData.y + playerCollisionOffsetAndSizeRect.y,
            w = playerCollisionOffsetAndSizeRect.w,
            h = playerCollisionOffsetAndSizeRect.h
        }
    }

    table.insert(player.players, newPlayer)
    newPlayer.playerGO = engine.Gameobject.CreateGameObject()
    engine.Gameobject.SetPosition(newPlayer.playerGO, newPlayer.x, newPlayer.y)
    newPlayer.playerSprite = engine.Sprite.NewSprite(player.player1Name, newPlayer.playerGO, { 0, 0, 32, 32 },
        { 0, 0, 32, 32 })
    newPlayer.playerAnimator = engine.Animation.CreateAnimator(player.player1Name, newPlayer.playerSprite)
    setPlayerDirection(newPlayer)
    engine.Camera.SetCameraFollowTarget(newPlayer.playerGO)
end

local function playerDestroy(playerMan)
    -- destroy animator first to avoid animator accessing the GO during its shutdown
    if playerMan.playerAnimator then
        engine.Animation.DestroyAnimator(playerMan.playerAnimator)
        playerMan.playerAnimator = nil
    end

    if playerMan.playerGO then
        engine.Gameobject.Destroy(playerMan.playerGO)
        playerMan.playerGO = nil
    end

    -- clear other references to help Lua/GC
    playerMan.playerSprite = nil
end

function player.Update()
    -- iterate backwards so removing entries doesn't shift upcoming indices
    for i = #player.players, 1, -1 do
        local playerMan = player.players[i]

        if playerMan == nil then
            -- defensive: remove any accidental nils
            table.remove(player.players, i)
        elseif playerMan.Destroy == true then
            playerDestroy(playerMan)
            table.remove(player.players, i)
        else
            playerInput(playerMan)
        end
    end
end

function player.End(force)
    -- mark all players for destruction; Update() does actual destroy/removal
    for _, playerMan in ipairs(player.players) do
        if force then
            playerDestroy(playerMan)
        else
            playerMan.Destroy = true
        end
    end
end

return player
