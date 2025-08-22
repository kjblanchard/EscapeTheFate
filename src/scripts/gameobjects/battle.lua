local battleStert = require("battle.battleStart")
local engine = require("Engine")
local ui = require("UI")
local battlers = require("gameobjects.battler")

local battle = {}
battle.battleUI = {}
battle.ticks = 0
battle.currentPercent = 0
--
battle.playingVictory = false
battle.exitingBattle = false
--
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
        ui.UpdateProgressBarPercent(battle.battleUI.ATBBars.player1.progressBar, battle.currentPercent)
        if battle.currentPercent == 100 then
            ui.PlayAnimation(battle.battleUI.ATBBars.player1.progressBarAnim, "turn")
        end
    end
    if not battle.playingVictory and engine.Input.KeyboardKeyJustPressed(engine.Input.Buttons.B) then
        for goPtr, battlerTable in pairs(battlers.battlers) do
            if not battlerTable.isPlayer then
                goto continue
            end
            engine.Animation.PlayAnimation(battlerTable.animator, "cheer", 4)
            engine.Animation.AddAnimationToQueue(battlerTable.animator, "clap", -1)
            ::continue::
        end
        engine.Audio.PlayBGM('victory', 1.0)
        battle.playingVictory = true
    end
    if battle.playingVictory and engine.Input.KeyboardKeyJustPressed(engine.Input.Buttons.A) then
        engine.Scene.LoadScene('debugTown')
        battle.playingVictory = false
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
