local engine = require("Engine")
local battlerClass = {}
---@class Stats
---@field Name string
---@field HP number
---@field Strength number
---@field Defense number
---@field Speed number

---@class Battler
---@field X number
---@field Y number
---@field CurrentHP number
---@field Stats Stats
---@field ATBMax number
---@field ATBCurrent number

---@type table[Battler]
local battlers = {}

---@param battler Battler
local function updateATB(battler)
    -- battler.ATBCurrent = battler.ATBCurrent + 1 + battler.Stats.Speed
    battler.ATBCurrent = battler.ATBCurrent + 1 + 1
end

---@param battler Battler
local function handleTurn(battler)
    battler.ATBCurrent = 0
end



---@param battler Battler
local function battlerUpdate(battler)
    updateATB(battler)
    if battler.ATBCurrent >= battler.ATBMax then handleTurn(battler) end
end

-- local function battlerEnd(battler)
-- end

---@param stats Stats
---@param offsetRect table offset rect
---@param animSheet string name of file to load
---@param idleAnimation string name of idle anim override
function battlerClass.New(x, y, stats, offsetRect, animSheet, idleAnimation)
    ---@type Battler
    local newBattler = {
        X = x,
        Y = y,
        CurrentHP = stats and stats.HP or 0,
        Stats = stats,
        ATBMax = 100,
        ATBCurrent = 0,
        Gameobject = nil,
        Sprite = nil,
        Animator = nil,
        IdleAnim = idleAnimation or "moveR"
    }
    table.insert(battlers, newBattler)
    -- We need to load the actual image here.
    newBattler.Gameobject = engine.Gameobject.CreateGameObject()
    engine.Gameobject.SetPosition(newBattler.Gameobject, x, y)
    -- We need to lookup the sprite info in the data somewhere.
    newBattler.Sprite = engine.Sprite.NewSprite(animSheet, newBattler.Gameobject, { 0, 0, offsetRect[3], offsetRect[4] },
        offsetRect)
    newBattler.Animator = engine.Animation.CreateAnimator(animSheet, newBattler.Sprite)
    engine.Animation.SetAnimatorSpeed(newBattler.Animator, 1.0)
    engine.Animation.PlayAnimation(newBattler.Animator,newBattler.IdleAnim)
end

function battlerClass.Update()
    for i = 1, #battlers, 1 do
        local battler = battlers[i]
        battlerUpdate(battler)
    end
end

function battlerClass.End() end

return battlerClass
