local gamestate = require("gameState")
local battlerLocation = {}

function battlerLocation.Start(data)
    local newlocation = {
        x = data.x,
        y = data.y,
        id = data.id
    }
    gamestate.battle.battlerLocations = gamestate.battle.battlerLocations or {}
    table.insert(gamestate.battle.battlerLocations, newlocation)
end


function battlerLocation.End()
    gamestate.battle.battlerLocations = nil
end

return battlerLocation
