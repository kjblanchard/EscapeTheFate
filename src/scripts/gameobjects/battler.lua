local engine = require("Engine")
local gameobject = require("GameObject")
local battler = {}
battler.battlers = {}

function BattlerObjectCreate(go, stats)
    battler.battlers[go] = {}
    battler.battlers[go].sprite = engine.NewSprite(stats.Image, go, { 0, 0, 48, 48 }, { 0, 0, 48, 48 })
    battler.battlers[go].animator = engine.CreateAnimator(stats.Image, battler.battlers[go].sprite)
    gameobject.SetType(go, GameObjectTypes.Battler)
end

function BattlerStart(go)
    local battlerData = battler.battlers[go]
    if battlerData == nil then return end
    engine.PlayAnimation(battlerData.animator, "idleR")
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
