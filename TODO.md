# Escape The Fate

Cpp game?

## In Progress

## Todos

- Move to jsonc instead of what we use now. .. Lua and glaze.

### UI

### Bugs

- When loading map doesn't exist, gets a segfault
- ASAN issue when trying to load a bgm that doesn't exist on scene change

## Backlogged

- More fields that can be optional to make it easier to write the jsonc file
    : not really needed right now

## Completed

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

