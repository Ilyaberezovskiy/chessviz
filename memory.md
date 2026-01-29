# Chessviz Project Memory

## Project Structure
- **Main executable**: `chessviz` (SFML-based chess game)
- **UI Components**: `src/ui/` (piece_renderer, board_renderer, window, input_handler)
- **Chess Logic**: `src/chess/` (board, piece, game, rules)
- **Build System**: CMake with SFML 2.6+ dependency

## Key Technical Details
- **SFML Version**: 2.6.1 (installed via libsfml-dev)
- **Build Command**: `cmake . && make`
- **Language**: C++17
- **No external image dependencies**: Pieces are drawn programmatically using SFML shapes

## Recent Changes
- **PieceRenderer redesign**: Completely replaced PNG-based rendering with programmatic SFML shape drawing
- **Benefits**: No external image dependencies, smaller executable, customizable piece appearance
- **Piece Colors**: 
  - White pieces: sf::Color(240, 240, 240) - light gray for visibility
  - Black pieces: sf::Color::Black
  - Outlines: Black (2.0f thickness)

## Piece Drawing Functions
Each piece has a dedicated drawing function using SFML shapes:
- `drawPawn()`: Circle + small circle on top
- `drawKnight()`: L-shape from two circles + head circle  
- `drawBishop()`: Large circle + smaller circle (hood) on top
- `drawRook()`: Rectangle base + 3 small rectangular "notches"
- `drawQueen()`: Circle base + 5 small circles (crown)
- `drawKing()`: Rectangle base + cross shape (vertical + horizontal rectangles)

## Build Environment
- **OS**: Ubuntu 24.04
- **Compiler**: GCC 13.3.0
- **SFML**: Installed via apt-get (libsfml-dev)
- **X11**: Not available in headless environment (expected)

## Important Files Modified
- `src/ui/piece_renderer.h`: Changed from texture-based to programmatic rendering
- `src/ui/piece_renderer.cpp`: Complete rewrite with SFML shape-based piece drawing