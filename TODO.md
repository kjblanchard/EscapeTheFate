# Escape The Fate

Cpp game?

## In Progress

- draw out target selection

## Todos

- target selection player battler

### Battle

- python script for changing color schemes based on time of day
- check on discord bot updates.
- ui animation for player speed
- add enemy health bar and player health bar

### UI

### Bugs

- When loading map doesn't exist, gets a segfault
- ASAN issue when trying to load a bgm that doesn't exist on scene change

## Backlogged

- More fields that can be optional to make it easier to write the jsonc file
    : not really needed right now
- Dialog system slidefade anim .. would be cool if we could handle tweening the opacity
- Better error handling / loading from json
- Use gameconfig to handle the dialog box variables, instead of const

## Refactors

- Animation system should use regular pointers and needs an overhaul, why use handles
    : not needed right now, and works fine.
- remove gameobject system from the C engine, only makes it annoying to work with

## Completed

- player speed
- ui progress bar
- update progress bar with data
- redo some of the UI update stuff, need to draw this out.
- Make battlerUI, so that battler can updateUI easily
- update player and enemy real health from data
- display enemy health number
- battler offset in battlerData
- spawn enemies
- move selection and fingers around
- vlg is needed to help organize.
- load battlers
- battle UI
- enter battle
- Handle different tweening types in engine.cpp
    : right now hardcoded to quint
- When do we clear dialog box cache?
    : never
- Dialog system needs to be completed
    : Handle multiple strings in a dialog
    : typing sound for background
    : Use objects instead of the json
    : Dialog Objects need to reference the strings somehow
    : When interacting, we should update the text properly
    : Dialog box doesn't clear properly sometimes
    : Dialog should open when interacted, and progress properly.
    : Animation
- load png instead of bmp please.
- Always creates the ui, need to actually load based on the scene loader
- Do not create the UI if it already exists
- Should we cache or always read the file?
    : we cache on every load, and right now we don't ever clear it
- When should we destroy the top level panels if they are NOT marked donotdestroy
    : we should do it on every load
    : but .. shouldn't we cache them?  I guess not, lets clean them.
- Add in a way for donotdestroy on uiobjects so they can persist
    : it's a var, no way to do it yet but not needed atm
    : but we probably should, cause the dialog box should be stored always?
- Move to jsonc instead of what we use now. .. Lua and glaze.

## Version Roadmap

- 0.1.0: basic dialog with interactions, enter battle, can kill enemy, battle ended.
    : dialog
    : battle enter
    : battle finish
    : battle end
    : fix all leaks
    : go through all todos and update

- 0.2.0: local multiplayer
    : second player can spawn
    : camera follows main p1
    : second player can switch screens and interact with things
    : second battler

- 0.3.0: Main menu and splash screens
    : add in splash screens when loading the game
    : add in a welcome menu, where you can new game, options, etc
    : open a menu when exploring, should be small enough for both
    : simple viewing of stats

- 0.4.0: Battle enhancements
    : inventory that you can have (shared)
    : equipment that can be used
    : equip and change stats with the equip
    : saving and loading
