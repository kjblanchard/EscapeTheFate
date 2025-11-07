local engine = require("Engine")
local gamestate = require("gameState")
local player = {}
player.moveSpeed = 100
player.players = {}
player.player1Name = "player1"


local Directions = {
    down = 0,
    right = 1,
    up = 2,
    left = 3
}

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
        direction = playerData.properties.direction
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
