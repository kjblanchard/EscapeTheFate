-- local ui = require("UI")
local engine = require("Engine")
local scheduler = require("Scheduler")
local player = require("gameobjects.player")
local scenes = require("scenes")
local config = require("gameConfig")
local debugh = require("debugh")
local dialog = require("gameobjects.dialogBox")
if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
    require("lldebugger").start()
end

local function handleNextScene()
    if engine.nextScene ~= nil then
        local sceneTable = scenes.scenes[engine.nextScene]
        local co = LoadSceneCo(sceneTable[1], sceneTable[2], sceneTable[3], sceneTable[4], sceneTable[5], sceneTable[6])
        scheduler:run(co)
        engine.nextScene = nil
    end
end

local function update()
    scheduler:update(engine.DeltaTimeInSeconds())
    handleNextScene()
end

local function draw()
    for _, value in pairs(debugh.DrawRects) do
        engine.DrawRectCamOffset(value)
    end
end

engine.SetWindowOptions(960, 540, "Escape The Fate")
engine.SetScalingOptions(480, 270)
engine.SetUpdateFunc(update)
engine.SetDrawFunc(draw)
engine.SetGlobalBGMVolume(config.audio.bgmVolume)
engine.SetGlobalSFXVolume(config.audio.sfxVolume)
player.RegisterPlayerFunctions()
dialog.RegisterDebugBoxFunctions()
engine.LoadDefaultScene()
