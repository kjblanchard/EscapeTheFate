local gameobjects = {}
local player = require("gameobjects.player")
local textInteraction = require("gameobjects.textInteraction")
local playerExit = require("gameobjects.playerExit")
local battle = require("gameobjects.battle")
local battleLocation = require("gameobjects.battlerLocation")

gameobjects["4Start"] = player.Start
gameobjects["5TextInteract"] = textInteraction.Start
gameobjects["2Exit"] = playerExit.Start
gameobjects["6BattleLocation"] = battleLocation.Start
gameobjects["7Battle"] = battle.Start

function gameobjects.UpdateAllGameObjects()
    player.Update()
    battle.Update()
end

function gameobjects.EndAllGameObjects(force)
    player.End(force)
    textInteraction.End(force)
    playerExit.End(force)
    battleLocation.End()
    battle.End(force)
end

return gameobjects
