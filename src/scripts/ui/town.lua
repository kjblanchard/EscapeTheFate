local engine = require("Engine")
local ui = require("UI")
local dialog = require("dialog")
local buttonAssignments = {}

local function panelStartFunc()
    if engine.IsMobile() then
        local upData = ui.UIInstance["TownUI"].children["ControllerUI"].children["UpButton"].data
        buttonAssignments[upData] = engine.Buttons.UP
        local rightData = ui.UIInstance["TownUI"].children["ControllerUI"].children["RightButton"].data
        buttonAssignments[rightData] = engine.Buttons.RIGHT
        local downData = ui.UIInstance["TownUI"].children["ControllerUI"].children["DownButton"].data
        buttonAssignments[downData] = engine.Buttons.DOWN
        local leftData = ui.UIInstance["TownUI"].children["ControllerUI"].children["LeftButton"].data
        buttonAssignments[leftData] = engine.Buttons.LEFT
        local aData = ui.UIInstance["TownUI"].children["ControllerUI"].children["AButton"].data
        buttonAssignments[aData] = engine.Buttons.A
        local bData = ui.UIInstance["TownUI"].children["ControllerUI"].children["BButton"].data
        buttonAssignments[bData] = engine.Buttons.B
    end
    table.insert(dialog.DialogBoxes, {
        ptr = ui.UIInstance["TownUI"].children["TextBoxBox"].children["TextBoxText"]
            .data
    })
end


local function buttonPressed(uiobjPtr)
    if not engine.IsMobile() then return end
    engine.Input.UIButtonPresses.Down[buttonAssignments[uiobjPtr]] = true
end

local function buttonHovered(uiobjPtr)
end


local returnTable = {
    name = "TownUI",
    startFunc = panelStartFunc,
    children = {
        {
            name = "ControllerUI",
            type = "Panel",
            doNotDestroy = true,
            isMobile = true,
            children = {
                {
                    name = "UpButton",
                    type = "button",
                    location = { 40, 205, 16, 16 },
                    pressedFunc = buttonPressed,
                    pressOnRelease = false,
                    hoverFunc = buttonHovered,
                    children = {
                        {
                            name = "UpButtonImage",
                            type = "image",
                            imageName = "ui-input-export",
                            location = { 0, 0, 16, 16 },
                            srcRect = { 144, 352, 16, 16 }

                        }
                    }
                },
                {
                    name = "RightButton",
                    type = "button",
                    location = { 60, 220, 16, 16 },
                    pressedFunc = buttonPressed,
                    hoverFunc = buttonHovered,
                    pressOnRelease = false,
                    children = {
                        {
                            name = "RightButtonImage",
                            type = "image",
                            imageName = "ui-input-export",
                            location = { 0, 0, 16, 16 },
                            srcRect = { 160, 352, 16, 16 }

                        }
                    }
                },
                {
                    name = "DownButton",
                    type = "button",
                    location = { 40, 235, 16, 16 },
                    pressedFunc = buttonPressed,
                    pressOnRelease = false,
                    hoverFunc = buttonHovered,
                    children = {
                        {
                            name = "downButtonImage",
                            type = "image",
                            imageName = "ui-input-export",
                            location = { 0, 0, 16, 16 },
                            srcRect = { 176, 352, 16, 16 }

                        }
                    }
                },
                {
                    name = "LeftButton",
                    type = "button",
                    location = { 20, 220, 16, 16 },
                    pressedFunc = buttonPressed,
                    pressOnRelease = false,
                    hoverFunc = buttonHovered,
                    children = {
                        {
                            name = "leftButtonImage",
                            type = "image",
                            imageName = "ui-input-export",
                            location = { 0, 0, 16, 16 },
                            srcRect = { 192, 352, 16, 16 }

                        }
                    }
                },
                {
                    name = "AButton",
                    type = "button",
                    location = { 375, 220, 16, 16 },
                    pressedFunc = buttonPressed,
                    pressOnRelease = false,
                    hoverFunc = buttonHovered,
                    children = {
                        {
                            name = "aButtonImage",
                            type = "image",
                            imageName = "ui-input-export",
                            transparency = 200,
                            location = { 0, 0, 42, 42 },
                            srcRect = { 128, 0, 16, 16 }

                        }
                    }
                },
                {
                    name = "BButton",
                    type = "button",
                    location = { 420, 200, 16, 16 },
                    pressedFunc = buttonPressed,
                    pressOnRelease = false,
                    hoverFunc = buttonHovered,
                    children = {
                        {
                            name = "aButtonImage",
                            type = "image",
                            transparency = 200,
                            imageName = "ui-input-export",
                            location = { 0, 0, 16, 16 },
                            srcRect = { 144, 0, 16, 16 }

                        }
                    }
                },
            }
        },
        {
            name = "TextBoxBox",
            type = "9slice",
            location = { 20, 185, 185, 75 },
            color = { 80, 0, 120, 235 },
            imageName = "uibase-export",
            alpha = 190,
            children = {
                {
                    name = "TextBoxText",
                    type = "text",
                    font = "PressStart2P",
                    size = 8,
                    wordWrap = true,
                    centeredX = true,
                    centeredY = true,
                    text = "Default Text",
                    location = { 8, 8, 169, 59 }
                },

            }
        },
    }
}

return returnTable
