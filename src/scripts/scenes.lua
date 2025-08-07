-- Each scene has a "key", which should be the name that you will reference when loading a scene in the ui. then:
-- Tiled tmx file name with no extension, ui name, bgm name, volume, fadein time, fadeout time
-- By default, player will be loaded at position 0 on the map.  You should set a default map as well, which is a key that you will load by default.
-- On the default scene, if you try and use some low values like 0.1 and 0.1 when loading the screen on first load, it can cause problems, so try larger numbers or make them different
---@class Scene
---@field map string     -- IDs of enemy groups
---@field ui string  -- Percent chance of each group
---@field bgm string            -- Background number
---@field volume number            -- Background number
---@field fadein number            -- Background number
---@field fadeout number            -- Background number




return {
    ---@type string
    -- default = "title",
    -- default = "debugTown",
    default = "forest",
    -- default = "debugSouth",
    ---@type  Scene[]
    scenes = {
        title = { "cloud", "uitest", "town1", 1.0, 0.0, 1.0 },
        debugTown = { "debugTown", "town", "town2", 1.0, 1.0, 0.5 },
        debugSouth = { "debugSouth", "town", "forest1", 1.0, 1.0, 0.5 },
        debugTownHome = { "debugTownHome", "town", "town2", 1.0, 1.0, 0.5 },
        forest = { "forest1", "battle", "battle1", 1.0, 1.0, 0.5 }
    }
}
