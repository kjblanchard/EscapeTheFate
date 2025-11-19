local engine = require("Engine")
local config = require("gameConfig")
local loader = require("gameobjects.gameobjects")
local gamestate = require("gameState")
local ui = require("UI")
local scenes = require("scenes")

local function update()
    engine.EngineUpdate()
    if gamestate.nextMap then
        loader.EndAllGameObjects()
        engine.Map.LoadScene(scenes.scenes[gamestate.nextMap], loader)
        gamestate.loadingMap = true
        gamestate.nextMap = false
    end
    loader.UpdateAllGameObjects()

    if engine.Input.KeyboardKeyJustPressed(engine.Input.Buttons.B) then
        engine.Audio.PlaySfxOneShot("enemyDead", 1.0)
    end
end

local function draw()
    ui.DrawUI()
    engine.DrawAllDebugBoxes()
end

local function quit()
    ui.DestroyUI()
    loader.EndAllGameObjects(true)
end
engine.Window.SetScalingOptions(config.window.x, config.window.y)
engine.Window.SetWindowOptions(config.window.xWin, config.window.yWin, config.window.title)
engine.SetUpdateFunc(update)
--engine.SetInputFunc(handleInput)
engine.SetDrawFunc(draw)
engine.SetQuitFunc(quit)
engine.Audio.SetGlobalBGMVolume(config.audio.bgmVolume)
engine.Audio.SetGlobalSFXVolume(config.audio.sfxVolume)
engine.Map.LoadScene(scenes.scenes[scenes.default], loader)
