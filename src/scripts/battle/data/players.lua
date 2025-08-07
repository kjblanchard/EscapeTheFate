---@class Player
---@field Name string     -- Name to be displayed in various UI elements
---@field HP   number
---@field MP number            -- Background number
---@field Str number            -- Background number
---@field Def number            -- Background number
---@field Spd number            -- Background number
---@field Image number | nil            -- Background number
---@field Moves table{integer}            -- Background number


---@type  Player[]
local players = {
    {
        Name = "Kevin",
        HP = 5,
        MP = 0,
        Str = 1,
        Def = 1,
        Spd = 1,
        Image = nil,
        Moves = {}
    }
}
