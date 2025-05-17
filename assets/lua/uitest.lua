local engine = require("Engine")
local ui = require("UI")
local _currentButtonHovered = 0
local _fingerButtonPtr = nil

local function newGameButtonClickFunc(uiObjPtr)
    engine.PlaySfxOneShot("menuSelect", 1.0)
    engine.LoadScene("debugTown", nil, "town2", 1.0)
end

local function optionGameButtonClickFunc(uiObjPtr)
    engine.PlaySfxOneShot("error1", 1.0)
end

local function continueGameButtonClickFunc(uiObjPtr)
    engine.PlaySfxOneShot("error2", 1.0)
end

-- called after all the ui objects are loaded in, so you can get things by data
local function panelStartFunc()
    _fingerButtonPtr = ui.UIInstance["Test Panel"].children["SelectionsPanel"].children["Selections Finger"].data
end

local function buttonHoverFunc(uiObjPtr, isJustHovered)
    if _currentButtonHovered == uiObjPtr or isJustHovered == false then return end
    local hoverdLocationX, hoveredLocationY = ui.GetObjectLocation(uiObjPtr)
    hoverdLocationX                         = hoverdLocationX - 8 --offset from words
    ui.SetObjectLocation(_fingerButtonPtr, hoverdLocationX, hoveredLocationY)
    engine.PlaySfxOneShot("menuMove", 1.0)
    _currentButtonHovered = uiObjPtr
end

-- Top level is always a panel, it just needs a name and then you can put all your children inside of it
local returnTable = {
    name = "Test Panel",
    startFunc = panelStartFunc,
    doNotDestroy = false,
    children = {
        {
            name = "Title BG",
            type = "rect",
            location = { 40, 10, 430, 75 },
            color = { 0, 0, 0, 190 }
        },
        {
            name = "Title",
            type = "text",
            font = "PressStart2P",
            size = 28,
            wordWrap = true,
            centeredX = true,
            centeredY = true,
            text = "Escape The Fate",
            location = { 40, 10, 430, 75 }
        },
        {
            name = "SelectionsPanel",
            type = "panel",
            location = { 0, 0, 0, 0 },
            children = {
                {
                    name = "Selections BG",
                    type = "rect",
                    location = { 48, 198, 156, 70 },
                    color = { 0, 0, 0, 190 }
                },
                {
                    name = "SelectionsVLG",
                    type = "vlg",
                    location = { 48, 203, 156, 70 },
                    spacing = 20,
                    children = {
                        {
                            name = "New Game Button",
                            type = "button",
                            location = { 0, 0, 156, 20 },
                            pressedFunc = newGameButtonClickFunc,
                            hoverFunc = buttonHoverFunc,
                            -- userdata = { pressed = newGameButtonClickFunc },
                            children = {
                                {
                                    name = "New Game",
                                    type = "text",
                                    size = 16,
                                    centeredX = true,
                                    centeredY = true,
                                    wordWrap = true,
                                    text = "New Game",
                                    location = { 0, 0, 156, 20 }
                                }
                            }
                        },
                        {
                            name = "Continue Button",
                            type = "button",
                            location = { 0, 0, 156, 20 },
                            pressedFunc = continueGameButtonClickFunc,
                            hoverFunc = buttonHoverFunc,
                            -- userdata = { pressed = newGameButtonClickFunc },
                            children = {
                                {
                                    name = "Continue",
                                    type = "text",
                                    size = 16,
                                    centeredX = true,
                                    centeredY = true,
                                    wordWrap = true,
                                    text = "Continue",
                                    location = { 0, 0, 156, 20 },
                                    color = { 150, 150, 150, 255 }
                                }
                            }
                        },
                        {
                            name = "Option Button",
                            type = "button",
                            location = { 0, 0, 156, 20 },
                            pressedFunc = optionGameButtonClickFunc,
                            hoverFunc = buttonHoverFunc,
                            children = {
                                {
                                    name = "Option",
                                    type = "text",
                                    centeredX = true,
                                    centeredY = true,
                                    wordWrap = true,
                                    text = "Option",
                                    location = { 0, 0, 156, 20 },
                                    color = { 150, 150, 150, 255 }
                                }
                            }
                        }
                    }
                },
                {
                    name = "Selections Finger",
                    type = "image",
                    location = { 40, 205, 16, 16 },
                    imageName = "fingers-export",
                    srcRect = { 16, 32, 16, 16 }

                }

            }

        }
    }
}

return returnTable
