-- local ui = require("UI")
print("Even do here is what no u")
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
        local co = engine.LoadSceneCo(sceneTable[1], sceneTable[2], sceneTable[3], sceneTable[4], sceneTable[5],
            sceneTable[6])
        scheduler:run(co)
        engine.nextScene = nil
        engine.sceneChange = true
    end
end

local function handleInput()
    engine.Input.Update()
end

local function update()
    handleNextScene()
    scheduler:update(engine.DeltaTimeInSeconds())
end

local function draw()
    for _, value in pairs(debugh.DrawRects) do
        engine.DrawRectCamOffset(value)
    end
end

print("Even do here is what no u")
engine.Log.LogWarn("What even")

engine.SetWindowOptions(960, 540, "Escape The Fate")
engine.SetScalingOptions(480, 270)
engine.SetUpdateFunc(update)
engine.SetInputFunc(handleInput)
engine.SetDrawFunc(draw)
engine.SetGlobalBGMVolume(config.audio.bgmVolume)
engine.SetGlobalSFXVolume(config.audio.sfxVolume)
player.RegisterPlayerFunctions()
dialog.RegisterDebugBoxFunctions()
engine.LoadDefaultScene()
print("FInished file")
