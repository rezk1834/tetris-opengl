# Tetris Game

A simple and engaging Tetris game built using **C++** and **OpenGL**. This project demonstrates fundamental computer graphics concepts such as shape rendering, animations, and user interaction, with added enhancements for improved gameplay experience.

## Features

- **Interactive Gameplay**:
  - Move blocks left, right, or down.
  - Rotate tetrominoes for better placement.
  - Pause and resume functionality.
  - Enhanced difficulty progression as the game advances.
- **Dynamic UI**:
  - Visual grid with highlighted borders for clarity.
  - Real-time score and level display.
  - Color-coded tetrominoes for easy differentiation.
- **Game Mechanics**:
  - Automatically detects and clears completed lines.
  - Ends the game when the grid is full.
  - Adaptive speed increases based on levels.

## Controls

- **left arrow**: Move tetromino left.
- **right arrow**: Move tetromino right.
- **down arrow**: Move tetromino down faster.
- **up arrow**: Rotate tetromino clockwise.
- **P**: Pause or resume the game.

## Requirements

- C++ Compiler (e.g., GCC, MinGW)
- OpenGL and GLUT libraries installed on your system.

## Setup and Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/rezk1834/tetris-opengl.git
   cd tetris-opengl
   ```

2. **Compile the code**:
   Use any C++ compiler with OpenGL and GLUT support. For example, with `g++`:
   ```bash
   g++ tetris.cpp -o tetris -lGL -lGLU -lglut
   ```

3. **Run the game**:
   ```bash
   ./tetris
   ```

## How It Works

1. **Game Grid**:
   - The game uses a 10x20 grid to display tetrominoes.
   - Each tetromino consists of 4 blocks, which can move or rotate.

2. **Collision Detection**:
   - Ensures that tetrominoes do not overlap, move out of bounds, or interfere with existing blocks.

3. **Line Clearing**:
   - A row is cleared when it is completely filled with blocks.
   - The score increases by 100 points per cleared row.
   - Clearing multiple lines simultaneously grants bonus points.

4. **Level System**:
   - Progress to the next level after clearing a set number of lines.
   - Game speed increases with higher levels for added challenge.

5. **Game Over**:
   - The game ends when no space is available for a new tetromino.
## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- **OpenGL** for graphics rendering.
- Inspiration from the classic **Tetris** game.
- Built as part of a Computer Graphics course project.

---

**Contributions** are welcome! Feel free to fork this repository, make improvements, and submit pull requests.

--- 
