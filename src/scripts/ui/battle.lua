local ui = require("UI")
local engine = require("Engine")
local battle = require("gameobjects.battle")
local battleFingerPtr = nil
-- local attackButtonPtr = nil
local _currentButtonHovered = 0
local function doThingLocal()
    -- The C progressBar ptr that you can call functions against.
    local atbBars = {}
    battle.battleUI.ATBBars = atbBars
    atbBars.player1 = {}
    atbBars.player1.progressBar = ui.lookup
        ["BattleUI.MainUIBox.SelectionsVLG.Player1Panel.Player1HLG.ATBBarAnimImage.ProgressBar"].data
    atbBars.player1.progressBarAnim = ui.lookup
        ["BattleUI.MainUIBox.SelectionsVLG.Player1Panel.Player1HLG.ATBBarAnimImage"].data
    battleFingerPtr = ui.lookup["BattleUI.Selections Finger"].data
    -- attackButtonPtr = ui.lookup["BattleUI.P1CommandsBox.AttackButton"].data
end

local function attackButtonPressed(uiObjPtr, justClicked)
    if not justClicked then return end
    battle.PlayerAttackButtonPressed()
end

local function skillsButtonPressed(_, justClicked)
    if not justClicked then return end
    battle.PlayerSkillsButtonPressed()
end

local function itemsButtonPressed(_, justClicked)
    if not justClicked then return end
    battle.PlayerItemsButtonPressed()
end

local function magicButtonPressed(_, justClicked)
    if not justClicked then return end
    battle.PlayerMagicButtonPressed()
end

local function buttonNotImplemented(uiObjPtr, justClicked)
end

local function buttonHoveredFunc(uiObjPtr, isJustHovered)
    if _currentButtonHovered == uiObjPtr or isJustHovered == false then return end
    local hoverdLocationX, hoveredLocationY = ui.GetObjectLocation(uiObjPtr)
    hoverdLocationX                         = hoverdLocationX + 5   --offset from words
    hoveredLocationY                        = hoveredLocationY - 20 --offset from words
    ui.SetObjectLocation(battleFingerPtr, hoverdLocationX, hoveredLocationY)
    engine.Audio.PlaySfxOneShot("menuMove", 1.0)
    _currentButtonHovered = uiObjPtr
end

local returnTable = {
    name = "BattleUI",
    startFunc = doThingLocal,
    children = {
        {
            name = "MainUIBox",
            type = "9slice",
            location = { 120, 225, 265, 45 },
            color = { 80, 0, 120, 235 },
            imageName = "uibase-export",
            alpha = 190,
            children = {
                {
                    name = "SelectionsVLG",
                    type = "vlg",
                    location = { 0, 0, 260, 60 },
                    spacing = 10,
                    children = {
                        {
                            name = "Player1Panel",
                            type = "panel",
                            location = { 0, 4, 260, 18 },
                            children = {
                                {
                                    name = "Kevin",
                                    type = "text",
                                    font = "PressStart2P",
                                    size = 8,
                                    text = "Kevin",
                                    location = { 8, 4, 70, 15 }
                                },
                                {
                                    name = "Player1HLG",
                                    type = "hlg",
                                    location = { 45, 4, 156, 20 },
                                    spacing = 77,
                                    children = {
                                        {
                                            name = "Player1HPPanel",
                                            type = "panel",
                                            location = { 0, 0, 156, 20 },
                                            children = {
                                                {
                                                    name = "HPText",
                                                    type = "text",
                                                    size = 8,
                                                    text = "HP",
                                                    location = { 20, 0, 156, 20 }
                                                },
                                                {
                                                    name = "HPCurrentText",
                                                    type = "text",
                                                    size = 8,
                                                    text = "50",
                                                    location = { 40, 0, 156, 20 }
                                                },
                                                {
                                                    name = "HPColon",
                                                    type = "text",
                                                    size = 8,
                                                    text = "/",
                                                    location = { 60, 0, 156, 20 }
                                                },
                                                {
                                                    name = "HPMaxText",
                                                    type = "text",
                                                    size = 8,
                                                    text = "50",
                                                    location = { 70, 0, 156, 20 }
                                                },
                                            }
                                        },
                                        {
                                            name = "Player1MPPanel",
                                            type = "panel",
                                            location = { 0, 0, 156, 20 },
                                            children = {
                                                {
                                                    name = "MPText",
                                                    type = "text",
                                                    size = 8,
                                                    text = "MP",
                                                    location = { 20, 0, 156, 20 }
                                                },
                                                {
                                                    name = "MPCurrentText",
                                                    type = "text",
                                                    size = 8,
                                                    text = "10",
                                                    location = { 40, 0, 156, 20 }
                                                },
                                                {
                                                    name = "MPColon",
                                                    type = "text",
                                                    size = 8,
                                                    text = "/",
                                                    location = { 60, 0, 156, 20 }
                                                },
                                                {
                                                    name = "MPMaxText",
                                                    type = "text",
                                                    size = 8,
                                                    text = "10",
                                                    location = { 70, 0, 156, 20 }
                                                },
                                            }
                                        },
                                        {
                                            name = "ATBBarAnimImage",
                                            type = "imageAnimator",
                                            location = { 25, -11, 32, 32 },
                                            imageName = "atbBar",
                                            srcRect = { 0, 0, 16, 16 },
                                            defaultAnimation = "idle",
                                            children = {
                                                {

                                                    name = "ProgressBar",
                                                    type = "progressBar",
                                                    location = { 4, 12, 24, 6 },
                                                    color = { 0, 255, 0, 255 },
                                                }
                                            }
                                        },
                                    }
                                }
                            }
                        },

                    }
                }
            },
        },
        {
            name = "P1CommandsBox",
            type = "9slice",
            location = { 360, 138, 118, 45 },
            color = { 80, 0, 120, 235 },
            imageName = "uibase-export",
            alpha = 190,
            children = {
                {
                    name = "AttackButton",
                    type = "button",
                    pressOnRelease = true,
                    pressedFunc = attackButtonPressed,
                    hoverFunc = buttonHoveredFunc,
                    location = { 5, 10, 52, 10 },
                    children = {
                        {
                            name = "AttackText",
                            type = "text",
                            size = 8,
                            text = "Attack",
                            location = { 3, 0, 52, 10 }
                        },

                    }
                },
                {
                    name = "MagicButton",
                    type = "button",
                    size = 8,
                    pressOnRelease = true,
                    pressedFunc = magicButtonPressed,
                    hoverFunc = buttonHoveredFunc,
                    location = { 66, 10, 45, 10 },
                    children = {
                        {
                            name = "MagicText",
                            type = "text",
                            size = 8,
                            text = "Magic",
                            color = { 110, 110, 110, 255 },
                            location = { 0, 0, 45, 10 }
                        },

                    }
                },
                {
                    name = "SkillButton",
                    type = "button",
                    pressOnRelease = true,
                    pressedFunc = skillsButtonPressed,
                    hoverFunc = buttonHoveredFunc,
                    location = { 8, 28, 45, 10 },
                    children = {
                        {
                            name = "SkillText",
                            type = "text",
                            size = 8,
                            text = "Skills",
                            color = { 110, 110, 110, 255 },
                            location = { 0, 0, 45, 10 }

                        }

                    }
                },
                {
                    name = "ItemButton",
                    type = "button",
                    pressOnRelease = true,
                    pressedFunc = itemsButtonPressed,
                    hoverFunc = buttonHoveredFunc,
                    location = { 66, 28, 50, 10 },
                    children = {
                        {
                            name = "ItemText",
                            type = "text",
                            size = 8,
                            text = "Items",
                            color = { 110, 110, 110, 255 },
                            location = { 0, 0, 50, 10 }
                        },
                    }

                }

            }
        },
        {
            name = "Selections Finger",
            type = "image",
            location = { 435, 130, 16, 16 },
            imageName = "fingers-export",
            srcRect = { 48, 32, 16, 16 }

        },
        {
            name = "EnemySelectFinger",
            type = "image",
            visible = false,
            location = { 0, 0, 16, 16 },
            imageName = "fingers-export",
            srcRect = { 48, 32, 16, 16 }

        },
        {
            name = "EnemyBattlerButtons",
            type = "panel",
            location = { 0, 0, 16, 16 },
            children = {}

        },
    }
}
return returnTable
