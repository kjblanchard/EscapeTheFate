local function startFunction()
    local dialogSystem = require("dialogSystem")
    local gamestate = require("gameState")
    local ui = require("UI")
    local textPtr = ui.UITree["ui.uitest"].children.textBox
    dialogSystem.InitializeDialogSystem(textPtr)
    gamestate.interactionImageTable = ui.UITree["ui.uitest"].children.interactionImage
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
                    rect = { 8, 8, 140, 54 },
                    text = "Hello World!",
                    centerX = true,
                    centerY = true,
                }
            }
        },
        interactionImage = {
            visible = false,
            class = "image",
            color = { 255, 255, 255, 255 },
            rect = { 0, 0, 16, 16 },
            srcRect = { 0, 0, 16, 16 },
            scale = 2.0,
            camera = true,
            filename = "interaction"
        }
    },
}
return uitest
