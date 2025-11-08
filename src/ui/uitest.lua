local function startFunction ()
    local dialogSystem = require("dialogSystem")
    local ui = require("UI")
    local textPtr = ui.UITree["ui.uitest"].children.textBox
    dialogSystem.InitializeDialogSystem(textPtr)
end

local uitest = {
    startFunc = startFunction,
    children = {
        textBox = {
            visible = false,
            class = "nineSlice",
            color = { 80, 0, 120, 235 },
            rect = { 60, 60, 156, 70 },
            xSize = 8,
            ySize = 9,
            filename = "uibase-export",
            children = {
                textTest = {
                    class = "text",
                    font = "PressStart2P",
                    fontSize = 8,
                    rect = {8,8,140,54},
                    text = "Hello World!",
                    centerX = true,
                    centerY = true,
                }
            }
        },
    },
}
return uitest
