local battlerLocation = {}

local battlerLocations = {}

function battlerLocation.Start(data)
    local newlocation = {
        x = data.x,
        y = data.y,
        id = data.id
    }
    table.insert(battlerLocations, newlocation)
end

---@param id integer
function battlerLocation.GetBattlerLocation(id)
    for i = 1, #battlerLocations, 1 do
        local currentLocation = battlerLocations[i]
        if id == currentLocation.id then
            return currentLocation.x, currentLocation.y
        end
    end
    return nil
end

function battlerLocation.End()
    battlerLocations = {}
end

return battlerLocation
