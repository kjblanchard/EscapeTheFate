# Escape The Fate

Cpp game?

## Version Roadmap

- 0.1.0: basic dialog with interactions, enter battle, can kill enemy, battle ended.

## In Progress

- Dialog system needs to be completed
    : Dialog should open when interacted, and progress properly.
    : Handle multiple strings in a dialog
- Dialog box doesn't clear properly sometimes
- Better error handling / loading from json of these things

## Todos

### UI

- When do we clear dialog box cache?

### Bugs

- When loading map doesn't exist, gets a segfault
- ASAN issue when trying to load a bgm that doesn't exist on scene change

## Backlogged

- More fields that can be optional to make it easier to write the jsonc file
    : not really needed right now
- Animation system should use regular pointers and needs an overhaul, why use handles
    : not needed right now, and works fine.

## Completed

- Dialog system needs to be completed
    : Dialog Objects need to reference the strings somehow
    : When interacting, we should update the text properly
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
