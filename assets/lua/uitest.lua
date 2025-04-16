-- Top level is always a panel, it just needs a name and then you can put all your children inside of it
return {
    name = "Test Panel",
    children = {
        {
            name = "Test Image",
            type = "image",
            imageName = "uibase-export",
            location = { 64, 64, 64, 64 }
        },
        {
            name = "Test Text",
            type = "text",
            font = "PressStart2P",
            size = 16,
            centeredX = true,
            centeredY = true,
            wordWrap = true,
            text = "Welcome to the game!  Hello world!",
            location = { 48, 48, 260, 120 }
        },
        {
            name = "Test Rect",
            type = "rect",
            location = { 48, 48, 260, 120 },
            color = { 0, 0, 0, 0 }
        },
        {
            name = "Test HLG",
            type = "hlg",
            location = { 48, 48, 260, 120 },
            spacing = 5,
            children = {
                {
                    name = "New Game",
                    type = "text",
                    centeredX = true,
                    centeredY = true,
                    wordWrap = true,
                    text = "New Game",
                    location = { 0, 0, 128, 64 }
                },
                {
                    name = "Continue",
                    type = "text",
                    centeredX = true,
                    centeredY = true,
                    wordWrap = true,
                    text = "Continue",
                    location = { 0, 0, 128, 64 }
                },
                {
                    name = "Option",
                    type = "text",
                    centeredX = true,
                    centeredY = true,
                    wordWrap = true,
                    text = "Option",
                    location = { 0, 0, 128, 64 }
                }
            }
        }
    }
}
