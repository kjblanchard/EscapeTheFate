local function doThingLocal()

end
local returnTable = {
    name = "BattleUI",
    startFunc = doThingLocal,
    children = {
        {
            name = "MainUIBox",
            type = "9slice",
            location = { 140, 200, 260, 60 },
            color = { 80, 0, 120, 235 },
            imageName = "uibase-export",
            alpha = 190,
            children = {
                {
                    name = "TextBoxText",
                    type = "text",
                    font = "PressStart2P",
                    size = 8,
                    wordWrap = true,
                    centeredX = true,
                    centeredY = true,
                    text = "Hello world",
                    location = { 8, 8, 169, 59 }
                },

            }
        },
    }
}
return returnTable
