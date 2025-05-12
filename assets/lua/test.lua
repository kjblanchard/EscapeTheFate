local ui = require("UI")
local engine = require("Engine")
if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
    require("lldebugger").start()
end

local testui = require("uitest")
engine.RegisterGameObjectFunctions(1, {
    function(userdata, go) engine.LogWarn("Just created!") end, -- index 1 = create
    function(go) engine.LogWarn("Just started") end,            -- index 2 = start
    function(go) engine.LogWarn("Updating!") end                -- index 3 = update
})
ui.CreatePanelFromTable(testui)
