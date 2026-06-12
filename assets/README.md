# Brotato-style Arena Shooter

This is a C++ / SFML implementation of a simple Brotato-style arena shooter.

## Concept
The player controls a potato hero inside an arena, survives enemy waves, collects materials, chooses upgrades between waves, and tries to survive as long as possible.

## Project requirements covered

- Division into `.cpp` and `.h` files.
- Inheritance:
  - `GameObject -> Player`
  - `GameObject -> Enemy -> MeleeEnemy / FastEnemy / RangedEnemy`
  - `GameObject -> Projectile`
  - `GameObject -> Pickup -> MaterialPickup / HealthPickup`
- Polymorphism and storing all game objects in one container:
  - `std::vector<std::unique_ptr<GameObject>> objects;`
- Usage of `std::unique_ptr`.
- Usage of `dynamic_cast` for collision detection.
- Base class `Game` supervising the course of the game.
- Public / protected / private class fields.
- Clock and `deltaTime` for movement and animation.
- Random enemy spawning and random pickup drops.
- Game parameterization through `GameConfig`.
- Simple sprite animations.
- Detailed comments in the source code.

## Controls

- `WASD` / `Arrow keys` - move
- `ESC` - pause
- `1 / 2 / 3 / 4` - choose upgrade in shop
- `R` - restart after game over

## How to run

Open this project folder in Visual Studio Code and press `F5`.

The project expects MSYS2 UCRT64 with SFML installed.


## Visual assets

Original custom sprite set created specifically for this project.
