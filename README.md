# Snake

A classic Snake game written in C++ with [raylib](https://www.raylib.com/), built with CMake.

## Features

- Grid-based movement with clean timing (independent of framerate)
- Input buffering, so quick key presses can't trigger an invalid 180° turn
- Growing snake body when food is eaten
- Random food spawning that never lands on the snake itself
- Collision detection (walls & self-collision)
- Score and highscore display
- Game-over and win screen with restart option

## Controls

| Action       | Keys                 |
|--------------|----------------------|
| Move         | `W A S D` or arrow keys |
| Restart      | `Enter` (after game over) |

## Requirements

- CMake ≥ 3.15
- A C++17-capable compiler (tested with MinGW/GCC on Windows)
- Internet connection for the first build (raylib is downloaded automatically via `FetchContent`)

## Build Instructions

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

The first `cmake ..` call downloads and compiles raylib, which can take a few minutes. After that, `cmake --build .` is enough to rebuild.

If you get a CMake policy error related to raylib during configuration, this flag can help:

```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5
```

## Running

```bash
./snake.exe
```

(inside the `build` folder, or `build/Debug` depending on the generator)

## Project Structure

```
snake_game/
├── CMakeLists.txt
├── src/
│   └── main.cpp
└── build/          <- generated, not included in the repo
```

## Technical Overview

The snake moves on a grid coordinate system instead of raw pixels, which makes collision checks much simpler. Movement itself runs on a time accumulator (`GetFrameTime()`), so it stays consistent regardless of framerate. The body is stored as a `std::vector<Vector2>`; on every movement step, each segment "slides" into the position of the segment before it, while the head actually moves in the current direction.

## License

This project is for learning purposes. raylib is licensed under the [zlib license](https://github.com/raysan5/raylib/blob/master/LICENSE).
