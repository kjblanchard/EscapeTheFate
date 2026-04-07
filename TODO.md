# Escape The Fate

## Backlogged

- More debug utils, copy from old repo
- BGM fadeout and fadein
- add enemy health bar and player health bar
- check on discord bot updates.
- More fields that can be optional to make it easier to write the jsonc file
  - not really needed right now
- Dialog system slidefade anim .. would be cool if we could handle tweening the opacity
- Better error handling / loading from json
- Use gameconfig to handle the dialog box variables, instead of const

## Version / Release Roadmap

- 0.2.x: local multiplayer
    : load all json at game load instead of on demand. - done
    : controllers and controller swapping - done
    : second player can spawn
    : camera follows main p1
    : second player can switch screens and interact with things
    : second battler
    : steam playtest - steamworks baby
    : use packed assets (.sg)

- 0.3.0: Main menu and splash screens
    : open a menu when exploring, should be small enough for both
    : settings screen in menu
    : simple viewing of stats
    : add in splash screens when loading the game
    : add in a welcome menu, where you can new game, options, etc
    : saving and loading

- 0.4.0: Battle enhancements
    : inventory that you can have (shared)
    : equipment that can be used
    : equip and change stats with the equip

## Previous releases

- 0.1.x: basic dialog with interactions, enter battle, can kill enemy, battle ended.
    : dialog - done
    : battle enter - done
    : battle finish - done
    : battle end - done
    : go through all todos and update - done
    : fix all leaks - done
