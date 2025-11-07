local gameobjects = { }
local player = require("gameobjects.player")
local textInteraction = require("gameobjects.textInteraction")

gameobjects["4Start"] = player.Start
gameobjects["5TextInteract"] = textInteraction.Start

function gameobjects.UpdateAllGameObjects()
    player.Update()
end

return gameobjects
