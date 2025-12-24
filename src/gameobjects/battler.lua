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
    engine.Log.LogWarn("Should do turn, just going back to 0 for now for " .. battler.Stats.Name)
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
function battlerClass.New(x, y, stats)
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
        Animator = nil
    }
    table.insert(battlers, newBattler)
    -- We need to load the actual image here.
    newBattler.Gameobject = engine.Gameobject.CreateGameObject()
    engine.Gameobject.SetPosition(newBattler.Gameobject, x, y)
    -- We need to lookup the sprite info in the data somewhere.
    newBattler.Sprite = engine.Sprite.NewSprite("player1Battler", newBattler.Gameobject, { 0, 0, 48, 48 },
        { 0, 0, 48, 48 })
    newBattler.Animator = engine.Animation.CreateAnimator("player1Battler", newBattler.Sprite)
    engine.Animation.SetAnimatorSpeed(newBattler.Animator, 1.0)
    engine.Animation.PlayAnimation(newBattler.Animator, "idle1")
end

function battlerClass.Update()
    for i = 1, #battlers, 1 do
        local battler = battlers[i]
        battlerUpdate(battler)
    end
end

function battlerClass.End() end

return battlerClass
