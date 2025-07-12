local engine = require("Engine")
local ui = require("UI")
local buttonAssignments = {}

local function panelStartFunc()
    local upData = ui.UIInstance["ControllerUI"].children["UpButton"].data
    buttonAssignments[upData] = engine.Buttons.UP
    local rightData = ui.UIInstance["ControllerUI"].children["RightButton"].data
    buttonAssignments[rightData] = engine.Buttons.RIGHT
    local downData = ui.UIInstance["ControllerUI"].children["DownButton"].data
    buttonAssignments[downData] = engine.Buttons.DOWN
    local leftData = ui.UIInstance["ControllerUI"].children["LeftButton"].data
    buttonAssignments[leftData] = engine.Buttons.LEFT
    local aData = ui.UIInstance["ControllerUI"].children["AButton"].data
    buttonAssignments[aData] = engine.Buttons.A
    local bData = ui.UIInstance["ControllerUI"].children["BButton"].data
    buttonAssignments[bData] = engine.Buttons.B
end

local function buttonPressed(uiobjPtr)
    engine.Input.UIButtonPresses.Down[buttonAssignments[uiobjPtr]] = true
end

local function buttonHovered(uiobjPtr)
end

local returnTable = {
    name = "ControllerUI",
    doNotDestroy = false,
    startFunc = panelStartFunc,
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
}

return returnTable
