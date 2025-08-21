local engine = require("Engine")
local scenes = require("scenes")
local config = require("gameConfig")
local debugh = require("debugh")
local dialog = require("gameobjects.dialogBox")
local player = require("gameobjects.player")
local startBox = require("gameobjects.startBox")
local exitBox = require("gameobjects.exitBox")
local battleZone = require("gameobjects.battleZone")
local battleLocation = require("gameobjects.battleLocation")
local battle = require("gameobjects.battle")
local battler = require("gameobjects.battler")
local dialogSystem = require("dialog")
local gamestate = require("gameState")
if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
    require("lldebugger").start()
end

local function handleNextScene()
    if gamestate.nextScene ~= nil then
        local sceneTable = scenes.scenes[gamestate.nextScene]
        if sceneTable ~= nil then
            local co = engine.Scene.LoadSceneCo(sceneTable[1], sceneTable[2], sceneTable[3], sceneTable[4], sceneTable
                [5],
                sceneTable[6])
            engine.Coroutine.run(co)
            -- scheduler:run(co)
        end
        gamestate.nextScene = nil
        gamestate.sceneChange = true
    end
end

local function handleInput()
    engine.Input.Update()
end

local function update()
    engine.EngineUpdate()
    handleNextScene()
    dialogSystem.UpdateDialogBoxes(gamestate.DeltaTimeSeconds)
end

local function draw()
    -- for _, value in pairs(debugh.DrawRects) do
    --     engine.Draw.DrawRectCamOffset(value)
    -- end
end

engine.Window.SetWindowOptions(960, 540, "Escape The Fate")
engine.Window.SetScalingOptions(480, 270)
engine.SetUpdateFunc(update)
engine.SetInputFunc(handleInput)
engine.SetDrawFunc(draw)
engine.Audio.SetGlobalBGMVolume(config.audio.bgmVolume)
engine.Audio.SetGlobalSFXVolume(config.audio.sfxVolume)
player.RegisterPlayerFunctions()
dialog.RegisterDebugBoxFunctions()
exitBox.RegisterDebugBoxFunctions()
startBox.RegisterDebugBoxFunctions()
battleZone.RegisterDebugBoxFunctions()
battleLocation.RegisterDebugBoxFunctions()
battle.RegisterDebugBoxFunctions()
engine.Scene.LoadDefaultScene()
battler.RegisterBattlerFunctions()
