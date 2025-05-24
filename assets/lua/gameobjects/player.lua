local engine = require("Engine")
local gameobject = require("GameObject")
local player = {}
player.players = {}
player.moveSpeed = 100
function PlayerObjectCreate(userdata, go)
    engine.NewSprite("player1", go, { 0, 0, 32, 32 }, { 0, 0, 32, 32 })
    engine.Log.LogWarn("Just created" .. tostring(go) .. " with userdata " .. tostring(userdata))
end

function PlayerStart(go)
    engine.Log.LogWarn("Starting player")
    player.players[go] = {}
end

-- function PlayerUpdate(go)
--     local playerData = player.players[go]
--     if playerData == nil then
--         engine.Log.LogWarn("Trying to update player with no userdata")
--         goto continue
--     end
--     local movedThisFrame = false
--     local velocity = { x = 0, y = 0 }
--     if engine.Input.KeyboardKeyDown(engine.Buttons.UP) then
--         engine.Log.LogWarn("Button Down")
--         velocity.y = -player.moveSpeed
--     elseif engine.Input.KeyboardKeyDown(engine.Buttons.DOWN) then
--         velocity.y = player.moveSpeed
--     end
--     if engine.Input.KeyboardKeyDown(engine.Buttons.RIGHT) then
--         velocity.x = player.moveSpeed
--     elseif engine.Input.KeyboardKeyDown(engine.Buttons.LEFT) then
--         velocity.x = -player.moveSpeed
--     end
--     if velocity.x ~= 0 or velocity.y ~= 0 then movedThisFrame = true end
--     if movedThisFrame == false then goto continue end
--     -- velocity.x = velocity.x * engine.DeltaTimeInSeconds()
--     -- velocity.y = velocity.y * engine.DeltaTimeInSeconds()
--     local length = math.sqrt(velocity.x * velocity.x + velocity.y * velocity.y)
--     if length > 0 then
--         velocity.x = velocity.x / length * player.moveSpeed * engine.DeltaTimeInSeconds()
--         velocity.y = velocity.y / length * player.moveSpeed * engine.DeltaTimeInSeconds()
--     end
--     local newPosX, newPosY = gameobject.Position(go)
--     newPosX = newPosX + velocity.x
--     newPosY = newPosY + velocity.y
--     gameobject.SetPosition(go, newPosX, newPosY)
--     ::continue::
-- end
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
    gameobject.SetPosition(go, newPosX + velocity.x, newPosY + velocity.y)
end

-- function PlayerUpdate(go)
--     local playerData = player.players[go]
--     if playerData == nil then
--         engine.Log.LogWarn("Trying to update player with no userdata")
--         goto continue
--     end

--     local velocity = { x = 0, y = 0 }
--     if engine.Input.KeyboardKeyDown(engine.Buttons.UP) then
--         velocity.y = -1
--     elseif engine.Input.KeyboardKeyDown(engine.Buttons.DOWN) then
--         velocity.y = 1
--     end
--     if engine.Input.KeyboardKeyDown(engine.Buttons.RIGHT) then
--         velocity.x = 1
--     elseif engine.Input.KeyboardKeyDown(engine.Buttons.LEFT) then
--         velocity.x = -1
--     end

--     local length = math.sqrt(velocity.x * velocity.x + velocity.y * velocity.y)
--     if length == 0 then goto continue end

--     velocity.x = velocity.x / length * player.moveSpeed * engine.DeltaTimeInSeconds()
--     velocity.y = velocity.y / length * player.moveSpeed * engine.DeltaTimeInSeconds()

--     local newPosX, newPosY = gameobject.Position(go)
--     newPosX = newPosX + velocity.x
--     newPosY = newPosY + velocity.y
--     gameobject.SetPosition(go, newPosX, newPosY)

--     ::continue::
-- end

function player.RegisterPlayerFunctions()
    engine.RegisterGameObjectFunctions(1, {
        PlayerObjectCreate, -- index 1 = create
        PlayerStart,
        PlayerUpdate,       -- index 2 = start
    })
end

return player
