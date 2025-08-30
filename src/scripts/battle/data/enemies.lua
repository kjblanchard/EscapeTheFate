local enemies = {}
-- Enemies is an array of enemy objects
---@type  Stats[]
enemies.enemies = {
    {
        Name = "Birb",
        HP = 5,
        MP = 0,
        Str = 1,
        Def = 1,
        Spd = 1,
        Image = "bBlackBird",
        OffsetAndSize = { x = -12, y = -36, w = 61, h = 57 },
        -- Moves = {}
    },
}
return enemies
