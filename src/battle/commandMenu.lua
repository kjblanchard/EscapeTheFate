local commandMenu = {}
local ui = require("UI")
local _fingerUIObject = nil
local _commandUIObjectTable = nil
local _currentFingerPos = 1
local _commandMenuUIObject = nil
local engine = require("Engine")
local fingerXOffset = -15

local function moveFingerToPosition()
    if not _fingerUIObject or not _commandUIObjectTable then return end
    local newX, newY = ui.GetWorldXYForObject(_commandUIObjectTable[_currentFingerPos])
    _fingerUIObject.rect.x = newX + fingerXOffset
    _fingerUIObject.rect.y = newY
end

local function handleInput()
    if engine.Input.KeyboardKeyJustPressed(engine.Input.Buttons.UP) then
        _currentFingerPos = _currentFingerPos - 1
        if _currentFingerPos < 1 then _currentFingerPos = #_commandUIObjectTable end
    end
    if engine.Input.KeyboardKeyJustPressed(engine.Input.Buttons.DOWN) then
        _currentFingerPos = _currentFingerPos + 1
        if _currentFingerPos > #_commandUIObjectTable then _currentFingerPos = 1 end
    end
    moveFingerToPosition()
end

--commands table is attack,magic,skill,items
function commandMenu.Initialize(commandMenuUIObject, fingerUIObject, commandsUIObjectTable)
    _commandMenuUIObject = commandMenuUIObject
    _fingerUIObject = fingerUIObject
    _commandUIObjectTable = commandsUIObjectTable
    if _commandMenuUIObject then
        _commandMenuUIObject.visible = commandsUIObjectTable
        -- _commandMenuUIObject.visible = false
    end
    if _fingerUIObject then
        -- _fingerUIObject.visible = false
        moveFingerToPosition()
    end
end

function commandMenu.Update()
    handleInput()
end

return commandMenu
