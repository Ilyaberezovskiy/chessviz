# Chess Game (chessviz)

A full-featured graphical chess game written in C++ using SFML.

## Features
- Complete chess rules (Castling, En Passant, Promotion)
- Move validation and check/mate detection
- Chess.com-like interface
- Highlight valid moves and last move
- Undo (U key) and Reset (R key) functionality

## Requirements
- C++17 compiler
- CMake 3.15+
- SFML 2.5+

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
./chessviz
```

## Controls
- **Left Mouse Click**: Select piece and move.
- **U**: Undo last move.
- **R**: Reset game.
