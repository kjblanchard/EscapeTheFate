-- local ui = require("UI")
local engine = require("Engine")
if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
    require("lldebugger").start()
end

engine.RegisterGameObjectFunctions(1, {
    function(userdata, go) engine.LogWarn("Just created!") end, -- index 1 = create
    function(go) engine.LogWarn("Just started") end,            -- index 2 = start
    function(go) engine.LogWarn("Updating!") end                -- index 3 = update
})
-- // SetWindowOptions(1920, 1080, "Escape The Fate"); // SetScalingOptions(480, 270);
-- engine.SetWindowOptions(1920, 1080, "Escape The Fate")
engine.SetWindowOptions(960, 540, "Escape The Fate")
engine.SetScalingOptions(480, 270)
engine.LoadScene("title", "uitest")
-- ui.CreatePanelFromTable(testui)
