---@class BattleZone
---@field enemyGroups number[]     -- IDs of enemy groups
---@field groupPercents number[]  -- Percent chance of each group
---@field bgNum number            -- Background number
---@field stepCount number            -- Background number

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
