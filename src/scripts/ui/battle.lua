local function doThingLocal()
end

local returnTable = {
    name = "BattleUI",
    startFunc = doThingLocal,
    children = {
        {
            name = "MainUIBox",
            type = "9slice",
            location = { 129, 208, 260, 60 },
            color = { 80, 0, 120, 235 },
            imageName = "uibase-export",
            alpha = 190,
            children = {
                {
                    name = "SelectionsVLG",
                    type = "vlg",
                    location = { 0, 0, 260, 60 },
                    spacing = 10,
                    children = {
                        {
                            name = "Player1Panel",
                            type = "panel",
                            location = { 0, 4, 260, 18 },
                            children = {
                                {
                                    name = "Kevin",
                                    type = "text",
                                    font = "PressStart2P",
                                    size = 8,
                                    centeredY = true,
                                    wordWrap = true,
                                    text = "Kevin",
                                    location = { 0, 0, 70, 15 }
                                },
                                {
                                    name = "Player1HLG",
                                    type = "hlg",
                                    location = { 0, 0, 156, 20 },
                                    spacing = 10,
                                    children = {
                                        {
                                            name = "Player1HPPanel",
                                            type = "panel",
                                            location = { 0, 0, 156, 20 },
                                            children = {
                                                {
                                                    name = "HPText",
                                                    type = "text",
                                                    size = 8,
                                                    centeredX = true,
                                                    centeredY = true,
                                                    wordWrap = true,
                                                    text = "HP",
                                                    location = { 0, 0, 156, 20 }
                                                },
                                                {
                                                    name = "HPCurrentText",
                                                    type = "text",
                                                    size = 8,
                                                    centeredX = true,
                                                    centeredY = true,
                                                    wordWrap = true,
                                                    text = "50",
                                                    location = { 5, 0, 156, 20 }
                                                },
                                                {
                                                    name = "HPColon",
                                                    type = "text",
                                                    size = 8,
                                                    centeredX = true,
                                                    centeredY = true,
                                                    wordWrap = true,
                                                    text = ":",
                                                    location = { 9, 0, 156, 20 }
                                                },
                                                {
                                                    name = "HPMaxText",
                                                    type = "text",
                                                    size = 8,
                                                    centeredX = true,
                                                    centeredY = true,
                                                    wordWrap = true,
                                                    text = "50",
                                                    location = { 15, 0, 156, 20 }
                                                },
                                            }
                                        },
                                    }
                                }
                            }
                        },
                    }
                }
            },
        },
    }
}
return returnTable
