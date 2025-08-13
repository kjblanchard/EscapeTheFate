---@class Enemy
---@field Name string     -- Name to be displayed in various UI elements
---@field HP   number
---@field MP number            -- Background number
---@field Str number            -- Background number
---@field Def number            -- Background number
---@field Spd number            -- Background number
---@field Image string            -- Background number
-- -@field Moves table{integer}            -- Background number

local enemies = {}
-- Enemies is an array of enemy objects
---@type  Enemy[]
enemies.enemies = {
    {
        Name = "Birb",
        HP = 5,
        MP = 0,
        Str = 1,
        Def = 1,
        Spd = 1,
        Image = "bBlackBird",
        OffsetAndSize = { x = -10, y = 10, w = 61, h = 57 },
        -- Moves = {}
    },
    {
        Name = "Orc",
        HP = 15,
        MP = 0,
        Str = 1,
        Def = 1,
        Spd = 1,
        Image = "",
        -- Moves = {}
    }

}
return enemies
