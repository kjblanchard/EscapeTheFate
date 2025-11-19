local engine = require("Engine")
local gamestate = require("gameState")
local playerExit = {}

local playerExits = {}

function playerExit.Start(data)
    local newExit = {
        rect = {
            x = data.x,
            y = data.y,
            w = data.width,
            h = data.height,
        },
    }
    newExit.nextMap = data.properties.loadMap
    newExit.nextLocation = data.properties.loadLocation
    table.insert(playerExits, newExit)
end

---@param rect table
---Checks to see if you have overlapped with an exit, if so, sets the gamestate to the next map
function playerExit.CheckForExits(rect)
    for i = 1, #playerExits, 1 do
        local currentExit = playerExits[i]
        if engine.Collision.CheckForCollision(rect, currentExit.rect) then
            gamestate.nextLocation = currentExit.nextLocation
            gamestate.nextMap = currentExit.nextMap
            engine.Audio.PlaySfxOneShot("transition2", 0.5)
            return true
        end
    end
    return false
end

function playerExit.End(force)
    playerExits = {}
end

return playerExit
