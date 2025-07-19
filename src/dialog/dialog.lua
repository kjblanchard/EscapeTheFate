local dialog = {}
local ui = require("UI")
dialog.DialogBoxes = {}

function dialog.DialogInteraction(playerNum, text)
    local box = dialog.DialogBoxes[playerNum]
    ui.UpdateText(box, text)
end

return dialog
