local engine = require("Engine")
local battler = {}
battler.battlers = {}

local function battlerCreateBase(go, stats)
    battler.battlers[go] = {}
    -- battler.battlers[go].sprite = engine.NewSprite(stats.Image, go,
    battler.battlers[go].sprite = engine.Sprite.NewSprite(stats.Image, go,
        { 0, 0, stats.OffsetAndSize.w, stats.OffsetAndSize.h }, stats.OffsetAndSize)
    battler.battlers[go].animator = engine.Animation.CreateAnimator(stats.Image, battler.battlers[go].sprite)
    engine.Gameobject.SetType(go, GameObjectTypes.Battler)
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

return battler
