local engine = require("Engine")
local gamestate = require("gameState")
local player = {}
local interaction = require("gameobjects.textInteraction")
local dialogSystem = require("dialogSystem")
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

local function updateInteractionRect(playerTable, collisionRect)
    local ewWH = { x = 26, y = 8 }
    local nsWH = { x = 8, y = 26 }
    local interactionRect = playerTable.interactionRect
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

local function playerInput(playerMan)
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
        direction = Directions.down
        moved = true
    end
    local animatorSpeed = 0
    if moved then
        local delta = gamestate.DeltaTimeSeconds
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
    local collisionRect = {x =  playerMan.x + playerCollisionOffsetAndSizeRect.x, y = playerMan.y + playerCollisionOffsetAndSizeRect.y, w =    playerCollisionOffsetAndSizeRect.w,h = playerCollisionOffsetAndSizeRect.h }
    -- collisionRect = engine.Collision.CheckRectForCollision(collisionRect)
    -- if collisionRect ~= nil then
    --     debugh.DrawRects[go] = collisionRect
    --     engine.Gameobject.SetPosition(go,
    --         engine.Tools.Round(collisionRect.x - playerCollisionOffsetAndSizeRect.x, 1),
    --         engine.Tools.Round(collisionRect.y - playerCollisionOffsetAndSizeRect.y, 1))
    -- end
    -- posX, posY = engine.Gameobject.Position(go)
    updateInteractionRect(playerMan, collisionRect)
    if interaction.CheckForTextInteractions(playerMan.interactionRect) then

    else
        dialogSystem.DisableDialogBox()

    end

    engine.DrawRect(playerMan.interactionRect, false, true)
end

function player.Start(playerData)
    if playerData.properties.loadLocation ~= gamestate.LoadLocation then
        return
    end
    local newPlayer = {
        playerGO = nil,
        playerAnimator = nil,
        x = playerData.x,
        y = playerData.y,
        playerSprite = nil,
        direction = playerData.properties.direction,
        interactionRect = { x = 0, y = 0, w = 0, h = 0 }
    }
    table.insert(player.players, newPlayer)
    newPlayer.playerGO = engine.Gameobject.CreateGameObject()
    engine.Gameobject.SetPosition(newPlayer.playerGO,newPlayer.x, newPlayer.y)
    newPlayer.playerSprite = engine.Sprite.NewSprite(player.player1Name, newPlayer.playerGO, { 0, 0, 32, 32 }, { 0, 0, 32, 32 })
    newPlayer.playerAnimator = engine.Animation.CreateAnimator(player.player1Name, newPlayer.playerSprite)
    engine.Camera.SetCameraFollowTarget(newPlayer.playerGO)

end

function player.Update()
    for _, playerMan in ipairs(player.players) do
        playerInput(playerMan)
    end
end

return player
