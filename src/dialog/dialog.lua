local dialog = {}
local ui = require("UI")
local numLettersStr = "NumLetters"
local maxLettersStr = "MaxLetters"
local currentTimeOnLetterStr = "CurrentTimeOnLetter"
dialog.DialogBoxes = {}

dialog.LetterTime = 0.05

function dialog.DialogInteraction(playerNum, text)
    local box = dialog.DialogBoxes[playerNum]
    -- ui.UpdateText(box, text)
    ui.UpdateText(box.ptr, text)
    ui.UpdateNumLettersForText(box.ptr, 0)
    box[numLettersStr] = 0
    box[currentTimeOnLetterStr] = 0.0
    box[maxLettersStr] = #text
end

---Updates all dialog boxes
function dialog.UpdateDialogBoxes(timeSeconds)
    for _, box in ipairs(dialog.DialogBoxes) do
        if box[numLettersStr] == nil or box[maxLettersStr] == nil then goto fin end
        box[currentTimeOnLetterStr] = box[currentTimeOnLetterStr] + timeSeconds
        if box[currentTimeOnLetterStr] >= dialog.LetterTime then
            box[currentTimeOnLetterStr] = 0.0
            box[numLettersStr] = math.min(box[numLettersStr] + 1, box[maxLettersStr])
            if box[numLettersStr] <= box[maxLettersStr] then
                ui.UpdateNumLettersForText(box.ptr, box[numLettersStr])
            end
        end
        ::fin::
    end
end

return dialog
