-- Top level is always a panel, it just needs a name and then you can put all your children inside of it
return {
    name = "Test Panel",
    children = {
        {
            name = "Title BG",
            type = "rect",
            location = { 68, 10, 394, 75 },
            color = { 0, 0, 0, 132 }
        },
        {
            name = "Title",
            type = "text",
            font = "PressStart2P",
            size = 32,
            wordWrap = true,
            centeredX = true,
            centeredY = true,
            text = "Escape The Fate",
            location = { 69, 10, 394, 75 }
        },
        {
            name = "Selections BG",
            type = "rect",
            location = { 48, 198, 156, 70 },
            color = { 0, 0, 0, 132 }
        },
        {
            name = "Selections",
            type = "vlg",
            location = { 48, 203, 156, 70 },
            spacing = 20,
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
                },
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
