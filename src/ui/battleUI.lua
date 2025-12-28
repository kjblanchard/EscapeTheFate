local function startFunction()
    local gamestate = require("gameState")
    local battleSystem = require("battleSystem")
    local ui = require("UI")
    -- local textPtr = ui.UITree["ui.uitest"].children.textBox
    -- dialogSystem.InitializeDialogSystem(textPtr)
    -- gamestate.interactionImageTable = ui.UITree["ui.uitest"].children.interactionImage
     battleSystem.InitializeUIVariables()
end

local commandItemSpacing = 14

local battleUI = {
    startFunc = startFunction,
    children = {
        textBox = {
            visible = true,
            class = "nineSlice",
            color = { 80, 0, 120, 245 },
            rect = { 330, 40, 72, 72 },
            xSize = 8,
            ySize = 9,
            filename = "uibase-export",
            children = {
                holder = {
                    rect = { 8, 10, 56, 12 },
                    children = {
                        attackText = {
                            class = "text",
                            font = "PressStart2P",
                            fontSize = 8,
                            rect = { 0, 0, 56, 12 },
                            text = "Attack",
                            centerX = true,
                            centerY = true,
                        },
                        magicText = {
                            class = "text",
                            font = "PressStart2P",
                            fontSize = 8,
                            rect = { 0, commandItemSpacing, 56, 12 },
                            text = "Magic",
                            centerX = true,
                            centerY = true,
                        },
                        skillText = {
                            class = "text",
                            font = "PressStart2P",
                            fontSize = 8,
                            rect = { 0, commandItemSpacing * 2, 56, 12 },
                            text = "Skills",
                            centerX = true,
                            centerY = true,
                        },
                        itemsText = {
                            class = "text",
                            font = "PressStart2P",
                            fontSize = 8,
                            rect = { 0, commandItemSpacing * 3, 56, 12 },
                            text = "Items",
                            centerX = true,
                            centerY = true,
                        }
                    }
                }
            }
        },
        interactionImage = {
            visible = true,
            class = "image",
            color = { 255, 255, 255, 255 },
            rect = { 350, 48, 16, 16 },
            srcRect = { 16, 32, 16, 16 },
            scale = 2.0,
            filename = "fingers"
        }
    },
}
return battleUI
