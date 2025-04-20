local ui = require("UI")
local engine = require("Engine")
if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
    require("lldebugger").start()
end

local testui = require("uitest")
ui.CreatePanelFromTable(testui)
