local battleStert = require("battle.battleStart")
local engine = require("Engine")
local ui = require("UI")
local battlers = require("gameobjects.battler")
local gameState = require("gameState")

local battle = {}
-- TODO maybe we use a state machine for this.. but for now keep it simple.
battle.targetSelection = false
battle.playingVictory = false
battle.exitingBattle = false
--
function battle.BattleCreate(userdata, go)
    battle[go] = {}
end

function battle.BattleStart(go)
    battleStert.LoadAllBattlers()
    -- All battlers should now be on the field with proper stats in place, inside of battlers.
    ui.SetObjectVisible(ui.lookup["BattleUI.P1CommandsBox"].data, false)
    ui.SetObjectVisible(ui.lookup["BattleUI.Selections Finger"].data, false)
    ui.SetObjectActive(ui.lookup["BattleUI.P1CommandsBox.AttackButton"].data, false)
end

local function victoryUI()
    ui.SetObjectVisible(ui.lookup["BattleUI.MainUIBox"].data, false)
    ui.SetObjectVisible(ui.lookup["BattleUI.P1CommandsBox"].data, false)
    ui.SetObjectVisible(ui.lookup["BattleUI.Selections Finger"].data, false)
end

function battle.BattleUpdate(go)
    if not battle.playingVictory and engine.Input.KeyboardKeyJustPressed(engine.Input.Buttons.B) then
        for goPtr, battlerTable in pairs(gameState.battle.currentBattlers) do
            if not battlerTable.isPlayer then
                goto continue
            end
            engine.Animation.PlayAnimation(battlerTable.animator, "cheer", 8)
            engine.Animation.AddAnimationToQueue(battlerTable.animator, "clap", -1)
            ::continue::
        end
        victoryUI()
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

function battle.PlayerAttackButtonPressed()
end

function battle.PlayerSkillsButtonPressed()
end

function battle.PlayerMagicButtonPressed()

end

function battle.PlayerItemsButtonPressed()
end

function battle.EnemyTargetButtonPressed()
end

return battle
