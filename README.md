# Cellular Automata SandBox

A particle simulation engine built with C++ and SDL2 that creates dynamic physics-based interactions between different particle types.

## Overview

This project implements a falling sand simulation with various particle types that interact with each other following cellular automata rules. Watch as particles fall, flow, burn, and create complex emergent behaviors in a visual sandbox environment.

## Features

### Particle Types
- **Sand** - Falls and piles up when it hits obstacles
- **Water** - Flows and fills spaces, evaporates near fire
- **Stone** - Static, immovable material
- **Wood** - Flammable, burns to smoke when exposed to fire
- **Fire** - Spreads and consumes flammable materials
- **Smoke** - Light particles that rise and disperse
- **Empty** - Vacuum that particles can move into

### Functionality
- 🎨 Real-time particle visualization with 800x800 grid
- 🖱️ Interactive drawing tools with adjustable brush size
- 🔥 Physics-based particle interactions (gravity, spreading, combustion)
- 💾 Save and load custom maps
- ⌨️ Keyboard controls for all features

## Controls

### Particle Selection
- **s** - Select Sand
- **w** - Select Water
- **k** - Select Stone
- **d** - Select Wood
- **f** - Select Fire
- **p** - Select Smoke
- **e** - Select Empty (eraser)

### Brush Size
- **0-9** - Set brush size (0 = smallest, 9 = largest)

### General
- **r** - Reset/clear the grid
- **m** - Enter map editor mode
- **l** - Load a saved map
- **q** - Quit (in map editor mode)

### Map Editor Mode
- **a** - Save current map
- **r** - Reset grid in editor
- **q** - Exit editor mode

## Building & Requirements

### Dependencies
- C++ compiler (C++11 or later)
- SDL2 library
- SDL2 development headers

### Compilation
```bash
g++ -o sandbox main.cpp particles.cpp -lSDL2
```

### Running
```bash
./sandbox
```

## Project Structure

```
Cellular-Automata-SandBox/
├── main.cpp          # Main application loop and rendering
├── particles.cpp     # Particle physics and update logic
├── particles.h       # Header file with constants and function declarations
├── maps/             # Directory for saving/loading custom maps
└── README.md         # This file
```

## How It Works

### Particle Physics
Each particle type has specific update rules:

- **Sand**: Falls due to gravity, comes to rest on obstacles
- **Water**: Flows sideways and downward, spreads when hitting surfaces
- **Fire**: Spreads to adjacent flammable materials (wood), creates smoke
- **Smoke**: Rises upward and slowly disperses
- **Wood**: Burns when surrounded by fire, converting to smoke

### Grid System
The simulation runs on a 200x200 cell grid (800x800 pixels at 4px per cell). Each frame, particles are updated based on their type and interactions with neighbors.

## Map System

Save and load custom scenarios:
1. Press **m** to enter map editor mode
2. Draw your scene
3. Press **a** to save - you'll be prompted for a map name
4. Press **q** to exit editor
5. Press **l** to load a previously saved map

Maps are stored in the `maps/` directory as text files.

## Performance

- Optimized cell updates using randomized left-to-right scanning
- Efficient texture-based rendering with SDL2
- 3ms per frame cap for smooth 60+ FPS simulation

## Future Enhancements

- Additional particle types (oil, acid, etc.)
- Temperature-based interactions
- Pressure mechanics
- Advanced visualization modes
- Performance optimizations for larger grids

## Author

Created by LeonLj1

## License

This project is open source. Feel free to use, modify, and distribute as you see fit.

---

**Enjoy experimenting with particle physics!** 🎮✨
