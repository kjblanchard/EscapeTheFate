-- Top level is always a panel, it just needs a name and then you can put all your children inside of it
return {
    name = "Test Panel",
    children = {
        {
            name = "Title BG",
            type = "rect",
            location = { 125, 10, 278, 75 },
            color = { 0, 0, 0, 132 }
        },
        {
            name = "Title",
            type = "text",
            font = "PressStart2P",
            size = 32,
            wordWrap = true,
            centeredX = true,
            text = "Escape The Fate",
            location = { 125, 10, 278, 75 }
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
            location = { 48, 198, 156, 70 },
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
                    location = { 0, 0, 156, 20 }
                },
                {
                    name = "Option",
                    type = "text",
                    centeredX = true,
                    centeredY = true,
                    wordWrap = true,
                    text = "Option",
                    location = { 0, 0, 156, 20 }
                }
            }
        }
    }
}
