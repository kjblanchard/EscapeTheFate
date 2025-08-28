local engine = require("Engine")
local ui = require("UI")
local battler = {}
battler.battlers = {}

local battlerTargeted = nil

local function battlerButtonPressFunc(uiObjPtr, justClicked)
    if not justClicked then return end

end

local function battlerButtonHoverFunc(uiObjPtr, justClicked)
    -- engine.Audio.PlaySfxOneShot("hit1", 1.0)
end

local function battlerCreateBase(go, stats)
    battler.battlers[go] = {}
    -- battler.battlers[go].sprite = engine.NewSprite(stats.Image, go,
    battler.battlers[go].sprite = engine.Sprite.NewSprite(stats.Image, go,
        { 0, 0, stats.OffsetAndSize.w, stats.OffsetAndSize.h }, stats.OffsetAndSize)
    battler.battlers[go].animator = engine.Animation.CreateAnimator(stats.Image, battler.battlers[go].sprite)
    engine.Gameobject.SetType(go, GameObjectTypes.Battler)
    local x, y        = engine.Gameobject.Position(go)
    x                 = x + stats.OffsetAndSize.x
    y                 = y + stats.OffsetAndSize.y
    local w           = stats.OffsetAndSize.w
    local h           = stats.OffsetAndSize.h
    local parentTable = ui.lookup["BattleUI.EnemyBattlerButtons"]
    local button      = ui.CreateButtonAtRuntime(stats.Name, { x = x, y = y, w = w, h = h }, parentTable,
        battlerButtonPressFunc,
        battlerButtonHoverFunc, false)
    ui.SetObjectActive(button, false)
end

function BattlerObjectCreate(go, stats)
    battlerCreateBase(go, stats)
    battler.battlers[go].isPlayer = true
end

function BattlerEnemyCreate(go, stats)
    battlerCreateBase(go, stats)
    battler.battlers[go].isPlayer = false
    if not battlerTargeted then battlerTargeted = battler.battlers[go] end
end

function BattlerStart(go)
    local battlerData = battler.battlers[go]
    if battlerData == nil then return end
    local animation = "idleR"
    if not battlerData.isPlayer then
        animation = "moveR"
    end
    -- TODO probably not needed
    engine.Animation.SetAnimatorSpeed(battlerData.animator, 1.0)
    engine.Animation.PlayAnimation(battlerData.animator, animation)
end

function BattlerUpdate(go)
end

function BattlerDestroy(go)
    engine.Animation.DestroyAnimator(battler.battlers[go].animator)
    battler.battlers[go].sprite = nil
    battler.battlers[go].animator = nil
    battler.battlers[go] = nil
end

function battler.RegisterBattlerFunctions()
    engine.Gameobject.RegisterGameObjectFunctions(GameObjectTypes.Battler, {
        nil,           -- index 1 = create
        BattlerStart,
        BattlerUpdate, -- index 2 = start
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
