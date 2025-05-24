local engine = require("Engine")
local gameobject = {}
gameobject.Sprites = {}

function gameobject.Position(ptr)
    return cGameObject.Position(ptr)
end

function gameobject.SetPosition(ptr, x, y)
    return cGameObject.SetPosition(ptr, x, y)
end

function gameobject.Size(ptr)
    return cGameObject.Size(ptr)
end

function gameobject.Sprite(ptr)
    return gameobject.Sprites[ptr]
end

return gameobject
