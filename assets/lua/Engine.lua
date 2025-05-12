-- luacheck: globals cAudio cLog
-- luacheck: undefined-global cLog
-- luacheck: undefined-global cAudio
local engine = {}
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

function engine.RegisterGameObjectFunctions(typeNumber, funcTable)
    cGameObject.NewGameObjectType(typeNumber, funcTable)
end

return engine
