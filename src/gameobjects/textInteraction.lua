local engine = require("Engine")
local textInteraction ={}

local textInteractions = {}

function textInteraction.Start(data)
    local newInteraction = {
        rect = {
            x = data.x,
            y = data.y,
            w = data.width,
            h = data.height,
        },
        text = require(string.format("data.dialog.%s",engine.MapName()))[data.properties.filename]
    }
    table.insert(textInteractions, newInteraction)
end

---@param rect table
function textInteraction.CheckForTextInteractions(rect)
    for i = 1, #textInteractions, 1 do
        local currentInteraction = textInteractions[i]
        if engine.Collision.CheckForCollision(rect, currentInteraction.rect) then
            return currentInteraction.text
        end
    end
    return nil
end


return textInteraction
