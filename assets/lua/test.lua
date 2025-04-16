if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
    require("lldebugger").start()
end
UI.CreatePanel("TestLuaPanel", { x = 0, y = 0, w = 255, h = 255 }, nil)
print('Hello from lua!')
