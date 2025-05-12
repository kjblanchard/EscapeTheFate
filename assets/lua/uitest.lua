local engine = require("Engine")
-- Top level is always a panel, it just needs a name and then you can put all your children inside of it
local function buttonFunc()
    engine.LogWarn("Button pressed!")
end

local function hoverFunc()
    engine.LogWarn("Button hovered!")
end

table = {
    name = "Test Panel",
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
            name = "Selections BG",
            type = "rect",
            location = { 48, 198, 156, 70 },
            color = { 0, 0, 0, 190 }
        },
        {
            name = "Selections",
            type = "vlg",
            location = { 48, 203, 156, 70 },
            spacing = 20,
            children = {
                {
                    name = "New Game Button",
                    type = "button",
                    pressedFunc = buttonFunc,
                    hoverFunc = hoverFunc,
                    children = {
                        name = "New Game",
                        type = "text",
                        size = 16,
                        centeredX = true,
                        centeredY = true,
                        wordWrap = true,
                        text = "New Game",
                        location = { 0, 0, 156, 20 }
                    }


                },
                -- {
                --     name = "New Game",
                --     type = "text",
                --     size = 16,
                --     centeredX = true,
                --     centeredY = true,
                --     wordWrap = true,
                --     text = "New Game",
                --     location = { 0, 0, 156, 20 }

                -- },
                {
                    name = "Continue",
                    type = "text",
                    centeredX = true,
                    centeredY = true,
                    wordWrap = true,
                    text = "Continue",
                    location = { 0, 0, 156, 20 },
                    color = { 150, 150, 150, 255 }
                },
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
}

return table
