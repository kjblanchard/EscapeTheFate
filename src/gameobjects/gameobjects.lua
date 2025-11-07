local gameobjects = { }
local player = require("gameobjects.player")

gameobjects["4Start"] = player.Start

function gameobjects.UpdateAllGameObjects()
    player.Update()
end

return gameobjects
