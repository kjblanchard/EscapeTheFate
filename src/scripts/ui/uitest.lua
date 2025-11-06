local uitest = {
    children = {
        niner = {
            class = "nineSlice",
            color = { 80, 0, 120, 235 },
            rect = { 60, 60, 156, 70 },
            xSize = 8,
            ySize = 9,
            filename = "uibase-export",
            children = {
                textTest = {
                    class = "text",
                    font = "PressStart2P",
                    fontSize = 8,
                    rect = {5,5,500,500},
                    text = "Hello World!",
                }
            }
        },
    },
}
return uitest
