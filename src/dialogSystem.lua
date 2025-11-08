local ui = require("UI")
local interaction = require("gameobjects.textInteraction")
local dialogSystem = {}

local textBoxObject = nil
local loadedTextPtr = nil
local currentTextInteractionObject = nil

function dialogSystem.InitializeDialogSystem(textBoxObj)
    textBoxObject = textBoxObj
    loadedTextPtr = textBoxObj.children.textTest.ptr
end

function dialogSystem.CheckForTextInteractions(rect)
    currentTextInteractionObject = interaction.CheckForTextInteractions(rect)
    return currentTextInteractionObject ~= nil

end

function dialogSystem.Start()
    if not loadedTextPtr or not currentTextInteractionObject then return end
    ui.UpdateTextText(loadedTextPtr,currentTextInteractionObject)
    textBoxObject.visible = true
end

function dialogSystem.DisableDialogBox()
    textBoxObject.visible = false

end

return dialogSystem
