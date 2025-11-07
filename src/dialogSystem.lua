local ui = require("UI")
local engine = require("Engine")
local dialogSystem = {}

local loadedTextPtr = nil

function dialogSystem.InitializeDialogSystem(textPtr)
    loadedTextPtr = textPtr
end

function dialogSystem.LoadTextToDialogBox(text)
    if not loadedTextPtr then return end
    ui.UpdateTextText(loadedTextPtr,text)
end

return dialogSystem
