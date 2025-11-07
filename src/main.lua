local engine = require("Engine")
local config = require("gameConfig")
local loader = require("gameobjects.gameobjects")
local ui = require("UI")
local scenes = require("scenes")

local function update()
    engine.EngineUpdate()
    loader.UpdateAllGameObjects()
end

local function draw()
    ui.DrawUI()
    engine.DrawAllDebugBoxes()
end
engine.Window.SetScalingOptions(config.window.x, config.window.y)
engine.Window.SetWindowOptions(config.window.xWin, config.window.yWin,config.window.title)
engine.SetUpdateFunc(update)
--engine.SetInputFunc(handleInput)
engine.SetDrawFunc(draw)
engine.Audio.SetGlobalBGMVolume(config.audio.bgmVolume)
engine.Audio.SetGlobalSFXVolume(config.audio.sfxVolume)
engine.Scene.LoadScene(scenes.scenes[scenes.default], loader)
