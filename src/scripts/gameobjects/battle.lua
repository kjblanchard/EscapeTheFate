local battleStert = require("battle.battleStart")
local engine = require("Engine")
local ui = require("UI")

local battle = {}
battle.battleUI = {}
battle.ticks = 0
battle.currentPercent = 0
function battle.BattleCreate(userdata, go)
    battle[go] = {}
end

function battle.BattleStart(go)
    battleStert.LoadAllBattlers()
    -- All battlers should now be on the field with proper stats in place, inside of battlers.
end

function battle.BattleUpdate(go)
    battle.ticks = battle.ticks + 1
    if battle.currentPercent < 100 and battle.ticks % 14 == 0 then
        battle.currentPercent = battle.currentPercent + 1
        --         atbBars.player1.progressBar = ui.lookup
        -- ["MainUIBox.SelectionsVLG.Player1Panel.Player1HLG.ATBBarAnimImage.ProgressBar"].data
        -- atbBars.player1.progressBarAnim
        ui.UpdateProgressBarPercent(battle.battleUI.ATBBars.player1.progressBar, battle.currentPercent)
        if battle.currentPercent == 100 then
            ui.PlayAnimation(battle.battleUI.ATBBars.player1.progressBarAnim, "turn")
        end
    end
end

function battle.BattleZoneDestroy(go)
    battle[go] = nil
end

function battle.RegisterDebugBoxFunctions()
    engine.Gameobject.RegisterGameObjectFunctions(GameObjectTypes.Battle, {
        battle.BattleCreate,
        battle.BattleStart,
        battle.BattleUpdate,
        battle.BattleZoneDestroy,
    })
end

return battle
