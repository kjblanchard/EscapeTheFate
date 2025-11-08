local ui = require("UI")
local engine = require("Engine")
local dialogSystem = {}

local textBoxObject = nil
local loadedTextPtr = nil

function dialogSystem.InitializeDialogSystem(textBoxObj)
    textBoxObject = textBoxObj
    loadedTextPtr = textBoxObj.children.textTest.ptr
end

function dialogSystem.LoadTextToDialogBox(text)
    if not loadedTextPtr then return end
    ui.UpdateTextText(loadedTextPtr,text)
    textBoxObject.visible = true
end

function dialogSystem.DisableDialogBox()
    textBoxObject.visible = false
    
end

return dialogSystem
