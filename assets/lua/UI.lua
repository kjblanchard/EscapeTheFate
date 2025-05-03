-- UI.lua
local UI = {}
local font = ""
local fontSize = 0
local function normalizeArrayTableWithKeys(rect, keys)
    if rect and #rect == #keys and rect[keys[1]] == nil then
        local normalizedRect = {}
        for i = 1, #keys, 1 do
            normalizedRect[keys[i]] = rect[i]
        end
        return normalizedRect
    end
    return rect
end

local function normalizeRect(rect)
    return normalizeArrayTableWithKeys(rect, { "x", "y", "w", "h" })
end

local function normalizeColorRect(rect)
    return normalizeArrayTableWithKeys(rect, { "r", "g", "b", "a" })
end

local function CreatePanel(name, rect, parentPanel)
    rect = normalizeRect(rect)
    return cUI.CreatePanel(name, rect, parentPanel)
end

local function CreateImage(name, rect, parentPanel, filename, srcRect)
    rect = normalizeRect(rect)
    srcRect = normalizeRect(srcRect)
    return cUI.CreateImage(name, rect, parentPanel, filename, srcRect)
end

local function CreateText(name, rect, parentPanel, text, fontName, fontSize, centerX, centerY, wordwrap, color)
    rect = normalizeRect(rect)
    color = normalizeColorRect(color)
    return cUI.CreateText(name, rect, parentPanel, text, fontName, fontSize, centerX, centerY, wordwrap, color)
end

local function CreateRect(name, rect, parentPanel, color)
    rect = normalizeRect(rect)
    color = normalizeColorRect(color)
    return cUI.CreateRect(name, rect, parentPanel, color)
end

local function CreateHLG(name, rect, parentPanel, spacing)
    rect = normalizeRect(rect)
    return cUI.CreateLayoutGroup(name, rect, parentPanel, spacing, true)
end

local function CreateVLG(name, rect, parentPanel, spacing)
    rect = normalizeRect(rect)
    return cUI.CreateLayoutGroup(name, rect, parentPanel, spacing, false)
end

local function CreateUIObjectAndChildren(objTable, parent)
    -- TODO we should validate these so it doesn't break
    local obj = nil
    if objTable.type == "image" then
        obj = CreateImage(objTable.name, objTable.location, parent, objTable.imageName, objTable.srcRect)
    elseif objTable.type == "text" then
        local thisFont = font
        local thisSize = fontSize
        if objTable.font ~= nil then
            thisFont = objTable.font
            font = thisFont
        end
        if objTable.centeredX == nil then objTable.centeredX = false end
        if objTable.centeredY == nil then objTable.centeredY = false end
        if objTable.wordWrap == nil then objTable.wordWrap = false end
        if objTable.color == nil then objTable.color = { 255, 255, 255, 255 } end
        if objTable.size ~= nil then
            thisSize = objTable.size
            fontSize = thisSize
        end
        obj = CreateText(objTable.name, objTable.location, parent, objTable.text, thisFont, thisSize,
            objTable.centeredX, objTable.centeredY, objTable.wordWrap, objTable.color)
    elseif objTable.type == "rect" then
        obj = CreateRect(objTable.name, objTable.location, parent, objTable.color)
    elseif objTable.type == "hlg" then
        obj = CreateHLG(objTable.name, objTable.location, parent, objTable.spacing)
    elseif objTable.type == "vlg" then
        obj = CreateVLG(objTable.name, objTable.location, parent, objTable.spacing)
    elseif objTable.type == "panel" then
    end
    if objTable.children then
        for _, child in ipairs(objTable.children) do
            CreateUIObjectAndChildren(child, obj)
        end
    end
end

function UI.CreatePanelFromTable(table)
    -- Top level is always a panel
    local panel = CreatePanel(table.name, { 0, 0, 0, 0 }, nil)
    for _, child in ipairs(table.children) do
        CreateUIObjectAndChildren(child, panel)
    end
end

return UI
