# Chess Game

A fully-functional graphical chess game built with C++ and SFML, featuring a chess.com-like interface.

## Features

### Interface and Visualization
- Modern graphical interface with chess board
- Visual representation of all chess pieces (black and white)
- Highlighting of legal moves when selecting a piece
- Highlighting of the last move made
- Game status information (whose turn, check, checkmate, etc.)

### Functionality
- Complete implementation of chess rules:
  - Correct movement for all pieces (King, Queen, Rook, Bishop, Knight, Pawn)
  - Move validation (cannot move if king would be in check)
  - Check and checkmate detection
  - Draw conditions (stalemate, 50-move rule, threefold repetition)
  - Castling (kingside and queenside)
  - En passant capture
  - Pawn promotion

### Gameplay
- Local multiplayer (two players on one computer, taking turns)
- Interactive controls: click piece → see legal moves → click target square
- "New Game" button to restart
- "Undo" button to take back moves

## Build Requirements

- CMake 3.10 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- SFML 2.5+ (will be automatically downloaded if not found)

## Building the Game

### Linux/macOS

```bash
# Install SFML (optional, CMake will fetch it if not found)
# Ubuntu/Debian:
sudo apt-get install libsfml-dev

# macOS:
brew install sfml

# Build the game
mkdir build
cd build
cmake ..
make

# Run the game
./chess
```

### Build with System SFML (faster)

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./chess
```

### Build with FetchContent (if SFML not installed)

```bash
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)
./chess
```

## How to Play

1. **Starting a Game**: The game starts automatically with white's turn
2. **Making a Move**:
   - Click on a piece to select it
   - Legal moves will be highlighted with dots
   - Click on a highlighted square to move the piece
3. **Special Moves**:
   - **Castling**: Select the king and click two squares towards the rook
   - **En Passant**: Automatically available when conditions are met
   - **Pawn Promotion**: When a pawn reaches the opposite end, choose the piece to promote to
4. **Game Controls**:
   - **New Game**: Click the button to restart
   - **Undo**: Click to take back the last move
5. **Game End**: The game will notify you when checkmate, stalemate, or draw occurs

## Architecture

The project follows clean architecture principles with separation of concerns:

- `Types.h` - Core data types and enums
- `Piece.h/cpp` - Piece representation and movement logic
- `ChessBoard.h/cpp` - Board state management
- `MoveValidator.h/cpp` - Move validation and legal move generation
- `GameState.h/cpp` - Game state management, move history, and game rules
- `Renderer.h/cpp` - Graphics rendering and UI
- `ChessGame.h/cpp` - Main game loop and event handling

## License

This project is open source and available for educational purposes.
