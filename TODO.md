# Escape The Fate

Cpp game?

## In Progress


## Todos

### UI

- When should we destroy the top level panels if they are NOT marked donotdestroy
: we should do it on every load
- Add in a way for donotdestroy on uiobjects so they can persist
- More fields that can be optional to make it easier to write the jsonc file

### Bugs

- ASAN issue when trying to load a bgm that doesn't exist on scene change

## Backlogged

## Completed

- Always creates the ui, need to actually load based on the scene loader
- Do not create the UI if it already exists
- Should we cache or always read the file?
: we cache on every load, and right now we don't ever clear it

