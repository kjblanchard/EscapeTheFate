local textBoxRect = { x = 330, y = 40, w = 72, h = 72 }
local commandsOffsetFromBox = { x = 8, y = 10, w = 56, h = 12 }
local commandItemSpacing = 14

local function startFunction()
    -- local gamestate = require("gameState")
    local battleSystem = require("battleSystem")
    local ui = require("UI")
    local engine = require("Engine")
    local fingerUIObject = ui.UITree["ui.battleUI"].children.finger
    local magicTextUIObject = ui.UITree["ui.battleUI"].children["textBox"].children["holder"].children["skillText"]
    engine.Log.LogWarn(magicTextUIObject.rect.x)
    engine.Log.LogWarn(magicTextUIObject.rect.y)
    -- magicTextUIObject = ui.UITree["ui.battleUI"].children["textBox"].children["holder"].children["attackText"]
    -- engine.Log.LogWarn(magicTextUIObject.rect.x)
    -- engine.Log.LogWarn(magicTextUIObject.rect.y)
    -- dialogSystem.InitializeDialogSystem(textPtr)
    -- gamestate.interactionImageTable = ui.UITree["ui.uitest"].children.interactionImage
    battleSystem.InitializeUIVariables(fingerUIObject, magicTextUIObject)
end


local battleUI = {
    startFunc = startFunction,
    children = {
        textBox = {
            visible = true,
            class = "nineSlice",
            color = { 80, 0, 120, 245 },
            rect = textBoxRect,
            xSize = 8,
            ySize = 9,
            filename = "uibase-export",
            children = {
                holder = {
                    rect = commandsOffsetFromBox,
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
        finger = {
            visible = true,
            class = "image",
            color = { 255, 255, 255, 255 },
            rect = { 350, 48, 16, 16 },
            srcRect = { 16, 32, 16, 16 },
            scale = 2.0,
            priority = 1,
            filename = "fingers",
        }
    },
}
return battleUI
