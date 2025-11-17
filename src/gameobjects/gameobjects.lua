local gameobjects = {}
local player = require("gameobjects.player")
local textInteraction = require("gameobjects.textInteraction")
local playerExit = require("gameobjects.playerExit")

gameobjects["4Start"] = player.Start
gameobjects["5TextInteract"] = textInteraction.Start
gameobjects["2Exit"] = playerExit.Start

function gameobjects.UpdateAllGameObjects()
    player.Update()
end

function gameobjects.EndAllGameObjects(force)
    player.End(force)
    textInteraction.End(force)
    playerExit.End(force)
end

return gameobjects
