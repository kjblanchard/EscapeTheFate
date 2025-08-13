---@class BattleZone
---@field enemyGroups number[]     -- IDs of enemy groups
---@field groupPercents number[]  -- Percent chance of each group
---@field bgNum number            -- Background number
---@field stepCount number            -- Background number

local function normalizeWeights(weights)
    local total = 0
    for _, w in ipairs(weights) do
        total = total + w
    end
    local normalized = {}
    if total == 0 then
        -- Avoid division by zero, give equal weights
        local equalWeight = 1 / #weights
        for i = 1, #weights do
            normalized[i] = equalWeight
        end
    else
        for i, w in ipairs(weights) do
            normalized[i] = w / total
        end
    end
    return normalized
end

local function chooseWeightedRandom(items, weights)
    local normalized = normalizeWeights(weights)
    local r = math.random()
    local cumulative = 0
    for i, w in ipairs(normalized) do
        cumulative = cumulative + w
        if r <= cumulative then
            return items[i]
        end
    end
    -- Fallback (in case of floating point issues)
    return items[#items]
end

---@type  BattleZone[]
local battlezones = {
    {
        enemyGroups = { 0 },
        groupPercents = { 1.0 },
        bgNum = 0,
        stepCount = 2.0
    },
    {
        enemyGroups = { 0, 1 },
        groupPercents = { 0.5, 0.5 },
        bgNum = 0,
        stepCount = 2.0
    }
}

---Gets a battle zone from the table.
---@param battleZoneNum integer The number of battle zone you want to get.
---@return BattleZone The battle zone info.
function GetBattleZone(battleZoneNum)
    return battlezones[battleZoneNum]
end

function GetEnemyGroupRandom(battleZoneNum)
    local bg = GetBattleZone(battleZoneNum)
    if not bg then return nil end
    return chooseWeightedRandom(bg.enemyGroups, bg.groupPercents)
end

math.randomseed(os.time())
