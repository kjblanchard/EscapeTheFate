local engine = require("Engine")
local ui = require("UI")
local gamestate = require("gameState")
local battler = {}
local enemyFingerName = "BattleUI.EnemySelectFinger"
local battlerButtonsName = "BattleUI.EnemyBattlerButtons"
local infoBoxGfx = "BattleUI.MainInfoBox"
local infoBoxText = "BattleUI.MainInfoBox.InformationText"

local function battlerButtonPressFunc(uiObjPtr, justClicked)
    if not justClicked then return end
end

local function battlerButtonHoverFunc(uiObjPtr, justClicked)
    if not justClicked then return end
    local battlerData = battler.battlers[battler.buttonToBattlerLookup[uiObjPtr]]
    local infoBoxTextData = ui.lookup[infoBoxText].data
    ui.UpdateText(infoBoxTextData, battlerData.stats.Name)
end

---comment
---@param battlerData battler
local function atbUpdate(battlerData)
    battlerData.currentATB = battlerData.currentATB + (battlerData.stats.Spd + 1) * gamestate.DeltaTimeSeconds
    battlerData.currentATB = math.min(battlerData.currentATB, battlerData.maxATB)
end

local function calculateMaxATB(battlerData)
    return 2
end

local function battlerCreateBase(go, stats, isPlayer)
    gamestate.battle.currentBattlers[go] = {}
    local battlerTable = gamestate.battle.currentBattlers[go]
    -- battler.battlers[go].sprite = engine.NewSprite(stats.Image, go,
    battlerTable.sprite = engine.Sprite.NewSprite(stats.Image, go, { 0, 0, stats.OffsetAndSize.w, stats.OffsetAndSize.h },
        stats.OffsetAndSize)
    battlerTable.animator = engine.Animation.CreateAnimator(stats.Image, battlerTable.sprite)
    battlerTable.stats = stats
    engine.Gameobject.SetType(go, GameObjectTypes.Battler)
    local x, y        = engine.Gameobject.Position(go)
    x                 = x + stats.OffsetAndSize.x
    y                 = y + stats.OffsetAndSize.y
    local w           = stats.OffsetAndSize.w
    local h           = stats.OffsetAndSize.h
    local parentTable = ui.lookup[battlerButtonsName]
    local button      = ui.CreateButtonAtRuntime(stats.Name, { x = x, y = y, w = w, h = h }, parentTable,
        battlerButtonPressFunc, battlerButtonHoverFunc, false)
    ui.SetObjectActive(button, false)
    battlerTable.button = button
    battlerTable.isPlayer = isPlayer
    battlerTable.currentATB = 0
    battlerTable.maxATB = calculateMaxATB(battlerTable)
    return battlerTable
end

-- Not called by engine, as we create these based on spawn points.
function BattlerPlayerCreate(go, stats)
    battlerCreateBase(go, stats, true)
end

function BattlerEnemyCreate(go, stats)
    local battlerTable = battlerCreateBase(go, stats, false)
    if not gamestate.battle.currentTarget then gamestate.battle.currentTarget = battlerTable end
end

function BattlerStart(go)
    local battlerData = gamestate.battle.currentBattlers[go]
    if battlerData == nil then return end
    local animation = "idleR"
    if not battlerData.isPlayer then
        animation = "moveR"
    end
    -- TODO probably not needed
    engine.Animation.SetAnimatorSpeed(battlerData.animator, 1.0)
    engine.Animation.PlayAnimation(battlerData.animator, animation)
end

---updates the atb bar.
---@param battlerData battler
local function updateBattlerATB(battlerData)
    local atbPercent = math.floor(battlerData.currentATB / battlerData.maxATB * 100)
    if atbPercent < 100 then
        atbUpdate(battlerData)
        if battlerData.isPlayer then
            ui.UpdateProgressBarPercent(
                ui.lookup["BattleUI.MainUIBox.SelectionsVLG.Player1Panel.Player1HLG.ATBBarAnimImage.ProgressBar"].data,
                atbPercent)
        end
        atbPercent = math.floor(battlerData.maxATB / battlerData.currentATB * 100)
        if atbPercent == 100 then
            if battlerData.isPlayer then
                ui.PlayAnimation(
                    ui.lookup["BattleUI.MainUIBox.SelectionsVLG.Player1Panel.Player1HLG.ATBBarAnimImage"].data,
                    "turn")
                ui.SetObjectVisible(ui.lookup["BattleUI.P1CommandsBox"].data, true)
                ui.SetObjectVisible(ui.lookup["BattleUI.Selections Finger"].data, true)
                ui.SetObjectActive(ui.lookup["BattleUI.P1CommandsBox.AttackButton"].data, true)
            end
        end
    end
end

function BattlerUpdate(go)
    local battlerData = gamestate.battle.currentBattlers[go]
    updateBattlerATB(battlerData)
end

function BattlerDestroy(go)
    local battlerData = gamestate.battle.currentBattlers[go]
    engine.Animation.DestroyAnimator(battlerData.animator)
    gamestate.battle.currentBattlers[go] = nil
end

function battler.RegisterBattlerFunctions()
    engine.Gameobject.RegisterGameObjectFunctions(GameObjectTypes.Battler, {
        nil,
        BattlerStart,
        BattlerUpdate,
        BattlerDestroy,
    })
end

function battler.DisplayBattlerTargetedDuringTargetSelectionPhase()
    -- Show name of target in the display box
    -- Change the color of the sprite
end

function battler.EndBattlerTargeted()
    -- Close display box
    -- Change the color of the sprite back to normal
end

return battler
