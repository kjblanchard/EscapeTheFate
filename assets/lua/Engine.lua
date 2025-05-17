-- luacheck: globals cAudio cLog
-- luacheck: undefined-global cLog
-- luacheck: undefined-global cAudio
local engine = {}
engine.currentScene = {}
function engine.LogDebug(message)
    cLog.Log(message, 1)
end

function engine.LogInfo(message)
    cLog.Log(message, 2)
end

function engine.LogWarn(message)
    cLog.Log(message, 3)
end

function engine.LogError(message)
    cLog.Log(message, 4)
end

function engine.LogCritical(message)
    cLog.Log(message, 5)
end

function engine.PlaySfxOneShot(soundName, volume)
    cAudio.PlaySfx(soundName, volume)
end

function engine.PlayBGM(soundName, volume)
    cAudio.PlayBgm(soundName, volume)
end

function engine.RegisterGameObjectFunctions(typeNumber, funcTable)
    cGameObject.NewGameObjectType(typeNumber, funcTable)
end

function engine.LoadTilemap(mapname)
    cScene.LoadMap(mapname)
end

function engine.LoadTilemapObjects()
    cScene.LoadObjectsOnMap()
end

---Sets all current gameobjects to be destroyed the next time DestroyGameObjects is called
---@param force boolean|nil default false | If set to true, even if DoNotDestroy is set on the gameobject, set it to be destroyed..
function engine.SetGameObjectsToBeDestroyed(force)
    if force == nil then
        force = false
    end
    cGameObject.SetDestroyGameObjects(force)
end

---Destroys all gameobjects that have ToBeDestroyed set on them.
function engine.DestroyGameObjects()
    cGameObject.DestroyGameObjects()
end

function engine.LoadScene(mapname, uiname, bgm, volume)
    engine.LoadTilemap(mapname)
    engine.SetGameObjectsToBeDestroyed(false)
    engine.LoadTilemapObjects()
    engine.DestroyGameObjects()
    local ui = require("UI")
    -- Destroy all ui panels that are not donotdestroy.
    for _, value in pairs(ui.UIInstance) do
        if value.data ~= nil and value.doNotDestroy == false then
            ui.DestroyPanel(value)
        end
    end
    -- Load ui if needed
    if uiname ~= nil then
        local testui = require(uiname)
        ui.CreatePanelFromTable(testui)
    end
    if bgm ~= nil then engine.PlayBGM(bgm, volume) end
end

function engine.SetWindowOptions(windowSize, windowHeight, windowName)
    cEngine.SetWindowOptions(windowSize, windowHeight, windowName)
end

function engine.SetScalingOptions(worldWidth, WorldHeight)
    cEngine.SetScalingOptions(worldWidth, WorldHeight)
end

return engine
