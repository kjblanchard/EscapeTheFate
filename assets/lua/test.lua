-- local ui = require("UI")
local engine = require("Engine")
local scheduler = require("Scheduler")
local player = require("gameobjects.player")
if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
    require("lldebugger").start()
end

local function update()
    scheduler:update(engine.DeltaTimeInSeconds())
end

engine.SetWindowOptions(960, 540, "Escape The Fate")
engine.SetScalingOptions(480, 270)
engine.SetUpdateFunc(update)
player.RegisterPlayerFunctions()
engine.LoadDefaultScene()
