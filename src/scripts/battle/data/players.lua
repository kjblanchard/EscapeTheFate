---@class Player
---@field Name string     -- Name to be displayed in various UI elements
---@field HP   number
---@field MP number            -- Background number
---@field Str number            -- Background number
---@field Def number            -- Background number
---@field Spd number            -- Background number
---@field Image string            -- Background number
-- -@field Moves table{integer}            -- Background number


---@type  Player[]
local players = {
    {
        Name = "Kevin",
        HP = 15,
        MP = 0,
        Str = 2,
        Def = 1,
        Spd = 1,
        Image = "player1BattlerNew",
        OffsetAndSize = { x = -8, y = -18, w = 48, h = 48 },
        -- Moves = {}
    }
}

return players
