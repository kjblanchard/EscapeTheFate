-- Each scene has a "key", which should be the name that you will reference when loading a scene in the ui. then:
-- Tiled tmx file name with no extension, ui name, bgm name, volume, fadein time, fadeout time
-- By default, player will be loaded at position 0 on the map.  You should set a default map as well, which is a key that you will load by default.
return {
    default = "title",
    -- default = "debugTown",
    scenes = {
        title = { "title", "uitest", "town1", 0, 0.1, 0.1 },
        debugTown = { "debugTown", nil, "town2", 1.0, 1.0, 0.5 }
    }
}
