---
name: sfml-game-dev
description: "Expert guidance for developing 2D games with C++ and SFML. Use when Gemini CLI needs to implement game loops, entity systems, asset management, tilemaps, or physics in an SFML-based project."
---

# SFML Game Development

This skill provides specialized workflows and patterns for building robust 2D games using the Simple and Fast Multimedia Library (SFML) in C++.

## Core Workflows

### 1. Game Architecture & Entity System
Avoid putting all logic in `main.cpp`. Prefer a modular approach with an `Entity` base class or specialized classes for `Player`, `NPC`, and `Environment`.

- **Workflow:** When adding a new game element, create a class that encapsulates its state (position, texture) and behavior (`update`, `draw`).
- **Example:** See [SFML_SNIPPETS.md](references/sfml_snippets.md#entity-structure) for a recommended class layout.

### 2. Asset Management
Loading textures repeatedly causes performance issues. Always load textures once and share them among sprites.

- **Rule:** Keep `sf::Texture` objects in a scope that outlives the `sf::Sprite` objects that use them.
- **Pattern:** Use an `AssetManager` or a `std::map<std::string, sf::Texture>` to cache assets.

### 3. Collision & Physics
For tile-based games like this one, use AABB (Axis-Aligned Bounding Box) collisions.

- **Workflow:** 
    1. Calculate movement velocity.
    2. Move on the X axis.
    3. Check for collisions; if colliding, revert X movement.
    4. Move on the Y axis.
    5. Check for collisions; if colliding, revert Y movement.
- **Example:** See [SFML_SNIPPETS.md](references/sfml_snippets.md#collision-detection) for implementation details.

### 4. Sprite Animation
Use `sf::IntRect` to define the "view" into a sprite sheet.

- **Workflow:**
    1. Maintain a `sf::Clock` for animation timing.
    2. Store the current frame index and animation state.
    3. Update the `IntRect` position based on elapsed time.
- **Rule:** Animation speed should be frame-rate independent (use `sf::Time` or `asSeconds()`).

## Reference Material

- **Code Snippets:** [SFML_SNIPPETS.md](references/sfml_snippets.md) contains ready-to-use patterns for common tasks.
- **Asset Integration:** Guidance on using local assets (like `player.png` or `TX Tileset Grass.png`).

## Common Gotchas

- **White Square Problem:** Ensure the `sf::Texture` object is not destroyed while a `sf::Sprite` is still using it.
- **Event Polling:** Always use a `while(window.pollEvent(event))` loop to ensure the OS window stays responsive.
- **Coordinate Systems:** SFML's origin (0,0) is at the top-left. Positive Y is down.
