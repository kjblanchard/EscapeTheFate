local engine = require("Engine")
local ui = require("UI")
local buttons = {
    Up = 1,
    Right = 2,
    Down = 3,
    Left = 4,
    A = 5,
    B = 6
}
local buttonAssignments = {}

local function panelStartFunc()
    local upData = ui.UIInstance["ControllerUI"].children["UpButton"].data
    buttonAssignments[upData] = buttons.Up
    local rightData = ui.UIInstance["ControllerUI"].children["RightButton"].data
    buttonAssignments[rightData] = buttons.Right
end

local function displayButtonPressed(button)
    if button == 1 then
        return "up"
    elseif button == 2 then
        return "right"
    elseif button == 3 then
        return "down"
    elseif button == 4 then
        return "left"
    elseif button == 5 then
        return "a"
    elseif button == 6 then
        return "b"
    else
        return "no button mapped"
    end
end
local function buttonPressed(uiobjPtr)
    engine.Log.LogWarn("Button " .. displayButtonPressed(buttonAssignments[uiobjPtr]) .. " pressd")
end

local function buttonHovered(uiobjPtr)
    -- engine.Log.LogWarn("Button " .. displayButtonPressed(buttonAssignments[uiobjPtr]) .. " hovered")
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

    }
}

return returnTable
