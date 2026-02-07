# Escape The Fate

## In Progress

## Todos

### Battle

### UI

### Bugs

- likely does not preload the tiled things cause of the way they are named

## Current Release

## Backlogged

- BGM fadeout and fadein
- add enemy health bar and player health bar
- check on discord bot updates.
- More fields that can be optional to make it easier to write the jsonc file
  - not really needed right now
- Dialog system slidefade anim .. would be cool if we could handle tweening the opacity
- Better error handling / loading from json
- Use gameconfig to handle the dialog box variables, instead of const

## Completed

- Fix bugs
- When fadetime is 0, it skips both.
- Issue with fadein time when loading takes too long.
- battle in debughome sends you offmap
    : happens if you dont move on a new map, probably fine for now.
- restart stream when you click it before it's loaded
- leaks
- ASAN issue when trying to load a bgm that doesn't exist on scene change
- When loading map doesn't exist, gets a segfault
- Errors.log has (null) in front of it, lol.
- House dialog / dialog when none exists
- Jitter in middle of screen
- Web build color on ATB bar is non-existant
- Web build - moving screens during a fade causes fade to not happen, shouldn't be able to exit until faded.
- Emscripten
  - Web build does not load on a black screen, not sure why that is
    - trying to fix logic for loading tor only happen on fade
  - Web build sometimes does not fade fully, should set fade at end back to full.

- Jitter, game is not smooth at 60fps.. try different update methods
- warnings rel 1.x
- leaks rel1.x
- transition screen for switching between maps
- ui animation for player atb in battle.
  - not really needed right now, will just make it look better.
- when you start battle the anim is moving back
- refactor battle exit
- remove gameobject system from the C engine, only makes it annoying to work with
- Animation system should use regular pointers and needs an overhaul, why use handles
    : not needed right now, and works fine.
    : just don't try and cache the things, no need
- Check perf
    : [link](https://www.brendangregg.com/FlameGraphs/cpuflamegraphs.html)
- Sometimes player does not animate and just slides.
- Jitter when walking into walls and when at map center.
- Battle finger should only pop up after the animation
- Battle finger should disappear after selection before close anim
- Direction not correct when loaded from map exit.
- When switching screens, player doesn't load properly
    : prolly from battle exit
- Win screen
- Battle needs to set camera to 0
- go back to world
    : need to track what screen we were on - done
    : need to track where we were when we entered the battle - done
    : need to track what direction - done
    : cleanup properly
    : we should have battle system enable and disable the ui as needed
- currently crashes when gameobjects keep spawning, either it is getting deleted
    for some reason, or it isn't being allocated correctly in gameobject
    : somehow numGameobjects is higher than sizegameobjects
    : removing the counting system
- Enemy dies
- Target selection starts on enemy properly.
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

## Version / Release Roadmap

- 0.1.x: basic dialog with interactions, enter battle, can kill enemy, battle ended.
    : dialog - done
    : battle enter - done
    : battle finish - done
    : battle end - done
    : go through all todos and update - done
    : fix all leaks - done

- 0.2.x: local multiplayer
    : load all json at game load instead of on demand.
    : second player can spawn
    : camera follows main p1
    : second player can switch screens and interact with things
    : second battler
    : steam playtest - steamworks baby

- 0.3.0: Main menu and splash screens
    : open a menu when exploring, should be small enough for both
    : simple viewing of stats
    : add in splash screens when loading the game
    : add in a welcome menu, where you can new game, options, etc

- 0.4.0: Battle enhancements
    : inventory that you can have (shared)
    : equipment that can be used
    : equip and change stats with the equip
    : saving and loading
