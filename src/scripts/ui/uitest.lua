local uitest = {
    children = {
        textTest = {
            class = "text",
            font = "PressStart2P",
            fontSize = 8,
            rect = {5,0,500,500},
            text = "Hello World!",
            children = {
                text2 = {
                    class = "text",
                    font = "PressStart2P",
                    fontSize = 8,
                    rect = {5,20,500,500},
                    text = "Hello UNIVERSE",
                    children = {}
                }

            }
        },
    }
}

return uitest
