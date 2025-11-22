local battle = {}
local battleSystem = require("battleSystem")
local battleGO = nil

function battle.Start()
    battleGO = 0
    battleSystem.Start()

end

function battle.Update()
    if battleGO == nil then return end
    battleSystem.Update()
end

function battle.End(force)
    battleGO = nil
    battleSystem.End(force)
end

return battle
