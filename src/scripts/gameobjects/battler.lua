local engine = require("Engine")
local gameobject = require("GameObject")
local battler = {}
battler.battlers = {}

local function battlerCreateBase(go, stats)
    battler.battlers[go] = {}
    battler.battlers[go].sprite = engine.NewSprite(stats.Image, go,
        { 0, 0, stats.OffsetAndSize.w, stats.OffsetAndSize.h }, stats.OffsetAndSize)
    battler.battlers[go].animator = engine.CreateAnimator(stats.Image, battler.battlers[go].sprite)
    gameobject.SetType(go, GameObjectTypes.Battler)
end

function BattlerObjectCreate(go, stats)
    battlerCreateBase(go, stats)
    battler.battlers[go].isPlayer = true
end

function BattlerEnemyCreate(go, stats)
    battlerCreateBase(go, stats)
    battler.battlers[go].isPlayer = false
end

function BattlerStart(go)
    local battlerData = battler.battlers[go]
    if battlerData == nil then return end
    local animation = "idleR"
    if not battlerData.isPlayer then
        animation = "moveR"
    end
    engine.PlayAnimation(battlerData.animator, animation)
end

function BattlerUpdate(go)
end

function BattlerDestroy(go)
    engine.DestroyAnimator(battler.battlers[go].animator)
    battler.battlers[go].sprite = nil
    battler.battlers[go].animator = nil
    battler.battlers[go] = nil
end

function battler.RegisterBattlerFunctions()
    engine.RegisterGameObjectFunctions(GameObjectTypes.Battler, {
        nil,           -- index 1 = create
        BattlerStart,
        BattlerUpdate, -- index 2 = start
        BattlerDestroy,
    })
end

return battler
