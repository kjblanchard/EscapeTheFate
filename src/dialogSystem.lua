local ui = require("UI")
local interaction = require("gameobjects.textInteraction")
local engine = require("Engine")
local gamestate = require("gameState")
local dialogSystem = {}

local textBoxObject = nil
local loadedTextPtr = nil
local currentTextInteractionText = nil
local displayingText = false
local currentDisplayedLetters = 0
local displayTimePerLetter = 0.05
local textDisplayEnded = false
local currentDisplayingCo = nil
local textBoxObjEndYPos = 0
local currentTweenTime = 0

local function createDialogCorouting()
    return coroutine.create(function()
        while textBoxObject and textBoxObject.rect.y ~= textBoxObjEndYPos do
            currentTweenTime = currentTweenTime + gamestate.DeltaTimeSeconds
            textBoxObject.rect.y = engine.Tween(textBoxObjEndYPos - 100, textBoxObjEndYPos, currentTweenTime, 0.15)
            coroutine.yield()
        end

        while displayingText and not textDisplayEnded and currentTextInteractionText and currentDisplayedLetters < #currentTextInteractionText do
            engine.Coroutine.Wait(displayTimePerLetter)
            currentDisplayedLetters = currentDisplayedLetters + 1
            engine.Text.SetTextNumLetters(loadedTextPtr, currentDisplayedLetters)
            if currentDisplayedLetters == #currentTextInteractionText then textDisplayEnded = true end
        end
        currentDisplayingCo = nil
    end)
end



function dialogSystem.InitializeDialogSystem(textBoxObj)
    textBoxObject = textBoxObj
    loadedTextPtr = textBoxObj.children.textTest.ptr
    textBoxObjEndYPos = textBoxObj.rect.y
end

function dialogSystem.CheckForTextInteractions(rect)
    currentTextInteractionText = interaction.CheckForTextInteractions(rect)
    return currentTextInteractionText ~= nil
end

function dialogSystem.Start()
    if not loadedTextPtr or not currentTextInteractionText or not textBoxObject then return end
    ui.UpdateTextText(loadedTextPtr, currentTextInteractionText)
    displayingText = true
    currentDisplayedLetters = 0
    textDisplayEnded = false
    currentTweenTime = 0
    textBoxObject.rect.y = textBoxObjEndYPos - 100
    engine.Text.SetTextNumLetters(loadedTextPtr, currentDisplayedLetters)
    currentDisplayingCo = createDialogCorouting()
    engine.Coroutine.run(currentDisplayingCo)
    textBoxObject.visible = true
end

function dialogSystem.ProgressDialog()
    if textDisplayEnded then
        dialogSystem.DisableDialogBox()
        return true
    end
    currentDisplayedLetters = #currentTextInteractionText
    engine.Text.SetTextNumLetters(loadedTextPtr, currentDisplayedLetters)
    textDisplayEnded = true
    displayingText = false
    return false
end

function dialogSystem.DisableDialogBox()
    textBoxObject.visible = false
    displayingText = false
end

return dialogSystem
