local dialog = {}
local ui = require("UI")
local engine = require("Engine")
local numLettersStr = "NumLetters"
local maxLettersStr = "MaxLetters"
local currentTimeOnLetterStr = "CurrentTimeOnLetter"
dialog.DialogBoxes = {}

dialog.LetterTime = 0.05

function dialog.DialogInteractionStart(playerNum, text)
    local box = dialog.DialogBoxes[playerNum]
    -- ui.UpdateText(box, text)
    ui.UpdateText(box.ptr, text)
    ui.UpdateNumLettersForText(box.ptr, 0)
    box[numLettersStr] = 0
    box[currentTimeOnLetterStr] = 0.0
    box[maxLettersStr] = #text
    engine.Audio.PlayBGMBackground("typing", 1)
    box.isTyping = true
    ui.SetObjectVisible(box.boxptr, true)
end

---Update the text interaction
---@param playerNum integer The player number that is interacting
---@return boolean if the interaction is complete.
function dialog.DialogInteractionUpdate(playerNum)
    local box = dialog.DialogBoxes[playerNum]
    if box[numLettersStr] == box[maxLettersStr] then
        ui.SetObjectVisible(box.boxptr, false)
        return true
    else
        box[numLettersStr] = box[maxLettersStr]
        ui.UpdateNumLettersForText(box.ptr, box[numLettersStr])
        return false
    end
end

---Updates all dialog boxes
function dialog.UpdateDialogBoxes(timeSeconds)
    for _, box in ipairs(dialog.DialogBoxes) do
        if not box.isTyping or box[numLettersStr] == nil or box[maxLettersStr] == nil then goto fin end
        box[currentTimeOnLetterStr] = box[currentTimeOnLetterStr] + timeSeconds
        if box[currentTimeOnLetterStr] >= dialog.LetterTime then
            box[currentTimeOnLetterStr] = 0.0
            box[numLettersStr] = math.min(box[numLettersStr] + 1, box[maxLettersStr])
            ui.UpdateNumLettersForText(box.ptr, box[numLettersStr])
            if box[numLettersStr] == box[maxLettersStr] then
                engine.Audio.StopBGMBackground()
                box.isTyping = false
            end
        end
        ::fin::
    end
end

return dialog
